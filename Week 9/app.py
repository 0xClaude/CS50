import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd, datetimeformat

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Added a custom filter to better display the time and date
app.jinja_env.filters['datetimeformat'] = datetimeformat

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/profile", methods=["GET", "POST"])
@login_required
def profile():
    """ Lets the user change his or her password """
    if request.method == "GET":

        # Get the name for current user
        username = db.execute(
            "SELECT * FROM users WHERE id = (?)", session["user_id"])

        # Prepare which data is transmitted
        userdata = {
            "id": session["user_id"],
            "username": username[0]["username"],
        }
        return render_template("profile.html", user=userdata)
    else:
        # Check data integrity
        if not request.form.get("password"):
            return apology("No password entered", 403)

        # Hash the password
        password = generate_password_hash(request.form.get("password"))

        # Update the table
        db.execute("UPDATE users SET hash = (?) WHERE id = (?)",
                   password, session["user_id"])

        # Redirect to home
        return redirect("/")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Get the portfolio
    portfolio = db.execute(
        "SELECT * FROM portfolio WHERE userid = (?) ", session["user_id"])

    # Check if user has a portfolio at all
    if len(portfolio) == 0:
        cash = db.execute(
            "SELECT cash FROM users WHERE id = (?)", session["user_id"])
        cash = usd(cash[0]["cash"])
        return render_template("index.html", cash=cash, total=cash)
    else:
        # Set an new list to track investments
        data = []

        # Calculate the grand total
        totalvalue = 0

        # Loop through every stock owned
        for item in portfolio:
            # Check the price and calculate the value
            price = lookup(item["stock"])
            value = price["price"] * item["amount"]

            # Update the grand total
            totalvalue += value

            # Create temporary dictionary to keep track
            temp = {
                "stock": item["stock"],
                "amount": item["amount"],
                "price": usd(price["price"]),
                "value": usd(value),
            }

            # Add data
            data.append(temp)

        # Check how much cash the user has
        cash = db.execute(
            "SELECT cash FROM users WHERE id = (?)", session["user_id"])
        totalvalue += cash[0]["cash"]
        cash = usd(cash[0]["cash"])

        return render_template("index.html", shares=data, cash=cash, total=usd(totalvalue))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Check for data integrity
        if not request.form.get("symbol"):
            return apology("Symbol is needed", 400)
        symbol = request.form.get("symbol")

        if not request.form.get("shares"):
            return apology("Number of shares is needed", 400)
        if not request.form.get("shares").isnumeric():
            return apology("Need a numeric value for shares", 400)
        # Check if Symbol exists
        check = lookup(symbol)
        if not check:
            return apology("There is no such symbol", 400)

        # Get some information about the stock
        shares = request.form.get("shares")
        price = check["price"]

        # Calculate if the user can afford the stocks
        due = float(shares) * float(price)

        user = db.execute("SELECT * FROM users WHERE id=(?)",
                          session["user_id"])
        cash = user[0]["cash"]
        if due > cash:
            return apology("You don't have enough cash", 403)

        # Add the new transaction(s) to the database
        action = "buy"
        db.execute("INSERT INTO transactions (buyer, stock, price, amount, total, action) VALUES(?, ?, ?, ?, ?, ?)",
                   session["user_id"], symbol, price, shares, due, action)

        # subtract the purchased amount
        rest = cash - due
        db.execute("UPDATE users SET CASH = (?) WHERE id = (?)",
                   rest, session["user_id"])

        #  Update portfolio
        # Check first if the user already has this stock

        portfolio_check = db.execute(
            "SELECT * FROM portfolio WHERE userid = (?) AND stock = (?)", session["user_id"], symbol)
        if len(portfolio_check) == 0:
            # The user has no shares of this company
            db.execute("INSERT INTO portfolio (userid, stock, amount) VALUES(?, ?, ?)",
                       session["user_id"], symbol, shares)
        else:
            # If not, add new row
            db.execute("UPDATE portfolio SET amount = amount + (?) WHERE userid = (?) AND stock = (?)",
                       shares, session["user_id"], symbol)

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    data = db.execute(
        "SELECT * FROM transactions WHERE buyer = (?)", session["user_id"])
    print(data)
    return render_template("history.html", log=data)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        if not request.form.get("symbol"):
            return apology("Must enter a Symbol", 400)
        stock = lookup(request.form.get("symbol"))
        if stock == None:
            return apology("No such stock", 400)
        return render_template("quoted.html", data=stock)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Check if username is provided
        if not request.form.get("username"):
            return apology("Must enter a username", 400)
        username = request.form.get("username")

        # Check if password is provided
        if not request.form.get("password"):
            return apology("Must enter a password", 400)

        # Check if password confirmation is provided
        if not request.form.get("confirmation"):
            return apology("Missing password confirmation", 400)

        # Check if passwords match
        if not request.form.get("password") == request.form.get("confirmation"):
            return apology("Passwords don't match", 400)

        # Hash password
        password = generate_password_hash(request.form.get("password"))

        # Check if user exists
        usernames = db.execute(
            "SELECT * FROM users WHERE username = (?)", username)
        if len(usernames) != 0:
            return apology("Username already taken", 400)
        else:

            # Save user to database and redirect to login
            db.execute(
                "INSERT INTO users (username, hash) VALUES(?, ?)", username, password)
            return redirect("/login")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        # Get a list of shares the user has
        portfolio = db.execute(
            "SELECT * FROM portfolio WHERE userid = (?)", session["user_id"])
        if len(portfolio) != 0:
            return render_template("sell.html", stocks=portfolio)
        else:
            return redirect("/buy")
    else:
        # Check data integrity
        if not request.form.get("shares"):
            return apology("Please specify the amount you want to sell", 400)

        if int(request.form.get("shares")) < 0:
            return apology("Please enter a positive number", 400)

        amount = int(request.form.get("shares"))
        symbol = request.form.get("symbol")

        # Check if user has enough stocks
        pocket = db.execute(
            "SELECT amount FROM portfolio WHERE userid = (?) AND stock = (?)", session["user_id"], symbol)

        if amount > pocket[0]["amount"]:
            return apology("You don't own enough stocks")
        else:
            # Add to transactions

            price = lookup(symbol)
            price = price['price']
            total = amount * price
            action = "sell"
            db.execute("INSERT INTO transactions (buyer, stock, price, amount, total, action) VALUES(?, ?, ?, ?, ?, ?)",
                       session["user_id"], symbol, price, amount, total, action)

            # remove from portfolio
            # particular case: if user sold all of his shares
            if amount == pocket[0]["amount"]:
                db.execute(
                    "DELETE FROM portfolio WHERE stock = (?) AND userid = (?)", symbol, session["user_id"])
            else:
                db.execute("UPDATE portfolio SET amount = amount - (?) WHERE userid = (?) AND stock = (?)",
                           amount, session["user_id"], symbol)

            # Update cash
            db.execute(
                "UPDATE users SET cash = cash + (?) WHERE id = (?)", total, session["user_id"])

            return redirect("/")
