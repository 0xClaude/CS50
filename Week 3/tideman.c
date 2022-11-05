#include <cs50.h>
#include <stdio.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

// add our custom helper function
bool circle(int start, int end);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
// OK
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i] == name)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
// OK
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
// OK
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
// OK
void sort_pairs(void)
{
    for (int i = pair_count; i >= 0; i--)
    {
        for (int j = 0; j < i; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] < preferences[pairs[j + 1].winner][pairs[j + 1].loser])
            {
                pair temp[j];
                temp[j] = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = temp[j];
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
// OK (FINALLY)
void lock_pairs(void)
{
    // looping through the pairs
    for (int i = 0; i < pair_count; i++)
    {
        // let's check if we don't create a circle
        if (!circle(pairs[i].winner, pairs[i].loser))
        {
            // then we can lock it
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

// a helper function to check if we have a circle
bool circle(int start, int end)
{
    // make sure we don't run this indefinitely
    // if start and end are the same, we have a cycle, so we return true
    if (start == end)
    {
        return true;
    }

    // if so far we don't have a circle, let's check for the other candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // do have any other locked, starting with the end of the original string?
        if (locked[end][i])
        {
            // if so, can we go deeper the rabbit hole?
            // we add the start variable to check whether we find a circle
            if (circle(start, i))
            {
                // if we find a neste circle, we also need to return true a level higher
                return true;
            }
        }
    }
    // eventually, when all test fail and we didn't find a circle before, we can lock it!
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // candidate wins if no arrows go in his direction
    for (int i = 0; i < candidate_count; i ++)
    {
        bool arrow = false;
        for (int j = 0; j < candidate_count; j ++)
        {
            if (locked[j][i] == true)
            {
                arrow = true;
            }
        }
        if (arrow == false)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}