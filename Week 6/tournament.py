# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = []
    # DONE: Read teams into memory from file
    # Open file
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)

        # Iterate through rows and then append teams to list
        for row in reader:
            # Create temp dictionary
            tmp = {"team": row["team"],
                   # Don't forget to typecast the rating to an integer
                   "rating": int(row["rating"])
                   }
            # Append the dictionary to the list
            teams.append(tmp)

    counts = {}
    # DONE: Simulate N tournaments and keep track of win counts
    for _ in range(N):

        # Get hold of the winner
        winner = simulate_tournament(teams)

        # Check if winner already won before
        # If so, increment wincount
        if winner in counts:
            counts[winner] += 1

        # Else: set wincount to 1
        else:
            counts[winner] = 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    # DONE
    n = len(teams)

    # Simumlate rounds until only one team is left
    while (n > 1):
        teams = simulate_round(teams)
        n = len(teams)
    return teams[0]["team"]


if __name__ == "__main__":
    main()
