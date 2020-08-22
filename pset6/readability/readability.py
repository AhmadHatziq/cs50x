from cs50 import get_string


def main():
    # Get text from user
    text = get_string("Text: ")

    # Get number of alphabets in the text
    num_letters = get_total_letters(text)
    # print("Total letters: " + str(num_letters))

    # Get number of words in the text
    num_words = get_total_words(text)
    # print("Total words: " + str(num_words))

    # Get number of sentences in the text
    num_sentences = get_total_sentences(text)
    # print("Total sentences: " + str(num_sentences))

    # Calculate the numbers
    L = (100 * float(num_letters) / float(num_words))
    S = (100 * float(num_sentences) / float(num_words))
    index = 0.0588 * L - 0.296 * S - 15.8
    grade = round(index)

    # Print the grade
    if (grade < 1):

        print("Before Grade 1")

    elif (grade >= 16):

        print("Grade 16+")

    else:

        print("Grade " + str(grade))


# Logic: A sentence always endes with '.', '!', '?'
def get_total_sentences(string):

    special_char_count = 0
    special_chars = ['?', '!', '.']

    for i in range(len(string)):
        char = string[i]

        if (char in special_chars):
            special_char_count += 1

    return special_char_count


# Returns the int number of words present in the argument string
# Logic: Looks for a space and a non-space character
def get_total_words(string):
    if (len(string) == 0):
        return 0

    num_words = 1
    i = 0

    # Loop through each char in the string
    while True:

        if ((string[i] == " ") and (string[i + 1] != " ")):
            num_words += 1

        i += 1

        if (i == len(string)):
            break

    return num_words


# Returns the int number of letters present in the argument string
def get_total_letters(string):
    total_num_of_letters = 0

    # Iterate through each char in the string
    for i in range(len(string)):
        char = string[i]

        # Increment if char is an alphabet
        if char.isalpha() == True:
            total_num_of_letters += 1

    return total_num_of_letters


if __name__ == "__main__":
    main()