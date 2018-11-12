from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp
from datetime import date, datetime
from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/changepass", methods=["GET", "POST"])
@login_required
def changepass():
    query = "SELECT username, hash FROM users WHERE id=:id"
    rows = db.execute(query, id = int(session["user_id"]))
    if request.method == "POST":
        pw = request.form.get("password")
        newpw = request.form.get("newpass")
        conf = request.form.get("confirm")
        if (newpw != conf):
            return apology("password confirmation error")
        elif not pwd_context.verify(pw, rows[0]["hash"]):
            return apology("wrong password")
        else:
            query2 = "UPDATE users SET hash = :hash WHERE id=:id" 
            rows2 = db.execute(query2, hash = pwd_context.encrypt(newpw), id = int(session["user_id"]))
            return redirect(url_for("logout"))
    else:
        return render_template("changepass.html", username = rows[0]["username"])



@app.route("/")
@login_required
def index():
    query1 = "SELECT symbol, name, sum(shares) AS shares, unitprice, sum(unitprice * shares) AS total FROM history GROUP BY symbol HAVING id=:id"
    rows = db.execute(query1, id = int(session["user_id"]))
    query2 = "SELECT username, cash FROM users WHERE id=:id"
    rows2 = db.execute(query2, id = int(session["user_id"]))
    return render_template("index.html", transactions = rows, cash = rows2[0]["cash"], username =rows2[0]["username"])
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))
        # check whether the symbol is correct
        # lookup returns NULL if symbol is not found or interet connection problem
        if (stock != None):
            shares = int(request.form.get("shares"))
            if (shares > 0):
                # how much does the user have in bank?
                row = db.execute("SELECT cash FROM users WHERE id = :id", id = int(session["user_id"]))
                current = row[0]["cash"]
                cost = int(shares) * stock["price"]
                print("current balance of the user: ",current)
                print("stock price: " , stock["price"])
                print("shares: ", shares)
                # can the user afford ?
                if(cost <= current):
                    # subract the cost incurred from the user's account
                    db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = current - cost, id = int(session["user_id"]))
                    # insert the transaction to purchase table
                    # id will be used as a foreign key
                    db.execute("INSERT INTO history(id, symbol, uniTprice, shares, type, pdate, name) VALUES(:id, :symbol, :uprice, :shares, 1,:pdate, :name)", id = int(session["user_id"]), symbol = stock['symbol'], uprice = stock['price'], shares = shares, pdate = str(date.today()), name = stock['name'])
                    # redirect function goes to the history function and it will load it
                    # so we will get updated list of transactions
                    # render_template will generate the history by going to the html file
                    # in that case we will get empty table because the select query on 
                    # transactions table is available in history function
                    return redirect(url_for("history"))
                else:
                    return apology("poor guy; deposit money to your account")
            else:
                return apology("shares should be a positive integer")

        else:
            return apology("symbol doesn't exist or internet connection is broken")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    rows = db.execute("SELECT * FROM history WHERE id = :id", id = int(session["user_id"]))
    return render_template("history.html", transactions = rows)
@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    # assuming that we create sessions only for user ids
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        quoted_dict = lookup(request.form.get("symbol"))
        if (quoted_dict != None):
            return render_template("quoted.html", stock = quoted_dict)
        else:
            return apology("symbol doesn't exist")
    else:
        return render_template("quote.html")

        """Get stock quote."""
    return apology("TODO")

@app.route("/quoted", methods=["GET", "POST"])
@login_required
def quoted():
    return render_template("quoted.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        print(len(rows))
        if request.form["username"] == "" or len(rows) > 0 :
            return apology("blank user name or user name exists")
        elif request.form["password"] == "" or (request.form["password"] != request.form["confirm"]):
            return apology("blank password or confirmation error")
        else:
            db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=request.form["username"], hash=pwd_context.encrypt(request.form["password"]))
            return redirect(url_for("index"))
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    query1 = "SELECT symbol, name, sum(shares) AS shares FROM history GROUP BY symbol HAVING id = :id"
    rows = db.execute(query1, id = int(session["user_id"]))
    if request.method == "POST":
        for x in rows:
            # this will give us the remaining share of a stock
            newshare = int(x["shares"]) - int(request.form.get(x["symbol"])) 
            # get the current price of the stock
            # put a new row for the sales
            # only if it is non-zero
            if (int(request.form.get(x["symbol"])) > 0):
                # get the current price
                stock = lookup(x["symbol"])
                query2 = "INSERT INTO history(id, symbol, uniTprice, shares, type, pdate, name) VALUES(:id, :symbol, :uprice, :shares, 0,:pdate, :name)"
                db.execute(query2, id = int(session["user_id"]), symbol = x['symbol'], uprice = stock["price"], shares = -(x["shares"]), pdate = str(date.today()), name = x["name"] )

                # how much the user has
                result = db.execute("SELECT cash FROM users WHERE id = :id", id = int(session["user_id"]))
                current = result[0]["cash"]
                # gain as a result of selling some stocks
                gain = int(x["shares"]) * stock["price"]
                # update the balance of the user
                newbalance = current + gain
                db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = newbalance, id = int(session["user_id"]))
                return redirect(url_for("history"))

    else:
        return render_template("sell.html", symbols = rows)
