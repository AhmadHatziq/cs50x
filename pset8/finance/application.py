import os

from cs50 import SQL
from datetime import datetime
from flask import Flask, flash, jsonify, redirect, render_template, request, session, url_for
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

# Store API Key here for easy reference
API_KEY = "pk_7ae03be62b7949e7b647035f023e09a8"

# Register the API KEY as an environ var
os.environ['API_KEY'] = API_KEY

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks inside user_stocks table"""

    # Extract user stocks
    stock_rows = db.execute("SELECT stock_symbol, stock_name, share_amount FROM user_stocks\
                       WHERE user_id = :user_id", user_id = session['user_id'])

    # Process each row
    stock_rows_parsed = []
    counter = 0
    for row in stock_rows:
        symbol = row['stock_symbol']
        name = row['stock_name']
        shares = row['share_amount']
        price = lookup(symbol)['price']
        total = price * shares

        single_row = {}
        single_row['symbol'] = symbol
        single_row['name'] = name
        single_row['price'] = usd(price)
        single_row['total'] = usd(total)
        single_row['shares'] = shares

        stock_rows_parsed.append(single_row)
        counter = counter + 1

    # Extract user cash
    rows = db.execute("SELECT cash from users WHERE id = :user_id", user_id = session['user_id'])
    user_cash = rows[0]['cash']

    return render_template("index.html", user_cash = usd(user_cash), stock_rows = stock_rows_parsed)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        # Check if any fields are empty
        if not request.form.get("symbol"):
            return apology("please input stock symbol")
        if not request.form.get("shares"):
            return apology("please input share(s) amount")

        # Run the lookup with the supplied stock symbol
        lookup_values = lookup(request.form.get("symbol").strip())

        # Check if symbol is valid
        if not lookup_values:
            return apology("invalid symbol")

        # Check if the number of shares to buy is not negative
        if int(request.form.get("shares")) < 0:
            return apology("num of shares should not be negative")

        # Check if the user has enough money
        rows = db.execute("SELECT cash FROM users WHERE id = :user_id",
                          user_id = session["user_id"])

        current_cash = rows[0]['cash']
        total_price = float(request.form.get("shares")) * lookup_values['price']

        if (current_cash < total_price):
            return apology("Unable to buy " + str(request.form.get("shares")) + ' shares of ' + lookup_values['symbol'] + ' as user does not have enough cash')

        '''Procees the transaction'''

        # Update user table
        remaining_amount = (current_cash - total_price)
        rows = db.execute("UPDATE users SET cash = :new_cash_value \
                           WHERE id = :user_id", new_cash_value = remaining_amount, user_id = session["user_id"])

        # Update user_stocks table
        rows = db.execute("SELECT * FROM user_stocks WHERE user_id = :user_id AND stock_symbol = :stock_symbol",
                           user_id = session["user_id"], stock_symbol = lookup_values["symbol"])

        if (len(rows) == 1):
            # entry exists, only need to increment the stock amount
            # Get old stock amount
            rows = db.execute("SELECT share_amount, user_stocks_id FROM user_stocks WHERE user_id = :user_id \
                               AND stock_symbol = :stock_symbol",
                               user_id = session["user_id"], stock_symbol = lookup_values["symbol"])
            current_shares = rows[0]['share_amount']
            new_amount = current_shares + int(request.form.get("shares"))
            user_stock_id = rows[0]['user_stocks_id']

            # Update the row
            rows = db.execute("UPDATE user_stocks SET share_amount = :new_amount \
                               WHERE user_stocks_id = :user_stock_id", new_amount = new_amount,
                               user_stock_id = user_stock_id)

        else:

            # Submit a new row
            rows = db.execute("INSERT INTO user_stocks (user_id, stock_symbol, stock_name, share_amount) \
                               VALUES (:user_id, :stock_symbol, :stock_name, :share_amount)",
                               user_id = session["user_id"], stock_symbol = lookup_values["symbol"], stock_name = lookup_values["name"],
                               share_amount = int(request.form.get("shares")))

        # Update transaction_history table
        dateTimeObj = datetime.now()
        timestampStr = dateTimeObj.strftime("%Y-%m-%d %H:%M:%S")
        rows = db.execute("INSERT INTO transaction_history(user_id, transaction_type, transaction_price, transaction_amount, \
                           transaction_timestamp, stock_symbol) VALUES (:user_id, :transaction_type, :price, :amount, :timestamp, :stock_symbol)",
                           user_id = session["user_id"], transaction_type = 'BUY', price = lookup_values['price'],
                           amount = int(request.form.get("shares")), timestamp = timestampStr, stock_symbol = lookup_values['symbol'])



        return redirect("/")

    elif request.method == "GET":

        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query transaction table
    rows = db.execute("SELECT * FROM transaction_history WHERE user_id = :user_id",
                       user_id = session['user_id'])

    row_list = []
    for row in rows:
        single_row = {}
        single_row['symbol'] = row['stock_symbol']
        single_row['shares'] = row['transaction_amount']
        single_row['price'] = usd(float(row['transaction_price']))
        single_row['transacted'] = row['transaction_timestamp']
        row_list.append(single_row)

    return render_template("history.html", transaction_rows = row_list)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username").strip())

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:

        # register route will supply argument if a new account is just created
        if ("newly_created_account" in request.args) and (request.args['newly_created_account'] == 'True'):
            flash("Please login with your newly created account")
            return render_template("login.html")
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

        # Check if the symbol is present
        if not request.form.get("symbol"):
            return apology("must provide the stock symbol for quote to be processed")

        # Process the lookup
        lookup_values = lookup(request.form.get("symbol"))

        # Check if the symbol and lookup is valid
        if not lookup_values:
            return apology("invalid symbol")

        return render_template("quoted.html", lookup_values = lookup_values)

    elif request.method == "GET":
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure both passwords were submitted
        if not request.form.get("password_01") or not request.form.get("password_02"):
            return apology("must provide passwords", 400)

        # Ensure both passwords are the same
        if request.form.get("password_01") != request.form.get("password_02"):
            return apology("must provide matching passwords", 400)

        # Check if the username already exists in the database
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username").strip())
        if len(rows) != 0:
            return apology("username already exists. please choose another username", 400)

        # NTS: Add check for password complexity via JS

        # All checks passed. Proceed to add entry to users table
        rows = db.execute("INSERT INTO users (username, hash) \
                            VALUES (:username, :password_hash)",
                            username = request.form.get("username").strip(),
                            password_hash = generate_password_hash(request.form.get("password_01"), "sha256"))

        # Redirect back to login page with addiitonal parameter inside args dict
        return redirect(url_for('login', newly_created_account = 'True'))

    # User reached route via GET (as by clicking a link or via redirect)
    elif request.method == "GET":
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        # POST request: process form data from user

        # Check if no symbol is selected
        if not request.form.get("symbol"):
            return apology("Please select a stock to sell")

        # Note that JavaScript handles the case when the user did not select a valid option.
        # If the user does not select a valid option, the button will remain disabled

        # Check if the user has the stock that is selected
        rows = db.execute("SELECT stock_symbol FROM user_stocks WHERE user_id = :user_id",
                           user_id = session['user_id'])
        user_symbols = []
        for row in rows:
            user_symbols.append(row['stock_symbol'])
        if request.form.get("symbol") not in user_symbols:
            return apology("User does not own stock to be sold.")

        # Check if the number of shares to sell is present
        if not request.form.get("shares"):
            return apology("Please enter number of shares to sell.")

        # Check if the number of shares to sell is positive
        if int(request.form.get("shares")) < 0:
            return apology("Please enter a non-negative number of shares to sell.")

        # Check if the user has enough shares to sell
        rows = db.execute("SELECT share_amount FROM user_stocks WHERE user_id = :user_id AND stock_symbol = :stock_symbol",
                           user_id = session['user_id'], stock_symbol = request.form.get("symbol"))
        current_share_amount = rows[0]['share_amount']
        amount_to_sell = int(request.form.get("shares"))
        if (amount_to_sell > current_share_amount):
            return apology("User does not have enough shares to sell.")

        # Proceed with transaction

        # Update users table
        # Update user money with profits from selling shares
        selling_price = lookup(request.form.get("symbol"))['price']
        rows = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session['user_id'])
        current_money = rows[0]['cash']
        updated_money = current_money + selling_price * float(request.form.get("shares"))
        rows = db.execute("UPDATE users SET cash = :updated_money WHERE id = :user_id",
                           updated_money = updated_money, user_id = session['user_id'])

        # Update user_stocks table
        rows = db.execute("SELECT share_amount FROM user_stocks WHERE user_id = :user_id AND stock_symbol = :stock_symbol",
                           user_id = session['user_id'], stock_symbol = request.form.get("symbol"))
        current_share_amount = rows[0]['share_amount']
        updated_share_amount = current_share_amount - int(request.form.get("shares"))

        if updated_share_amount == 0:
            # 0 stocks left. Hence, need to delete, not update the row
            rows = db.execute("DELETE FROM user_stocks WHERE user_id = :user_id AND stock_symbol = :stock_symbol",
                               user_id = session['user_id'], stock_symbol = request.form.get("symbol"))
        elif updated_share_amount > 0:
            # More than 0 stocks. Update corresponding row
            rows = db.execute("UPDATE user_stocks SET share_amount = :new_share_amount WHERE user_id = :user_id AND stock_symbol = :stock_symbol",
                               new_share_amount = updated_share_amount, user_id = session['user_id'], stock_symbol = request.form.get("symbol"))

        # Update transaction_history table
        dateTimeObj = datetime.now()
        timestampStr = dateTimeObj.strftime("%Y-%m-%d %H:%M:%S")
        rows = db.execute("INSERT INTO transaction_history(user_id, transaction_type, transaction_price, transaction_amount, \
                           transaction_timestamp, stock_symbol) VALUES (:user_id, :transaction_type, :price, :amount, :timestamp, :stock_symbol)",
                           user_id = session["user_id"], transaction_type = 'SELL', price = selling_price,
                           amount = int(request.form.get("shares")) * -1, timestamp = timestampStr, stock_symbol = request.form.get("symbol"))


        return redirect('/')

    elif request.method == "GET":
        # GET request: send html to user

        # Query DB and see what stocks does the user have
        rows = db.execute("SELECT stock_symbol FROM user_stocks WHERE user_id = :user_id",
                           user_id = session['user_id'])

        # Process the rows into a list
        stock_symbols = []
        for row in rows:
            stock_symbols.append(row['stock_symbol'])

        return render_template("sell.html", stock_symbols = stock_symbols)



def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
