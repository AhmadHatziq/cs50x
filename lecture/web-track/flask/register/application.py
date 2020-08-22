from cs50 import SQL
from flask import Flask, render_template, request, redirect

app = Flask(__name__)

db = SQL("sqlite:///lecture.db")

@app.route("/")
def index():
    rows = db.execute("SELECT * FROM registrants")
    return render_template("index.html", rows = rows)

@app.route("/register", methods = ["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")
    elif request.method == "POST":
        name = request.form.get("name")
        email = request.form.get("email")

        # Check if a name is not provided
        if not name:
            return render_template("apology.html", message = "You must provide a name.")

        # Check if email is not provided
        if not email:
            return render_template("apology.html", message = "You must provide an email address.")
        db.execute("INSERT INTO registrants (name, email) VALUES (:name, :email)", name = name, email = email)
        return redirect("/") # Redirect back to main page, which shows overall rows
