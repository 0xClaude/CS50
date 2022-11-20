import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

@app.route("/delete", methods=["POST"])
def delete():
    if not request.form.get("id"):
        return "Missing id"
    id = request.form.get("id")

    db.execute("DELETE FROM birthdays WHERE id = (?)", id)
    return redirect("/")

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # Check for data integrity
        if not request.form.get("name"):
            return "Missing name"
        name = request.form.get("name")

        if not request.form.get("month"):
            return "Wrong month"
        month = request.form.get("month")

        if not request.form.get("day"):
            return "Wrong day"
        day = request.form.get("day")

        # Add the user's entry into the database
        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ? ,?)", name, month, day)

        return redirect("/")

    else:
        # Display the entries in the database on index.html
        birthdays = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", birthdays=birthdays)


