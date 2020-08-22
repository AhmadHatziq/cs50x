from flask import Flask, render_template, request


app = Flask(__name__)

@app.route("/") # Routes are what we might type at the end of the URL eg "/maps", "/settings"
                # Single slash "/" is the default page
def index():
    return render_template("index.html")

@app.route("/hello")
def bye():
    name = request.args.get("name")

    # Handle the case where name is non-existing
    if not name:
        return render_template("failure.html")

    return render_template("hello.html", name = name)