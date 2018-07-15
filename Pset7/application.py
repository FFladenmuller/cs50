import os

import pdb
import decimal
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import *

# Ensure environment variable is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    cash = get_cash()

    update_portfolio_pricing(get_portfolio())

    # Get total worth of user
    userworth = get_portfolio_worth()

    # If no orders
    if not userworth[0].get("total", "none"):
        total = round(cash, 2)
    else:
        total = userworth[0].get("total", "none") + float(cash)

    # Redirect user to portfolio page
    return render_template("portfolio.html", rows=get_portfolio(), usd=usd, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Must provide symbol")
        if not request.form.get("shares"):
            return apology("Must provide shares")
        shares = request.form.get("shares")
        symbol = request.form.get("symbol")
        if not lookup(request.form.get("symbol")):
            return apology("Must enter a valid symbol")
        if not shares or not represents_int(shares) or int(shares) < 1:
            return apology("Enter a valid number of shares")
        stock_info = lookup(request.form.get("symbol"))
        shares = request.form.get("shares")
        symbol = request.form.get("symbol")
        price = int(shares) * stock_info["price"]
        check_enough_cash(price)
        subtract_cash(shares, stock_info)
        insert_in_order_history(symbol, shares, lookup(symbol)["price"])
        if not contains_stock(symbol):
            insert_in_portfolio(symbol, shares, stock_info)
        # Else increment stock
        else:
            add_portfolio_shares(get_portfolio_shares(symbol), symbol, shares)
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return render_template("history.html", rows=get_order_history(), usd=usd)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""
    # Forget any user_id
    session.clear()
    if request.method == "POST":
        username = check_for_username(request.form.get("username"))
        # Ensure username exists and password is correct
        if not username or not check_password_hash(username[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password")
        # Remember which user has logged in
        session["user_id"] = username[0]["id"]
        return redirect("/")
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""
    # Forget any user_id
    session.clear()
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not lookup(request.form.get("symbol")):
            return apology("Please enter a valid symbol")
        # Quote stock
        stock_info = lookup(request.form.get("symbol"))
        return render_template("quoted.html", symbol=stock_info["symbol"], price=stock_info["price"], usd = usd)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("Please provide a username")
        if not request.form.get("password"):
            return apology("Please provide a password")
        if request.form.get("password") !=  request.form.get("confirmation"):
            return apology("Your password confirmation did not match your password.")
        hashPw = generate_password_hash(password=request.form.get("password"), method='pbkdf2:sha256', salt_length=8)
        # Insert user to DB, Check that insertion was successful
        if not insert_in_users(request.form.get("username"), hashPw):
            return apology("Username has already been taken. Please try another one.")
        # Log user in
        session["user_id"] = check_for_username(request.form.get("username"))[0]["id"]
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        if not request.form.get("shares"):
            return apology("Please enter a valid number of shares")
        shares = request.form.get("shares")
        symbol = request.form.get("symbol")
        # Check that user has enough shares to sell equal to input amount
        p_shares = get_portfolio_shares(symbol)[0].get("shares", "none")
        if p_shares < int(shares):
            return apology("You do not own that many shares. Please enter a valid number.")
        if p_shares == int(shares):
            delete_all_shares_of_symbol(symbol)
        else:
            remove_portfolio_shares(get_portfolio_shares(symbol), symbol, shares)
        insert_in_order_history(symbol, -int(shares), lookup(symbol)["price"])
        add_cash(shares, lookup(symbol))
        return redirect("/")
    else:
        return render_template("sell.html", symbols=db.execute("SELECT symbol FROM portfolio WHERE userid = :userid", userid=session["user_id"]))


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
