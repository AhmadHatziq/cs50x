from cs50 import get_int


def main():
    # Get valid input from user
    height = -1
    while (True):
        height = get_int("Height: ")

        if (is_valid(height) == True):
            break

    # print("Valid n is " + str(n))

    # Start printing the pyramids

    # Find the length of the base string (including spaces).
    base_length = 2 + 2 * height

    # Counter for each level
    for current_height in range(1, height + 1):
        num_brick_left = current_height
        num_space_left = height - current_height

        print_repeated_char(num_space_left, ' ')
        print_repeated_char(num_brick_left, '#')
        print_repeated_char(2, ' ')
        print_repeated_char(num_brick_left, '#')
        print("")


'''
Checks if the argument int is valid
Returns True if between 1 and 8 inclusive
Returns False if not between 1 and 8 inclusive
'''


def is_valid(num):
    if (num < 1) or (num > 8):
        return False
    else:
        return True

# Helper function to print a single char 'n' number of times


def print_repeated_char(n, char):
    for i in range(n):
        print(char, end="")


if __name__ == "__main__":
    main()