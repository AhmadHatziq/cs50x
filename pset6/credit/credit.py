from cs50 import get_string


def main():
    credit_string = get_string("Number: ")
    # print("Credit num is: " + credit_num)

    # Get Luhns number
    product_sum = 0

    if (len(credit_string) % 2) == 0:

        # Even case
        for i in range(0, len(credit_string), 2):
            char = credit_string[i]
            digit_num = int(char)

            if (digit_num * 2 >= 10):
                product_sum += (digit_num * 2 - 10) + 1
            else:
                product_sum += digit_num * 2

        # print("String len " + str(len(credit_string)))
        for i in range(1, len(credit_string), 2):
            # print(i)
            # print(credit_string[i])
            product_sum += int(credit_string[i])

    else:
        # Odd case
        # Start from index 1
        for i in range(1, len(credit_string), 2):
            char = credit_string[i]
            digit_num = int(char)

            if (digit_num * 2 >= 10):
                product_sum += (digit_num * 2 - 10) + 1
            else:
                product_sum += digit_num * 2

        for i in range(0, len(credit_string), 2):
            product_sum += int(credit_string[i])

    # Extract first 2 digits
    first_digit = int(credit_string[0])
    second_digit = int(credit_string[1])
    first_two = first_digit * 10 + second_digit

    if (product_sum % 10 != 0):
        print("INVALID")
    elif ((len(credit_string) == 15) and (first_two == 37 or first_two == 34)):
        print("AMEX")
    elif ((len(credit_string) == 16) and (first_two > 50) and (first_two < 56)):
        print("MASTERCARD")
    elif ((len(credit_string) == 16 or len(credit_string) == 13) and (first_digit == 4)):
        print("VISA")
    else:
        print("INVALID")


if __name__ == "__main__":
    main()