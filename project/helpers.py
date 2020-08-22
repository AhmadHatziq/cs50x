import os
import sys
import requests
import urllib.parse

from datetime import datetime
from flask import redirect, render_template, request, session
from functools import wraps

""" Returns which semester the date is from. Agument format is YYYY-MM-DD"""
def get_semester(string_date):

    # Convert to date_time object
    date_time_obj = datetime.strptime(string_date, '%Y-%m-%d')

    # Create date_time_obj for AY 1617
    ay_1617_sem_1_start = datetime.strptime('2016-08-08', '%Y-%m-%d')
    ay_1617_winter_start = datetime.strptime('2016-12-01', '%Y-%m-%d')
    ay_1617_sem_2_start = datetime.strptime('2017-01-15', '%Y-%m-%d')
    ay_1617_summer_start = datetime.strptime('2017-06-01', '%Y-%m-%d')

    # Create date_time_obj for AY 1718
    ay_1718_sem_1_start = datetime.strptime('2017-08-08', '%Y-%m-%d')
    ay_1718_winter_start = datetime.strptime('2017-12-01', '%Y-%m-%d')
    ay_1718_sem_2_start = datetime.strptime('2018-01-15', '%Y-%m-%d')
    ay_1718_summer_start = datetime.strptime('2018-06-01', '%Y-%m-%d')

    # Create date_time_obj for AY 1819
    ay_1819_sem_1_start = datetime.strptime('2018-08-08', '%Y-%m-%d')
    ay_1819_winter_start = datetime.strptime('2018-12-01', '%Y-%m-%d')
    ay_1819_sem_2_start = datetime.strptime('2019-01-15', '%Y-%m-%d')
    ay_1819_summer_start = datetime.strptime('2019-06-01', '%Y-%m-%d')

    # Create date_time_obj for AY 1920
    ay_1920_sem_1_start = datetime.strptime('2019-08-08', '%Y-%m-%d')
    ay_1920_winter_start = datetime.strptime('2019-12-01', '%Y-%m-%d')
    ay_1920_sem_2_start = datetime.strptime('2020-01-15', '%Y-%m-%d')
    ay_1920_summer_start = datetime.strptime('2020-06-01', '%Y-%m-%d')

    # Create date_time_obj for AY 2021
    ay_2021_sem_1_start = datetime.strptime('2020-08-08', '%Y-%m-%d')
    ay_2021_winter_start = datetime.strptime('2020-12-01', '%Y-%m-%d')
    ay_2021_sem_2_start = datetime.strptime('2021-01-15', '%Y-%m-%d')
    ay_2021_summer_start = datetime.strptime('2021-06-01', '%Y-%m-%d')

    # Create date_time_obj for AY 2022
    ay_2122_sem_1_start = datetime.strptime('2021-08-08', '%Y-%m-%d')

    # Check if date is in AY1617
    if ay_1617_sem_1_start <= date_time_obj <= ay_1617_winter_start:
        return("AY 16/17 Semester 1")
    elif ay_1617_winter_start <= date_time_obj <= ay_1617_sem_2_start:
        return("AY 16/17 Winter Semester")
    elif ay_1617_sem_2_start <= date_time_obj <= ay_1617_summer_start:
        return("AY 16/17 Semester 2")
    elif ay_1617_summer_start <= date_time_obj <= ay_1718_sem_1_start:
        return("AY 16/17 Special Term")

    # Check if date is in AY1718
    if ay_1718_sem_1_start <= date_time_obj <= ay_1718_winter_start:
        return("AY 17/18 Semester 1")
    elif ay_1718_winter_start <= date_time_obj <= ay_1718_sem_2_start:
        return("AY 17/18 Winter Semester")
    elif ay_1718_sem_2_start <= date_time_obj <= ay_1718_summer_start:
        return("AY 17/18 Semester 2")
    elif ay_1718_summer_start <= date_time_obj <= ay_1819_sem_1_start:
        return("AY 17/18 Special Term")

    # Check if date is in AY1819
    if ay_1819_sem_1_start <= date_time_obj <= ay_1819_winter_start:
        return("AY 18/19 Semester 1")
    elif ay_1819_winter_start <= date_time_obj <= ay_1819_sem_2_start:
        return("AY 19/19 Winter Semester")
    elif ay_1819_sem_2_start <= date_time_obj <= ay_1819_summer_start:
        return("AY 18/19 Semester 2")
    elif ay_1819_summer_start <= date_time_obj <= ay_1920_sem_1_start:
        return("AY 18/19 Special Term")

    # Check if date is in AY1920
    if ay_1920_sem_1_start <= date_time_obj <= ay_1920_winter_start:
        return("AY 19/20 Semester 1")
    elif ay_1920_winter_start <= date_time_obj <= ay_1920_sem_2_start:
        return("AY 19/20 Winter Semester")
    elif ay_1920_sem_2_start <= date_time_obj <= ay_1920_summer_start:
        return("AY 19/20 Semester 2")
    elif ay_1920_summer_start <= date_time_obj <= ay_2021_sem_1_start:
        return("AY 19/20 Special Term")

    # Check if date is in AY2021
    if ay_2021_sem_1_start <= date_time_obj <= ay_2021_winter_start:
        return("AY 20/21 Semester 1")
    elif ay_2021_winter_start <= date_time_obj <= ay_2021_sem_2_start:
        return("AY 20/21 Winter Semester")
    elif ay_2021_sem_2_start <= date_time_obj <= ay_2021_summer_start:
        return("AY 20/21 Semester 2")
    elif ay_2021_summer_start <= date_time_obj <= ay_2122_sem_1_start:
        return("AY 20/21 Special Term")

    # Log error message
    print("Please code in valid date in 'helpers/py'")
    return string_date

def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/1.0/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # Contact API
    try:
        api_key = os.environ.get("API_KEY")
        response = requests.get(f"https://cloud-sse.iexapis.com/stable/stock/{urllib.parse.quote_plus(symbol)}/quote?token={api_key}")
        response.raise_for_status()
    except requests.RequestException:
        return None

    # Parse response
    try:
        quote = response.json()
        return {
            "name": quote["companyName"],
            "price": float(quote["latestPrice"]),
            "symbol": quote["symbol"]
        }
    except (KeyError, TypeError, ValueError):
        return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"
