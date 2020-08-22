#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
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
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
// Returns true if the candidate exists
// Returns false if the candidate name does not exist
bool vote(string name)
{
    // Search through the candidate array
    for (int i = 0; i < candidate_count; i++)
    {
        candidate current_candidate = candidates[i];
        string current_candidate_name = current_candidate.name;

        // A match is found. Increment and return true
        if (strcmp(current_candidate_name, name) == 0)
        {
            int current_votes = current_candidate.votes;
            current_candidate.votes = current_votes + 1;
            candidates[i] = current_candidate;
            return true;
        }
    }

    // Return false if there is no match iterating through whole array
    return false;
}

// Print the winner (or winners) of the election
// Logic: Get highest number of votes. Then get the candidates who match that number.
void print_winner(void)
{
    // Initialize max_votes to be the first candidate value
    int max_votes = candidates[0].votes;

    // Get the max votes in the array
    for (int i = 0; i < candidate_count; i++)
    {
        int current_vote = candidates[i].votes;

        if (current_vote > max_votes)
        {
            max_votes = current_vote;
        }
    }

    // Print all candidates that match the winning number of votes
    for (int i = 0; i < candidate_count; i++)
    {
        int current_vote = candidates[i].votes;

        if (current_vote == max_votes)
        {
            string winning_name = candidates[i].name;
            printf("%s\n", winning_name);
        }
    }

    return;
}

