from cs50 import get_string
import re


def main():
    # Ask user input
    text = get_string("Text: ")

    # Remove any whitespaces at the beginning of end
    text = text.strip()

    # we use list comprehension to check for numerical values
    # then, we check the length of the list
    letters = len([i for i in text if i.isalpha()])

    # we just split the text into a list, then check the length of the list
    words = len(text.split())

    # regular expressions can be used to split the text
    # we look for ! ? or . to split
    sentences = len(re.split("[!?.]", text)) - 1

    # We do the calculation
    l = letters / words * 100
    s = sentences / words * 100

    coleman = round((0.0588 * l) - (0.296 * s) - 15.8)

    # Border cases
    if coleman < 1:
        print("Before Grade 1")
    elif coleman > 16:
        print("Grade 16+")
    else:
        print(f"Grade {coleman}")


if __name__ == "__main__":
    main()
