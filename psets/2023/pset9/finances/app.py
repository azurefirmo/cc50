import os
from flask import Flask, flash, jsonify, redirect, jsonify, render_template, request, session
from datetime import datetime
from cs50 import SQL
import flask
from flask_session import Session
from tempfile import mkdtemp
import requests
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

app = Flask(__name__)

app.config["TEMPLATES_AUTO_RELOAD"] = True

app.jinja_env.filters["usd"] = usd

app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

db = SQL("sqlite:///finance.db")

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
    if request.method == "GET":
        user_id = session['user_id']

        cash_place = db.execute(
            "SELECT cash FROM users WHERE id = ?", session['user_id'])
        cash_place = cash_place[0]['cash']
        cash_place = f"${cash_place:,.2f}".format(cash_place)

        result = db.execute(
            "SELECT * FROM trans WHERE user_id = ?", session['user_id'])

        for i, v in enumerate(result):

            price_temp = v['price']
            new_price_temp = f"${price_temp:,.2f}".format(price_temp)
            result[i]['price'] = new_price_temp
            total_temp = v['total']

            new_total = f"${total_temp:,.2f}".format(total_temp)
            v['total'] = new_total

        CASH_TOTAL_PLACE = "$10.000.00"

        return render_template("index.html", value=result, total=CASH_TOTAL_PLACE, cash_place=cash_place)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "GET":
        return render_template("buy.html")

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("Missing symbol")
        if not shares:
            return apology("Missing shares")

        try:
            shares = int(shares)
        except ValueError:
            return apology("Server Error", code=500)

        if shares < 0:
            return apology("Negative Number")

        symbol_call = lookup(symbol.upper())

        if symbol_call == None or not symbol_call:
            return apology("Invalid symbol")
        else:
            price_symbol = symbol_call['price'] * shares

            user_money = db.execute("""
            SELECT cash from users WHERE id = ?""", session['user_id'])

            user_money = user_money[0]['cash']

            if user_money > price_symbol:

                update_cash = user_money - price_symbol

                db.execute("UPDATE users SET cash = ? WHERE id = ?",
                           update_cash, session['user_id'])

                time = datetime.now()
                temp = db.execute(
                    "SELECT * FROM trans WHERE user_id = ? and symbol = ?", session['user_id'], symbol)

                if not temp or len(temp) == 0:
                    db.execute("""
                    INSERT INTO trans (user_id,symbol,name,shares,price,total,time)
                    VALUES (?, ?, ?, ?, ?, ?, ?)""",
                               session['user_id'], symbol_call['symbol'],
                               symbol_call['name'], shares, symbol_call['price'], price_symbol, time)
                    flash("bought! ")
                    return redirect("/")
                else:
                    now_shares_user = db.execute("""
                    SELECT SUM(shares) AS sum FROM trans WHERE user_id = ? AND symbol = ?
                    """, session['user_id'], symbol)

                    now_shares_user = now_shares_user[0]['sum']
                    now_shares_user += shares

                    db.execute("""
                    UPDATE trans SET shares = ? WHERE user_id = ? AND symbol = ?
                    """, now_shares_user, session['user_id'], symbol)
                    flash("Bought")

                    db.execute("""
                    INSERT INTO history (user_id,symbol,shares,price,time)
                    VALUES(? ,? ,? ,? ,?)
                    """, session['user_id'], symbol_call['symbol'],
                               shares, symbol_call['price'], datetime.now())
                    return redirect('/')

            else:
                return apology("Not enough money")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    value = db.execute(
        "SELECT * FROM history WHERE user_id = ?", session['user_id'])
    for i, v in enumerate(value):
        value[i]['price'] = f"${value[i]['price']                                :,.2f}".format(value[i]['price'])

    return render_template("history.html", value=value)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    session.clear()

    if request.method == "POST":

        if not request.form.get("username"):
            return apology("must provide username", 403)

        elif not request.form.get("password"):
            return apology("must provide password", 403)

        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        session["user_id"] = rows[0]["id"]

        return redirect("/")

    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    session.clear()

    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "GET":
        return render_template("quote.html")

    if request.method == "POST":
        symbol = request.form.get("quote")

        if not symbol:
            return apology("MIssing symbol", code=400)

        res = lookup(symbol)

        if res == None:
            return apology("MIssing symbol", code=400)

        else:
            res['price'] = f"{res['price']:.2f}"
            return render_template("/quoted.html", value=res)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    if request.method == "POST":
        username = request.form.get('username')
        password = request.form.get('password')
        password_re = request.form.get('confirmation')

    if not username:
        return apology("Missing Username")
    if not password or not password_re:
        return apology("missing Password")
    if password != password_re:
        return apology("Passwords are not match")

    if res or len(res) != 0:
        return apology("Username is duplicate")

    res = db.execute("""
    INSERT INTO users (username,hash) VALUES (?, ?)
    """, username, generate_password_hash(password))

    flash("register is complete")

    return redirect('/login')


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "GET":

        db_user_shares = db.execute("""SELECT symbol,SUM(shares) AS shares FROM trans WHERE
         user_id = ? """, session["user_id"])

        symbol = []
        user_shares = {}

        for i, v in enumerate(db_user_shares):
            temp = v['symbol']
            if temp not in symbol:
                symbol.append(temp)

            if temp in user_shares:
                user_shares[temp] += v['shares']
            else:
                user_shares[temp] = v['shares']
        return render_template("sell.html", value=symbol)

    if request.method == "POST":

        symbol_input = request.form.get("symbol")
        shares_input = request.form.get("shares")

        if not symbol_input:
            return apology("symbol is empty")
        if not shares_input:
            return apology("shares is empty")

        temp = lookup(symbol_input)
        if not temp or temp == None:
            return apology("Invalid symbol :(")

        try:
            shares_input = int(shares_input)
        except ValueError:
            return apology("Share must be Number")

        if shares_input < 0:
            return apology("Share must be positive number")

        db_user_shares = db.execute("""
        SELECT SUM(shares) AS sum FROM trans WHERE symbol = ? AND user_id =?
         """, symbol_input, session['user_id'])
        db_user_shares = db_user_shares[0]['sum']

        if not db_user_shares:
            return apology("Invalid symbol")

        if shares_input > db_user_shares:
            return apology("Not enough shares")
        else:
            lookup_now = lookup(symbol_input)

            if not lookup_now:
                return apology("invalid symbol")

            cr_price_now = lookup_now['price']

            have_update_db = cr_price_now * shares_input
            cash_in_db = db.execute("""
            SELECT cash FROM users WHERE id = ?
            """, session['user_id'])
            cash_in_db = cash_in_db[0]['cash']

            cash_in_db += have_update_db

            db.execute("""
            update users SET cash = ? WHERE id = ?
            """, cash_in_db, session['user_id'])

            all_shares = db.execute(""" SELECT SUM(shares) as sum FROM trans WHERE user_id = ? AND symbol = ?
             """, session['user_id'], symbol_input)

            db.execute("DELETE FROM trans WHERE user_id = ? and symbol = ?",
                       session['user_id'], symbol_input)

            NOW_TIME = datetime.now()
            db_user_shares -= shares_input

            db.execute("""INSERT INTO trans
            (user_id,symbol,name,shares,price,total,time)
            VALUES (?,?,?,?,?,?,?)""",
                       session['user_id'], lookup_now['symbol'], lookup_now['name'],
                       db_user_shares, lookup_now['price'],
                       lookup_now['price']*db_user_shares, NOW_TIME)

            db.execute("DELETE FROM trans WHERE shares = ?", 0)

            his_share = "-" + str(shares_input)
            his_share = int(his_share)

            db.execute("""
            INSERT INTO history (user_id,symbol,shares,price,time)
            VALUES(? ,? ,? ,? ,?)
            """, session['user_id'], lookup_now['symbol'],
                       his_share, lookup_now['price'], datetime.now())
            flash("sold")
            return redirect("/")
