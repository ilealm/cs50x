import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    stocks = []
    
    # step 4,
    # get the username from the current session["user_id"]
    rows = db.execute("SELECT username, cash FROM users WHERE id = ?", session["user_id"])
    # Ensure username exists and password is correct
    if len(rows) != 1:
        return apology("invalid username and/or password", 400)
    
    username = rows[0]['username']  
    cash = rows[0]['cash']  
    
    # get the user's transactions where are buys, group by symbol
    # TODO, FIX THIS SELECT TO AFFECT THE SOLD SHARES
    transactions = db.execute(
        "SELECT stock, symbol , SUM(shares) as 'StockShares' FROM transactions WHERE username= ? AND operation='BUY' GROUP BY symbol", username)
    
    # for each transaction, get the info from API to get the current stock value and get the holding_value
    holding_sum = cash
    for i in range(len(transactions)):
        current_price = lookup(transactions[i]["symbol"])
        # print(current_price["price"])
     
        stocks.append({"stock": transactions[i]["stock"], "StockShares": transactions[i]["StockShares"], 
                       "current_price": current_price["price"], "holding_value": current_price["price"] * transactions[i]["StockShares"]})
        holding_sum += current_price["price"] * transactions[i]["StockShares"]
    
    # send the array to index.html
    # print(stocks)
    
    return render_template("index.html", stocks=stocks, holding_sum=holding_sum, cash=cash)  


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # step 3,
    if request.method == 'POST':
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")

        # validations
        if not symbol or not shares:
            return apology("The Stock and/or shares are required", 400)
            
        if not shares.isdigit():
            return apology("The shares must be a positive integer", 400)
        
        if int(shares) <= 0:
            return apology("The shares must be greater than zero", 400)
        
        # get the stock info, if exists
        stock = lookup(symbol)
        if not stock: 
            return apology("The stock does not exists.", 400)

        # get the transaction values
        symbol_name = stock['symbol'] + ' - ' + stock['name']
        price = stock['price']
        transaction_amount = float(shares) * price

        # get the user info
        user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        # Ensure username exists and password is correct
        if len(user) != 1:
            return apology("invalid username and/or password", 400)
        
        # print(symbol, price, shares, transaction_amount)
        
        # validate if the user has founds to do the transaction
        user_cash = user[0]['cash']
        if transaction_amount > user_cash:
            return apology("Insufficient funds", 400)
            
        # store the transaction
        db.execute("BEGIN TRANSACTION")
        
        # update user's cash
        new_cash = user_cash - transaction_amount
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])
        # add the new transacction
        db.execute("INSERT INTO transactions (username, stock, price, shares, operation, amount, symbol) VALUES (?, ?, ?, ?, ?, ?, ?)", 
                   user[0]['username'], symbol_name, price, shares, 'BUY', transaction_amount, symbol)
        
        # user tbl id, username, hash, cash
        db.execute("COMMIT")
        
        return redirect("/")
 
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # step 6
    user_info = get_userInfo()
    history = db.execute("SELECT * FROM transactions WHERE username=?", user_info["username"])
    
    boughts = db.execute("SELECT * FROM transactions WHERE operation='BUY' AND username=?", user_info["username"])
    sold = db.execute("SELECT * FROM transactions WHERE operation='SELL' AND username=?", user_info["username"])
    
    return render_template("history.html", history=history, boughts=len(boughts), sold=len(sold), num_operations=len(boughts) + len(sold))


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

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
    # step 2, build quote.html and handle here the methods
    if request.method == "POST":
        symbol = request.form.get("symbol")

        if not symbol:
            return apology("A stock is requiered", 400)
        
        stock = lookup(symbol)
        if not stock:
            return apology("The stock does not exist", 400)
            
        return render_template("quoted.html", stock=stock)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # step 1, build register.html and manage the post here
    if request.method == "POST":
        username = request.form.get("username")
        pwd = request.form.get("password")
        conf = request.form.get("confirmation")

        if not username:
            return apology("must provide username", 400)

        if not pwd:
            return apology("must provide a password", 400)

        if not conf:
            return apology("must provide a confirmation", 400)

        if not pwd == conf:
            return apology("password and confirmation must be equal", 400)

        # check if that username is already in the DB
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        if len(rows) == 1:
            return apology("User already exists.", 400)

        # insert the new user
        db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, generate_password_hash(pwd))
        
        # add the new user to the session 
        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("register.html")


def get_userInfo():
    rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

    # Ensure username exists and password is correct
    if len(rows) != 1:
        return apology("invalid username and/or password", 400)
    else:
        return rows[0]
    

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # step 5
    if request.method == 'POST':
        # get the form values
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
  
        # validate that the user entered shared and/or symbol
        if not shares or not symbol:
            return apology("select a symbol and shares", 400)
        
        if shares < 1:
            return apology("shares must be greater than zero", 400)
            
        # validate if the user can sell the selected amount of shares
        user_info = get_userInfo()
        bought_shares = db.execute(
            "SELECT SUM(shares) as 'bought' FROM transactions WHERE username= ? AND operation='BUY' AND symbol=?", user_info["username"], symbol)
        sold_shares = db.execute(
            "SELECT SUM(shares) as 'sold' FROM transactions WHERE username= ? AND operation='SELL' AND symbol=?", user_info["username"], symbol) 
        if sold_shares[0]["sold"] == None:
            available_shares = bought_shares[0]["bought"]
        else:
            available_shares = bought_shares[0]["bought"] - sold_shares[0]["sold"]
        
        if shares > available_shares:
            return apology("Not enouhg shares to sell", 400)
      
        # get the current stock value and calculate the gain
        current_stock = lookup(symbol)
        gain = shares * current_stock["price"]
        new_cash = user_info["cash"] + gain  
        # print(gain,user_info["cash"] , new_cash)
        
        # store the transaction
        db.execute("BEGIN TRANSACTION")
        
        # update user's cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])
        
        # log the transaction
        # get the transaction values
        symbol_name = current_stock['symbol'] + ' - ' + current_stock['name']
        price = current_stock['price']
        transaction_amount = float(shares) * price
        db.execute("INSERT INTO transactions (username, stock, price, shares, operation, amount, symbol) VALUES (?, ?, ?, ?, ?, ?, ?)", 
                   user_info["username"], symbol_name, price, shares, 'SELL', transaction_amount, symbol)
        
        db.execute("COMMIT")
        # redirect to index
        return redirect("/")
    else:
        user_info = get_userInfo()
        # get the user's symbols
        symbols = db.execute('SELECT DISTINCT(symbol) FROM transactions WHERE username= ? ORDER BY symbol', user_info["username"])
        
        return render_template("sell.html", symbols=symbols)
        

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
