import csv
import os
import urllib.request
import pdb
import decimal

from flask import redirect, render_template, request, session
from cs50 import SQL
from datetime import date
from functools import wraps

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Check if a string represents an integer
def represents_int(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/0.12/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function

def lookup(symbol):
    if symbol == "GOOGL":
        return {"price": 1204.42,"symbol": "GOOGL"}

    elif symbol == "NFLX":
        return {"price": 395.80,"symbol": "NFLX"}

    elif symbol == "MSFT":
        return {"price": 105.43, "symbol": "MSFT"}

    elif symbol == "CMG":
        return {"price": 457.21, "symbol": "CMG"}
    else:
        return None

def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"

def update_portfolio_pricing(portfolio):
    for row in portfolio:
        if row["price"] != lookup(row["symbol"])["price"]:
            rows = db.execute("UPDATE portfolio SET price = :new_price, total = :new_total",
                        new_price = lookup(row["symbol"])["price"],
                        new_total = lookup(row["symbol"]) * row["shares"])

def add_portfolio_shares(result, symbol, shares):
    db.execute("UPDATE portfolio set shares = :new_shares WHERE symbol = :same_symbol AND userid = :user_id",
                              new_shares = int(shares) + result[0]["shares"], same_symbol = symbol, user_id = session["user_id"])

def remove_portfolio_shares(result, symbol, shares):
    db.execute("UPDATE portfolio set shares = :new_shares WHERE symbol = :same_symbol AND userid = :user_id",
                              new_shares = result[0]["shares"] - int(shares), same_symbol = symbol, user_id = session["user_id"])

def insert_in_portfolio(symbol, shares, stock_info):
    db.execute("INSERT INTO portfolio (symbol, shares, userid, total, price) VALUES(:symbol, :shares, :userid, :total, :new_price)",
                            symbol = symbol,
                            shares = shares,
                            userid = session["user_id"],
                            total = stock_info["price"] * int(shares),
                            new_price = stock_info["price"])

def insert_in_users(username, hashPw):
    return db.execute("INSERT INTO users (username, hash) VALUES(:username, :hashPw)", username = username, hashPw = hashPw)

def insert_in_order_history(symbol, shares, price):
    db.execute("INSERT INTO orderhistory (userid, symbol, shares, total, date) VALUES (:userid, :symbol, :shares, :total, :date)",
                  userid = session["user_id"], symbol = symbol, shares = shares, total = price, date = date.today())

def get_portfolio():
    return db.execute("SELECT * FROM portfolio WHERE userid = :userid", userid = session["user_id"])

def get_portfolio_shares(symbol):
    return db.execute("SELECT shares FROM portfolio where symbol = :symbol AND userid = :user_id",
                        symbol = symbol, user_id = session["user_id"])

def get_portfolio_worth():
    return db.execute("SELECT SUM(total) AS total FROM portfolio WHERE userid = :user_id", user_id = session["user_id"])

def get_order_history():
    return db.execute("SELECT * FROM orderhistory WHERE userid = :userid", userid = session["user_id"])

def get_cash():
    return round(decimal.Decimal(get_user_with_id()[0]["cash"]),2)

def get_user_with_id():
    return db.execute("SELECT * FROM users WHERE id = :userid",
                          userid = session["user_id"])

def add_cash(shares, stock_info):
    db.execute("UPDATE users SET cash = :new_cash WHERE id = :userid",
                            new_cash = get_user_with_id()[0]["cash"] + int(shares) * stock_info["price"],
                            userid = session["user_id"])

def subtract_cash(shares, stock_info):
    db.execute("UPDATE users SET cash = :new_cash WHERE id = :userid",
                            new_cash = get_user_with_id()[0]["cash"] - int(shares) * stock_info["price"],
                            userid = session["user_id"])

def contains_stock(symbol):
     return db.execute("SELECT * FROM portfolio where symbol = :bought_symbol AND userid = :user_id", bought_symbol = symbol, user_id = session["user_id"])

def check_for_username(username):
    return db.execute("SELECT * FROM users WHERE username = :username",
                          username = username)

def check_password_matches_confirm(password, passwordConfirm):
    if password !=  passwordConfirm:
            return apology("Your password confirmation did not match your password."
                           + "Please try again.", 403)

def check_valid_symbol(symbol):
    if not lookup(symbol):
            return apology("Please enter a valid symbol", 403)

def check_form_filled_in(formvalue, errormessage):
    if not request.form.get(formvalue):
            return apology(errormessage, 403)

def check_enough_cash(price):
    rows = db.execute("SELECT * FROM users WHERE id = :userid",
                          userid = session["user_id"])
    if price > rows[0]["cash"]:
        return apology("Not enough cash :(", 403)

def delete_all_shares_of_symbol(symbol):
    db.execute("DELETE FROM portfolio WHERE symbol = :symbol AND userid = :userid", symbol = symbol, userid = session["user_id"])
