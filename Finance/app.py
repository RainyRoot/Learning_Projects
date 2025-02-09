import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


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
    stocksymbol = db.execute("SELECT stocksymbol FROM portfolio WHERE userid = ?", user_id)
    amount = db.execute("SELECT amount FROM portfolio WHERE userid = ?", user_id)
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = round(cash[0]["cash"])
    stockvalue = []
    for symbol_data in stocksymbol:
        symbol = symbol_data['stocksymbol']
        data = lookup(symbol)
        if data:
            stockvalue.append(data)
    stock_value_total = 0
    for i in range(len(stocksymbol)):
        stock_value_tmp = stockvalue[i]["price"] * amount[i]["amount"]
        stock_value_total = stock_value_total + stock_value_tmp
        stock_value_total = round(stock_value_total)
    cash_stock_value_total = stock_value_total + cash
    cash_stock_value_total = round(cash_stock_value_total)

    return render_template("index.html", stocksymbol=stocksymbol, amount=amount, cash=cash, stockvalue=stockvalue, cash_stock_value_total=cash_stock_value_total)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        stocksymbol = request.form.get("stocksymbol")
        amount = int(request.form.get("amount"))
        if amount <= 0:
            return apology("Please enter a positive amount")
        buy_data = lookup(stocksymbol)
        if not buy_data:
            return apology("Please enter a valid stock symbol")
        user_id = session["user_id"]
        user_cash_row = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        if not user_cash_row:
            return apology("User not found")
        user_cash = float(user_cash_row[0]["cash"])
        total_cost = buy_data["price"] * amount
        if total_cost > user_cash:
            return apology("You don't have enough cash")
        new_cash = user_cash - total_cost
        existing_portfolio_entry = db.execute("SELECT * FROM portfolio WHERE userid = ? AND stocksymbol = ?", user_id, stocksymbol)
        if existing_portfolio_entry:
            new_amount = existing_portfolio_entry[0]["amount"] + amount
            db.execute("UPDATE portfolio SET amount = ? WHERE userid = ? AND stocksymbol = ?", new_amount, user_id, stocksymbol)
        else:
            db.execute("INSERT INTO portfolio (userid, stocksymbol, amount) VALUES (?, ?, ?)", user_id, stocksymbol, amount)

        #updates user's cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user_id)

        return render_template("index.html")
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ? ORDER BY timestamp DESC", user_id)
    return render_template("history.html", transactions=transactions)

    return apology("TODO")


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
        stocksymbol = request.form.get("stocksymbol")
        try:
            quote_data = lookup(stocksymbol)
            if quote_data:
                return render_template("quote.html", quote_data=quote_data)
        except:
            return apology("please enter a valid stock symbol")
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        verify_password = request.form.get("verify_password")
        if username == "" or password == "":
            return apology("please enter a username and a password")
        if password != verify_password:
            return apology("your password does not match")
        hash = generate_password_hash(password)
        if db.execute("SELECT COUNT(*) FROM users WHERE username = ?", username)[0]["COUNT(*)"] != 0:
            return apology("username already in use")
        else:
            db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hash)
            return login()
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    stocksymboldb = [row['stocksymbol'] for row in db.execute("SELECT stocksymbol FROM portfolio WHERE userid = ?", user_id)]
    if request.method == "POST":
        shares = request.form.get("shares")
        stocksymbol = request.form.get("stocksymbol")
        if stocksymbol in stocksymboldb:
            amount_row = db.execute("SELECT amount FROM portfolio WHERE userid = ? AND stocksymbol = ?", user_id, stocksymbol)
            if amount_row:
                amount = amount_row[0]['amount']
                if amount is not None and int(amount) >= int(shares):
                    new_shares = amount - int(shares)
                    if new_shares == 0:
                        deleteid = db.execute("SELECT id FROM portfolio WHERE userid = ? AND stocksymbol = ?", user_id, stocksymbol)[0]['id']
                        db.execute("DELETE FROM portfolio WHERE id = ?", deleteid)
                    else:
                        db.execute("UPDATE portfolio SET amount = ? WHERE userid = ? AND stocksymbol = ?", new_shares, user_id, stocksymbol)
                        # Calculate total value of sold stocks
                        sell_data = lookup(stocksymbol)
                        total_value = sell_data["price"] * int(shares)
                        # Update user's cash
                        user_cash_row = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
                        user_cash = float(user_cash_row[0]["cash"])
                        new_cash = user_cash + total_value
                        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user_id)
                    return redirect("/")
                else:
                    return apology("You don't own that many shares")
            else:
                return apology("No shares found for this stock symbol")
        else:
            return apology("Please select a valid stock symbol")
    else:
        return render_template("sell.html", stocksymboldb=stocksymboldb)
