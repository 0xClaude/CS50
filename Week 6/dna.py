import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # Read database file into a variable
    # We use a list with a nested dictionary
    # Each item in the list corresponds to one person
    # Each person has a dictionary with his/her name and the different DNA subsets
    database = []
    with open(sys.argv[1], "r") as f:
        data = csv.DictReader(f)

        # Since we later want to compare the two dictionaries, we need to convert the integers
        # Basically, we need to loop through every item and check whether it is numeric
        # If so, we can typecast it to an integer
        # If not, we just keep the original value
        for row in data:

            # We need a temporary dictionary, which will later append
            tmp = {}

            # Check each item whether it is numerical or not
            for item in row:
                if row[item].isnumeric():
                    tmp[item] = int(row[item])
                else:
                    tmp[item] = row[item]
            database.append(tmp)

    # Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as f:
        sequence = f.read()

    # Remove whitespaces at the beginning and at the end
        sequence = sequence.strip()

    # Find longest match of each STR in DNA sequence
    srtcounter = {}
    sequencecount = 0

    # Go through the first line in the database
    for item in database:
        for row in item:

            # Ignore the "name" field
            if row == "name":
                continue

            # Check how often the STR is found in the sequence
            else:
                if row not in srtcounter:
                    counter = longest_match(sequence, row)
                    srtcounter[row] = counter
                    sequencecount += 1

    # TODO: Check database for matching profiles

    name = "No match"

    for item in srtcounter:
        for person in database:

            # Check if srtcounter is a sub
            if srtcounter.items() <= person.items():
                name = person['name']
    print(name)

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
