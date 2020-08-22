import csv
import cs50
from cs50 import SQL
from sys import argv, exit


def main():
    # Check command line arguments
    if len(argv) != 2:
        print("Usage: python roster.py [house]")
        exit(1)

    # Extract specified house
    house = argv[1]

    # Connect to db
    db = cs50.SQL("sqlite:///students.db")

    # Query and get a list of dict
    sql_string = "SELECT first, middle, last, birth FROM students WHERE house LIKE '" + house + "'" + " ORDER BY last, first ASC"
    query_result_list = db.execute(sql_string)

    # Print the contents
    for row in query_result_list:

        # Extract name string
        name_string = None
        if row["middle"] is None:
            name_string = row["first"] + " " + row["last"]
        elif row["middle"] is not None:
            name_string = row["first"] + " " + row["middle"] + " " + row["last"]

        # Print contents
        print(name_string + ", born " + str(row["birth"]))


if __name__ == "__main__":
    main()