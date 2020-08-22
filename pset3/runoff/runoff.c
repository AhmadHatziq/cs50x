#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

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
void printVotes();

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
        // For each voted
        // Query for each rank, from each voter
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

        // Debugging statement for checking votes
        // printVotes();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            // printf("Winner found\n");
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins. Ie everyone has same votes
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

        // Terminate in 1 iteration for testing purposes
        // break;
    }
    return 0;
}

// Record preference if vote is valid
// Arguments: voter - index of voter, rank - rank no. from 0 to 2. name - name of candidate
bool vote(int voter, int rank, string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        string current_name = candidates[i].name;

        // If there is a matching name
        if (strcmp(current_name, name) == 0)
        {
            // Mark that this voter, gave an rth rank to candidate i
            preferences[voter][rank] = i;
            return true;
        }
    }

    // No name match found
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // Iterate through the voters
    for (int i = 0; i < voter_count; i++)
    {

        // Extract indexes of the preferred candidates
        int first_prefer_candidate = preferences[i][0];
        int second_prefer_candidate = preferences[i][1];
        int third_prefer_candidate = preferences[i][2];

        /*
        printf("Voter %i has in decreasing order of cand. pref.: %i %i %i \n",i, first_prefer_candidate,
        second_prefer_candidate, third_prefer_candidate);
        */

        // Vote for the candidate which is not eliminated
        candidate first_prefer = candidates[first_prefer_candidate];
        candidate second_prefer = candidates[second_prefer_candidate];
        candidate third_prefer = candidates[third_prefer_candidate];

        if (first_prefer.eliminated == false)
        {
            candidate new_cand = first_prefer;
            new_cand.votes += 1;
            candidates[first_prefer_candidate] = new_cand;
        }
        else if (second_prefer.eliminated == false)
        {
            candidate new_cand = second_prefer;
            new_cand.votes += 1;
            candidates[second_prefer_candidate] = new_cand;
        }
        else if (third_prefer.eliminated == false)
        {
            candidate new_cand = third_prefer;
            new_cand.votes += 1;
            candidates[third_prefer_candidate] = new_cand;
        }
    }

    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // Get the number of votes necessary to win
    float half_votes = (float) voter_count / 2.0;
    int votes_needed_to_win = ceil(half_votes + 0.01);

    // Iterate through candidate array. If someone matches, return true
    for (int i = 0; i < candidate_count; i++)
    {
        candidate current_candidate = candidates[i];
        int current_votes = current_candidate.votes;

        if (current_candidate.eliminated == false)
        {
            if (current_votes == votes_needed_to_win)
            {
                // printf("Winning candidate %i with votes: %i \n", i, current_votes);
                printf("%s\n", current_candidate.name);
                return true;
            }
        }
    }

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // Initialize min to first candidate who is still in the election
    int min_votes = -1;
    for (int i = 0; i < candidate_count; i++)
    {
        candidate current_candidate = candidates[i];

        if (current_candidate.eliminated == false)
        {
            min_votes = current_candidate.votes;
            break;
        }
    }

    // Print error msg if min_votes did not take on a value
    if (min_votes == -1)
    {
        printf("All candidates are eliminated in 'int find_min(void)' function\n");
    }

    // Iterate through array and grab minimum votes

    for (int i = 0; i < candidate_count; i++)
    {
        candidate current_candidate = candidates[i];

        if (current_candidate.eliminated == false)
        {
            if (current_candidate.votes < min_votes)
            {
                min_votes = current_candidate.votes;
            }
        }
    }

    // printf("Minimum votes is %i\n", min_votes);
    return min_votes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // Iterate through all valid candidates. If one has a number not matching, terminate and return false
    for (int i = 0; i < candidate_count; i++)
    {
        candidate current_candidate = candidates[i];
        int current_votes = current_candidate.votes;

        if (current_candidate.eliminated == false)
        {
            if (current_votes != min)
            {
                return false;
            }
        }
    }

    // printf("Is a tie with %i votes \n", min);
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        candidate current_candidate = candidates[i];
        int current_votes = current_candidate.votes;

        // Only check if a candidate is not eliminated
        if (current_candidate.eliminated == false)
        {
            if (current_votes == min)
            {
                current_candidate.eliminated = true;
                candidates[i] = current_candidate;
            }
        }
    }
    return;
}

// Print candidate with votes. To be called after tabulate to check vote counts
void printVotes()
{
    for (int i = 0; i < candidate_count; i++)
    {
        printf("Candidate %i has votes: %i \n", i, candidates[i].votes);
    }
}