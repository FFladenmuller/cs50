___
# Griffen's Programming Help
___

Hey Griffen, this is a little guide designed to help you. Good luck!

## General Ideas About Programming

Programming is like telling a very dumb person what to do exactly. If I wanted to tell a computer how to 
make a peanut butter jelly sandwich, I might give it instructions like this:
```
Open bread bag
Take 2 slices of bread out
Close bread bag
Get knife
Put peanut butter on knife
Spread peanut butter on bread
```
And so on. Obviously this is not actual code, But the best way I have found to program is to write out my
ideas of how the program should work in regular english, and then once I have finished I can replace my english
with actual code. Kind of like translating one language into another. Writing "fake" code or writing out your
program in english is called pseudocode. 

## Classes

#### What is a class?
A class represents an object, or a noun. It could represent a dog, 
an iphone, or some more abstract thing, like a windows form.

#### What does a class include?
A class includes things that describe that class, and actions that class can perform.
The things that describe the class are variables, and actions the class can perform are functions/subs.

#### Class Examples
Class | Description | Action
---   | ---| --- |
Dog | Color, age, breed |Bark, wag tail, bite
Iphone|Model, model #, color, battery life |Send text, flashlight on, take picture
Windows Form|Textbox text, button information| Change textbox, display something


#### What is an instance of a class, or an object?
A class itself is like a blueprint, or an idea. An instance/object is a copy of the class, in usable form.

#### Instance/Object Examples
Class |Object Example
---|---
Dog |Your personal dog, or any specific dog. 
Iphone| Griffen’s Iphone X
Windows Form| Your particular form you are working with

#### How do I declare a class?
Your programs will be wrapped inside of your class. So at the beginning of your program, should be your class
name, and then the last line should be End Class. Like this:
```vb
Public Class Dog

[Rest of program here]

End Class

```
Ignore the public next to class for now. 

## Variables 
#### Why do we use variables?
To store information about something. 

#### Variable Types


Variable Type | What does it store?
--- | ---
Boolean |True/False
String |Text
Character |One letter/number
Integer/Long |Whole number/Bigger whole numbers
Float/Double/Decimal| Decimal numbers, with 7/16/28 digits respectively
Object| Instance of a class. 

#### How do I declare variables?
Following this formula: 
```vb
Dim variableName As variableType
```
This defines a variable with the name you gave it, as the type you want it to be.

## Functions/Sub Routines:

#### What is the purpose of a function/sub?
To perform some action. It could be making a robot move, shuffling a virtual deck of cards, sending a text, etc. 
A function could also be used to get information.

#### What is a return value?
A return value is the information returned from a function. Basically you are asking the function some question
in code, like what is the age of my dog? and it will return/give you back a number. 

#### Return Value Examples

Information Wanted | Return Type | What is returned?
--- | ---| ---
Is this person my friend on facebook? | Boolean | True or false
What is the age of this dog? | Integer | 1
What is my phone’s battery life?| Integer |87
What is the average of temperatures in my textboxes? |Decimal |79.3

#### What’s the difference between a function and a sub?
A function has a return value. A sub is a function but you don’t want any information 
back from it, so it has no return value.


#### Function Vs Sub Examples
Function | Function or Sub?
--- | ---
Find the nearest restaurants |Function: it is getting information (nearest restaurants)
Shuffle deck of cards |Sub: it is not telling us anything, just performing an action.
Turn on flashlight |Sub:  it is not telling us anything, just performing an action.

#### What is a parameter?
Sometimes, a function needs information in order to be able to do its job. 
A parameter is information you give to the function, so that it can complete its job.

#### Parameter examples

Function | Example Parameter
--- | ---
Find Nearest Restuarants | Your location/coordinates, so that the function knows where to search by
Calculate temperature average of 3 months | The temperature average of month one, month two, and month three, so it can add these three together and divide them by 3. 

#### In code, how do I declare a sub/function?
When you are initially making the sub/function, you need to declare it, like a variable, like this:
```vb
Function FunctionName [(ParameterList)] As ReturnType  
    [Statements]  
End Function  
```
This might look really confusing and technical, like a bunch of random weird words. But if you go word by word
and break it down, it isn't so bad. By declaring this you are saying, I am making a Function called FunctionName.
I want to have X, Y, and Z as parameters, because my function needs those variables to calculate something. Lastly 
I am returning this number. Then, you write your code inside the function to do whatever you need, and end the function.
If this is still confusing, try looking through this example: 
```vb
Function calculateSummerAverage[(juneTemp As double, julyTemp As double, augustTemp as double)] As double
    Return math.Round((juneTemp +julyTemp + augustTemp) / 3)
End Function
```
So here is an example of a function from your homework I helped you with. First I declare that I am making a function 
called calculateSummerAverage. After that, I make a pair of square brackets, and inside that parentheses. Then I 
type out all the parameters I need for this method (the average temperature in june, july, and august), and I tell
the function what kind of variable these parameters are (they are all doubles). The "As double" after the square bracket closes
means that the function returns a double. Then, all I am doing is returning the temperature average of the summer months, rounded off
by the math.Round() function (so that the temperature isn't a million decimal places long). Now the function has done it's work, and we 
can type end function. There is definitely alot to take in, but after a little practice it becomes second nature. 

#### How do I use a function, after I have written/declared it?
Just because you wrote it, doesn't mean the function will do anything. You have to call it's name in order to use it.
To use a function, you simply type it's name, put an opening parentheses, write in your parameters (note : when calling 
a method you don't have to write the variable type after write the variable name. I will show this in a minute), and 
write the closing parentheses. So to use the method we just wrote:

```vb
calculateSummerAverage(juneTemp, julyTemp,augustTemp)
```
Notice I don't write As double after the variable names. Now if we were to just type this function into our program, the
return value wouldn't be saved anywhere, so that line would be pointless. Instead you would do this: 

```vb
dim summerAverage As double
summerAverage = calculateSummerAverage(juneTemp, julyTemp,augustTemp)
```
or you could do this: 
```vb
dim summerAverage As double = calculateSummerAverage(juneTemp, julyTemp,augustTemp)
```
or lastly using "local type inference", where the computer knows that summerAverage is a double already because
the return type of calculateSummerAverage is a double:
```vb
dim summerAverage = calculateSummerAverage(juneTemp, julyTemp,augustTemp)
```

## Loops

#### What is a For...Next loop?
A for loop is a loop where something repeats a specific amount of times. For example if you wanted to print the
letter 'a' 5 times, or add 1 to a number 50 times. 

#### How do I make a For...Next loop?
```vb
For counter As datatype  = start To end 
    [code to repeat]
Next
```
Again this might look confusing, but try looking at this next example.

```vb
For index As Integer = 1 To 5
    Debug.Write('a')
Next
```
So here is an example where we are going to get the letter 'a' printed out 5 times. Basically, this is saying make 
index a variable that is an integer. Initially it will be set to equal 1. Everytime the code gets to "Next", it will 
go to the next loop through, and increase index to the next number. Like this:

```vb
For index As Integer = 1 To 5 (index = 1)
  Debug.Write('a')
Next

For index As Integer = 1 To 5 (index = 2)
  Debug.Write('a')
Next

For index As Integer = 1 To 5 (index = 3)
  Debug.Write('a')
Next
```
and so on. For...next loops can be used in alot of creative ways, and are very useful. There are more configurations
on how to use them, which can be found online, but that is the basic setup of them.

#### What is a While...End loop?
A while loop is a loop that will continue forever, so long as whatever condition you set it to is true. 

#### How do I make a While...End loop?
Here is the basic code for one:
```vb
While condition = true
    [ statements ]   
End While
```
And here is one with some actual filled in code:
```vb
boolean griffDating = true
While griffDating = true
  Debug.Write("Griffen is a hoe")
End While
```
So this loop will go on forever. If you changed griffDating to equal false in the loop however, it would end,
after it hit the next While griffDating = true. It is the same as For...Next in the sense that after it hits the 
bottom line of the loop it will go back to the top and repeat again. 

#### For....Next vs While...End
So basically, For...Next is when you want something to happen a certain number of times, while the a While...End 
is when you want something to happen forever or you don't know how long it needs to go on.


