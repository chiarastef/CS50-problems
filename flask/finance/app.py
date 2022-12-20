# Problem link: https://cs50.harvard.edu/x/2022/psets/9/finance/

import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

import datetime

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

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


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user_id = session["user_id"]

    # Transactions info
    transactions = db.execute(
        "SELECT symbol, SUM(shares) AS shares, price, price*shares AS total, name FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)
    # User's cash
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

    # Get grand total (stocks’ total value plus cash).
    total = cash
    for transaction in transactions:
        total = total + transaction["total"]

    return render_template("index.html", data=transactions, cash=usd(cash), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("shares must be a positive integer", 400)

        # If field is left blank, return an apology
        if not symbol:
            return apology("must provide symbol", 400)
        # If symbol does not exist, return an apology
        quote = lookup(symbol.upper())
        if quote == None:
            return apology("symbol does not exist", 400)
        # If shares is not a positive integer, return an apology
        if shares < 0:
            return apology("Number of shares not valid", 400)

        # Add stock to portfolio
        price = shares * quote["price"]
        user_id = session["user_id"]
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        # If the user cannot afford the shares, return an apology
        if user_cash < price:
            return apology("not enough money", 400)

        # Update user's cash after purchase
        updated_cash = user_cash - price
        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, user_id)

        # Current datetime of purchase
        date = datetime.datetime.now()

        # Add info about purchase to transactions table
        db.execute("INSERT INTO transactions(user_id, symbol, shares, price, date, name) VALUES (?, ?, ?, ?, ?, ?)",
                   user_id, quote["symbol"], shares, quote["price"], date, quote["name"])

        flash("Bought")

        # Redirect user to homepage
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", user_id)

    return render_template("history.html", data=transactions)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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

    if request.method == "POST":
        symbol = request.form.get("symbol")

        # If field is left blank, return an apology
        if not symbol:
            return apology("must provide symbol", 400)

        # Get stock quote
        quote = lookup(symbol.upper())
        # Check if lookup function is successful
        if quote == None:
            return apology("symbol does not exist", 400)

        return render_template("quoted.html", name=quote["name"], price=quote["price"], symbol=quote["symbol"])
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check for errors
        # If any field is left blank, return an apology
        if not username:
            return apology("must provide username", 400)
        if not password:
            return apology("must provide password", 400)
        if not confirmation:
            return apology("must confirm password", 400)
        # If password and confirmation don't match return an apology
        if password != confirmation:
            return apology("passwords do not match", 400)

        # Generate a hash of the password
        password_hash = generate_password_hash(password)

        # Add user to database (if the username is already taken, return an apology)
        try:
            user = db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, password_hash)
        except:
            return apology("username already taken", 400)

        # Log user in
        session["user_id"] = user
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        # If field is left blank, return an apology
        if not symbol:
            return apology("must provide symbol", 400)
        # If symbol does not exist, return an apology
        quote = lookup(symbol.upper())
        if quote == None:
            return apology("symbol does not exist", 400)
        # If shares is not a positive integer, return an apology
        if shares < 0:
            return apology("number of shares not valid", 400)

        # Remove stock to portfolio
        price = shares * quote["price"]
        user_id = session["user_id"]

        # If user does not own that many shares of the stock, return an aapology
        user_shares = db.execute("SELECT SUM(shares) AS shares FROM transactions WHERE user_id = ? AND symbol = ?", user_id, symbol)[
            0]["shares"]

        if shares > user_shares:
            return apology("not enough shares", 400)

        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        # Update user's cash after sale
        updated_cash = user_cash + price
        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, user_id)

        # Current datetime of purchase
        date = datetime.datetime.now()

        # Add info about purchase to transactions table
        db.execute("INSERT INTO transactions(user_id, symbol, shares, price, date, name) VALUES (?, ?, ?, ?, ?, ?)",
                   user_id, quote["symbol"], (-1)*shares, quote["price"], date, quote["name"])

        flash("Sold")

        # Redirect the user to home page
        return redirect("/")
    else:
        user_id = session["user_id"]
        # Get symbols of owned shares
        symbols = db.execute("SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", user_id)

        return render_template("sell.html", symbols=[row["symbol"] for row in symbols])