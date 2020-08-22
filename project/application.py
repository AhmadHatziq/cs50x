import os

from cs50 import SQL
from datetime import datetime
from flask import Flask, flash, jsonify, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, get_semester

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

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///courses.db")

@app.route("/")
@login_required
def index():
    """Show overview of all courses taken"""

    # Extract all courses relating to the user
    rows = db.execute("SELECT * FROM courses WHERE user_id = :user_id ORDER BY date_completed ASC", user_id = session['user_id'])

    # Parse the rows
    parsed_rows = []
    index_counter = 0
    rows_by_date = {}
    for row in rows:

        # Compile a dict for each row
        values = {}
        index_counter = index_counter + 1
        values['index'] = index_counter
        values['name'] = row['course_name']
        values['provider'] = row['course_provider']
        values['grade'] = row['course_grade']
        values['link'] = row['course_certificate_link']

        current_date = get_semester(row['date_completed'])
        values['date'] = get_semester(row['date_completed'])
        values['actual_date'] = row['date_completed']

        # Append to the overall dict
        if current_date not in rows_by_date.keys():
            temp_list = []
            temp_list.append(values)
            rows_by_date[current_date] = temp_list
        else:
            current_list = rows_by_date[current_date]
            current_list.append(values)
            rows_by_date[current_date] = current_list

    if ("updated_recently" in request.args) and (request.args['updated_recently'] == 'True'):
        flash("Your courses have been updated")
        return render_template("index.html", rows_by_date = rows_by_date)
    else:
        return render_template("index.html", rows_by_date = rows_by_date)

@app.route("/add", methods = ["GET", "POST"])
@login_required
def add():
    """Adds courses into the database"""
    if request.method == "POST":

        # Checking for existence of fields is done via JavaScript

        # Extract all form arguments
        course_name = request.form.get("course_name").strip()
        course_completion_date = request.form.get("course_completion_date")
        course_grade = request.form.get("course_grade")
        course_provider = request.form.get("course_provider")
        course_link = request.form.get("course_link")
        course_description = request.form.get("course_description").strip()

        # Check if there is a course with the same name inside the DB
        rows = db.execute("SELECT course_name FROM courses WHERE user_id = :user_id AND course_name = :course_name",
                           user_id = session['user_id'], course_name = course_name)
        if (len(rows) != 0):
            return apology("Course already exists")


        # Store data to DB
        rows = db.execute("INSERT INTO courses (course_name, date_completed, course_provider, course_description, course_certificate_link, \
                            course_grade, user_id) VALUES (:name, :date, :provider, :desc, :link, :grade, :user_id)", name = course_name, date = course_completion_date,
                            provider = course_provider, desc = course_description, link = course_link, grade = course_grade, user_id = session['user_id'])

        # NTS: Redirect to index page, where will show all courses in DB

        return redirect("/")
    elif request.method == "GET":
        return render_template("add_course.html")

@app.route("/update", methods = ["POST"])
@login_required
def update():
    if request.method == "POST":

        # Extract all form arguments
        course_name = request.form.get("course_name").strip()
        course_completion_date = request.form.get("course_completion_date")
        course_grade = request.form.get("course_grade")
        course_provider = request.form.get("course_provider")
        course_link = request.form.get("course_link")
        course_description = request.form.get("course_description").strip()
        course_id = request.form.get("course_id")

        # Update the database
        rows = db.execute("UPDATE courses SET course_name = :course_name, date_completed = :date, course_provider = :provider,\
                           course_description = :desc, course_certificate_link = :link, course_grade = :grade WHERE\
                           course_id = :course_id AND user_id = :user_id", course_name = course_name, date = course_completion_date,
                           provider = course_provider, desc = course_description, link = course_link, grade = course_grade,
                           course_id = course_id, user_id = session['user_id'])

        return redirect(url_for('index', updated_recently = 'True'))

@app.route("/delete", methods = ["GET", "POST"])
@login_required
def delete():
    if request.method == "POST":

        # Check if any radio option is selected
        course_to_delete = request.form.get("course_chosen")
        if not course_to_delete:
            return apology("Please select a course to delete")

        # Procees with deletion
        rows = db.execute("DELETE FROM courses WHERE user_id = :user_id AND course_name = :course_name",
                           user_id = session['user_id'], course_name = course_to_delete)

        return redirect(url_for('index', updated_recently = 'True'))

    elif request.method == "GET":

        # Extract all courses relating to the user
        rows = db.execute("SELECT * FROM courses WHERE user_id = :user_id ORDER BY date_completed ASC", user_id = session['user_id'])

        # Parse the rows
        parsed_rows = []
        index_counter = 0
        rows_by_date = {}
        for row in rows:

            # Compile a dict for each row
            values = {}
            index_counter = index_counter + 1
            values['index'] = index_counter
            values['name'] = row['course_name']
            values['provider'] = row['course_provider']
            values['grade'] = row['course_grade']
            values['link'] = row['course_certificate_link']

            current_date = get_semester(row['date_completed'])
            values['date'] = get_semester(row['date_completed'])
            values['actual_date'] = row['date_completed']

            # Append to the overall dict
            if current_date not in rows_by_date.keys():
                temp_list = []
                temp_list.append(values)
                rows_by_date[current_date] = temp_list
            else:
                current_list = rows_by_date[current_date]
                current_list.append(values)
                rows_by_date[current_date] = current_list
        return render_template("delete.html", rows_by_date = rows_by_date)


@app.route("/edit", methods = ["GET", "POST"])
@login_required
def edit():
    if request.method == "POST":

        # Check if an option was selected
        if not request.form.get("course_chosen"):
            return apology("no option was selected")

        # Get course info
        course_chosen = request.form.get("course_chosen")
        row = db.execute("SELECT * FROM courses WHERE user_id = :user_id AND course_name = :course_name",
                           user_id = session['user_id'], course_name = course_chosen)

        # Store course data in a dictionary
        course_data = {}
        course_data['course_id'] = row[0]['course_id']
        course_data['course_name'] = row[0]['course_name']
        course_data['course_date'] = row[0]['date_completed']
        course_data['course_provider'] = row[0]['course_provider']
        course_data['course_description'] = row[0]['course_description']
        course_data['course_certificate_link'] = row[0]['course_certificate_link']
        course_data['course_grade'] = row[0]['course_grade']

        return render_template("edit_course.html", course_data = course_data)

    elif request.method == "GET":

        # Extract all courses relating to the user
        rows = db.execute("SELECT * FROM courses WHERE user_id = :user_id ORDER BY date_completed ASC", user_id = session['user_id'])

        # Parse the rows
        parsed_rows = []
        index_counter = 0
        rows_by_date = {}
        for row in rows:

            # Compile a dict for each row
            values = {}
            index_counter = index_counter + 1
            values['index'] = index_counter
            values['name'] = row['course_name']
            values['provider'] = row['course_provider']
            values['grade'] = row['course_grade']
            values['link'] = row['course_certificate_link']

            current_date = get_semester(row['date_completed'])
            values['date'] = get_semester(row['date_completed'])
            values['actual_date'] = row['date_completed']

            # Append to the overall dict
            if current_date not in rows_by_date.keys():
                temp_list = []
                temp_list.append(values)
                rows_by_date[current_date] = temp_list
            else:
                current_list = rows_by_date[current_date]
                current_list.append(values)
                rows_by_date[current_date] = current_list

        return render_template("edit.html", rows_by_date = rows_by_date)

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


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)

# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
