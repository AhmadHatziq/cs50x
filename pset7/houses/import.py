import csv
import cs50
from cs50 import SQL
from sys import argv, exit


def main():
    # Check command line arguments
    if len(argv) != 2:
        print("Usage: python import.py [characters.csv]")
        exit(1)

    # Open new connection to db. Do not create new DB as we do not have the CREATE statements
    db = cs50.SQL("sqlite:///students.db")

    # Clear elements inside table
    db.execute("DELETE FROM students")

    # Open csv file
    csv_location = argv[1]

    with open(csv_location, "r") as file:

        # Create DictReader
        # Automatically handles first header row
        reader = csv.DictReader(file, delimiter=",")

        # Iterate over CSV file
        for row in reader:
            name_string = row["name"]
            house_string = row['house']
            birth_string = row["birth"]

            # Extract first, middle, last names
            first_name, middle_name, last_name = None, None, None
            split_string = name_string.split()
            if (len(split_string) == 2):

                # Only first and last name
                first_name = split_string[0]
                last_name = split_string[1]
                middle_name = None

            elif (len(split_string) == 3):

                # Have all 3 names
                first_name = split_string[0]
                last_name = split_string[2]
                middle_name = split_string[1]

            # Extract birth year
            birth_year = int(birth_string)

            # Insert elements into SQL table
            db.execute("INSERT INTO students (first, middle, last, house, birth)    \
                        VALUES(?, ?, ?, ?, ?)", first_name, middle_name, last_name, house_string, birth_year)


if __name__ == "__main__":
    main()