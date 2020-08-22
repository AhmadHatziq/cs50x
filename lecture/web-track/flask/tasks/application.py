from flask import Flask, render_template, request, redirect, session
from flask_session import Session

app = Flask(__name__)
app.config["SESSION_PERMANENT"] = False
app.config['SESSION_TYPE'] = "filesystem"
Session(app)

@app.route("/")
def tasks():
    if "todos" not in session:
            session["todos"] = []
    return render_template("tasks.html", todos = session['todos'])

@app.route("/add", methods = ["GET","POST"])
def add():

    # User is just trying to get this page
    if request.method == "GET":
        return render_template("add.html")
    elif request.method == "POST":
        todo = request.form.get("task") # The name of the input field is "task"
        session["todos"].append(todo)
        return redirect("/") # Redirect the user to root

