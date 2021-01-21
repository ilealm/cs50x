#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);
void print_preferences(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();

        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    for (int c = 0; c < candidate_count; c++)
    {
        if (strcmp(candidates[c].name, name) == 0)
        {
            preferences[voter][rank] = c;
            return true;
        }
    }

    return false;
}

// helper function that returns if a candidate is eliminated
void print_preferences(void)
{
    printf("\n PREFERENCES\n");
    for (int voter = 0; voter < voter_count; voter++)
    {
        for (int rank = 0; rank < candidate_count; rank++)
        {
            printf("%i - ", preferences[voter][rank]);
        }
        printf("\n");
    }
    printf("\n VOTES");
    for (int i = 0; i < candidate_count; i++)
    {
        printf("\n%s, %i, ", candidates[i].name, candidates[i].votes);
        printf("%s", candidates[i].eliminated ? "true" : "false");
    }
    printf("\n");
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    int candidate;

    // get the vote of each voter
    for (int voter = 0; voter < voter_count; voter++)
    {
        // search for the first vote that has a not elimitated candidate
        for (int rank = 0; rank < candidate_count; rank++)
        {
            candidate = preferences[voter][rank];

            if (!candidates[candidate].eliminated)
            {
                candidates[candidate].votes += 1;
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // NOTE: I need to convert to float the values to divide, BC if not, will return always a integer.
    int votes_to_win = round((float) voter_count / (float) 2);
    // printf("\nvotes to win %i \n", votes_to_win);

    for (int i = 0; i < candidate_count; i++)
    {
        if ((!candidates[i].eliminated) && (candidates[i].votes > votes_to_win))
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // establich a compatation point
    int min_votes = candidates[0].votes;

    for (int i = 0; i < candidate_count; i++)
    {
        if ((!candidates[i].eliminated) && (candidates[i].votes < min_votes))
        {
            min_votes = candidates[i].votes;
        }
    }

    return min_votes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // establich a compatation point
    bool all_same = true;

    for (int i = 0; i < candidate_count; i++)
    {
        if ((!candidates[i].eliminated) && (candidates[i].votes != min))
        {
            all_same = false;
        }
    }
    return all_same;
}


// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if ((!candidates[i].eliminated) && (candidates[i].votes == min))
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}
