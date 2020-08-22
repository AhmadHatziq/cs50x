import csv
from sys import argv, exit


def main():

    # Check command line arguments
    if len(argv) != 3:
        print("Usage: python dna.py [database] [dna sequence]")
        exit(1)

    # Initialize vars for reading csv file
    database_location = argv[1]
    dna_location = argv[2]
    str_types = []
    database_dict = {}

    # Stores mapping of row index to str column string
    str_dict = {}

    # Read the database csv file into memory
    with open(database_location) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')

        # Iterate through rows
        line_count = 0
        for row in csv_reader:

            # Process headers and STR values
            if line_count == 0:

                # Get the str types from the headers
                num_columns = len(row)
                for i in range(1, num_columns):
                    str_name = row[i]
                    str_types.append(str_name)
                    str_dict[i] = str_name

                line_count += 1

            else:

                # Extract name
                name = row[0]

                # Extract str values, store as dist
                single_dict = {}
                for i in range(1, num_columns):
                    str_name = str_dict[i]
                    str_value = int(row[i])
                    single_dict[str_name] = str_value

                # Store the name as a key, str dict as a value
                database_dict[name] = single_dict

                line_count += 1

    # Verify that csv is read succesfully
    # print("DB values: ")
    # print(database_dict)

    # Read the dna sequence file
    dna_file = open(dna_location, "r")
    dna_sequence = dna_file.read()

    # Get the consecutibe counts for each str
    dna_str_concecutive_counts = {}
    for single_str in str_types:

        # Declare index_counts, which stores the counts of substring at each index of the dna string
        starting_index = []

        # Loop through each character and get the starting positions
        for i in range(len(dna_sequence) - len(single_str) + 1):
            # subsetting excluds the right index
            substring = dna_sequence[i: i + len(single_str)]

            if (substring == single_str):
                starting_index.append(i)

        # For each starting position, see how far the sequence goes
        # NTS: Check for case where no '1' in index_counts
        max_consecutive_count = 0
        for index in starting_index:

            # Get a substring from index to end of dna_seq
            substring = dna_sequence[index: len(dna_sequence)]
            current_count = 0
            starting_index = index

            while True:

                if (len(substring) < len(single_str)):
                    break

                if (single_str == dna_sequence[starting_index: starting_index + len(single_str)]):
                    current_count += 1
                    starting_index += len(single_str)

                else:
                    # Thee is no match and hence, no consecutive strings
                    break

            # print("Starting index: " + str(index) + "| Consec counts: " + str(current_count))

            # Store the max value
            if current_count > max_consecutive_count:
                max_consecutive_count = current_count

        # Store value of max consec counts
        dna_str_concecutive_counts[single_str] = max_consecutive_count

    # Check counts for the dna str
    # print(dna_str_concecutive_counts)

    # Search for a match from the database
    found_match = False
    for person in database_dict:

        # Extract the dictionary
        person_dict = database_dict[person]

        if (is_person_matching_dna_counts(person_dict, dna_str_concecutive_counts) == True):
            print(person)
            found_match = True

    if (found_match == False):
        print("No match")

# Function which returns true or false, checking if the contents of the 2 dictionaries match


def is_person_matching_dna_counts(person_dict, consecutive_dict):

    for single_str in person_dict:
        person_value = int(person_dict[single_str])
        sequence_value = int(consecutive_dict[single_str])

        if (person_value != sequence_value):
            return False

    # Only return true if we exit out ie all fields match
    return True


if __name__ == "__main__":
    main()