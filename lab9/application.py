import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # TODO: Add the user's entry into the database
        # step 5, get form's data
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")
        # step 6, validate data
        message = "The data is not complete."
        if not name or not month or not day:
            return render_template("index.html", message=message)
        
        # step 6, add data to db
        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?,?,?)", name, month, day)

        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html
        # step 1, query the db
        birthdays =  db.execute("SELECT * FROM birthdays")
        
        # step 2, send the query to index.html
        
        return render_template("index.html", birthdays=birthdays)


