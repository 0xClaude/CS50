from cs50 import get_string


# Define main function
def main():
    # Ask for user input
    name = get_string("What is your name?\n")
    # Greet user
    print(f"hello, {name}")


# Check if file is run directly
if __name__ == "__main__":
    main()