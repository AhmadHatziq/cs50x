#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

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
}
pair;

// Array of candidates
string candidates[MAX];

// Array of pair objects
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
void print_preferences(void);
bool existing_pair(int i, int j);
void print_pairs(void);
bool cause_cycle(int source_node, int destination_node);
void print_graph(void);
int get_starting_node(void);

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

            // Store the vote for each rank
            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }
    // End of recording votes

    // Debug statement to see preferences
    //print_preferences();

    add_pairs();

    // Debug statement to see pairs array
    // printf("Before sorting: \n");
    // print_pairs();

    sort_pairs();

    // Debug statement to see pairs array
    // printf("After sorting: \n");
    print_pairs();

    lock_pairs();

    print_graph();

    print_winner();
    return 0;
}

// Update ranks given a new vote
// string name is the name of the candidate that this voter voted for
// ranks array is used to indicate that the voter has the candidate as their "ranked" preference
// rank is an int of preference: {0, 1, 2, ...}
bool vote(int rank, string name, int ranks[])
{
    // Iterate through and find if the candidate name exists
    for (int i = 0; i < candidate_count; i++)
    {

        // Check if there is a match in names
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;

            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
// Called once, after each voter has typed in votes / ranks
void record_preferences(int ranks[])
{
    // ranks[0] is the first preference

    // Iterate through the ranks array
    // Stop at candidate_count - 1 as last guy is not preferred over anyone
    for (int i = 0; i < candidate_count - 1; i++)
    {
        int more_preferred = ranks[i];

        for (int j = i + 1; j < candidate_count; j++)
        {
            int less_preferred = ranks[j];

            // Update global pref array
            // printf("This guy prefers %s over %s\n", candidates[more_preferred], candidates[less_preferred]);
            preferences[more_preferred][less_preferred] = preferences[more_preferred][less_preferred] + 1;
        }

    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Iterate through preferences array
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {

            if (j != i)
            {

                // printf("No. of people preferring %s over %s: %i \n", candidates[i], candidates[j], preferences[i][j]);

                if (preferences[i][j] > preferences[j][i])
                {
                    // printf("Prefer %s over %s \n", candidates[i], candidates[j]);
                    if (existing_pair(i, j) == false)
                    {

                        // Create pair, insert and increment counter
                        pairs[pair_count].winner = i;
                        pairs[pair_count].loser = j;
                        pair_count = pair_count + 1;

                    }
                }
                else if (preferences[i][j] < preferences[j][i])
                {
                    // printf("Prefer %s over %s \n", candidates[j], candidates[i]);
                    if (existing_pair(j, i) == false)
                    {

                        // Create pair, insert and increment counter
                        pairs[pair_count].winner = j;
                        pairs[pair_count].loser = i;
                        pair_count = pair_count + 1;

                    }

                }
            }
        }
    }
    return;
}

// Returns true if the pair(i,j) where a: winner, b:loser exists
// Returns false if the pair(i,j) where a: winner, b:loser doesnot exists
bool existing_pair(int a, int b)
{
    for (int i = 0; i < pair_count; i++)
    {

        pair current_pair = pairs[i];
        int winner = current_pair.winner;
        int loser = current_pair.loser;

        if ((winner == a) && (loser == b))
        {
            return true;
        }
    }
    return false;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair temp;
    for (int i = 1, j; i < pair_count; i++)
    {
        temp = pairs[i];
        j = i - 1;
        for (; j >= 0 && preferences[pairs[j].winner][pairs[j].loser] < preferences[temp.winner][temp.loser]; j--)
        {
            pairs[j + 1] = pairs[j];
        }
        pairs[j + 1] = temp;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Lock the first pair as there is no chance of a cycle
    pair first_pair = pairs[0];
    locked[first_pair.winner][first_pair.loser] = true;

    for (int i = 1; i < pair_count; i++)
    {
        pair current_pair = pairs[i];

        // Check if that edge will cause a cycle
        // A cycle is when the destination node points back to a source node
        int destination_node = current_pair.loser;
        int source_node = current_pair.winner;
        if (cause_cycle(source_node, destination_node) == true)
        {
            // Do not add the cycle
        }
        else
        {
            // Add the cycle
            locked[current_pair.winner][current_pair.loser] = true;
        }

    }

    return;
}

bool cause_cycle(int source_node, int destination_node)
{
    // Return false ie cannot cause cycle if the source_node is not inside the graph
    bool is_source_inside = false;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {

            // Check if the source node is in any of the entries
            if (locked[i][j] == true)
            {
                if (i == source_node | j == source_node)
                {
                    is_source_inside = true;
                }
            }
        }
    }

    if (is_source_inside == false)
    {
        //printf("Source is not in graph: %i, %i \n", source_node, destination_node);
        return false;
    }

    // At this point, the source is definitely inside the graph
    // Need to check for the destination node
    // If the destination node is not inside the graph, no cycle can be formed too

    bool is_destination_inside = false;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {

            if (locked[i][j] == true)
            {
                // Check if the source node is in any of the entries
                if (i == destination_node | j == destination_node)
                {
                    is_destination_inside = true;
                }
            }
        }
    }

    if (is_destination_inside == false)
    {
        //printf("Destination is not in graph: %i, %i \n", source_node, destination_node);
        return false;
    }

    // Do another check. If the winner is a loser, there will be a cycle


    // Since both source and destination are inside, need to check if the edge forms a cycle
    // Check for the destination node inside the graph,  if there are any incoming edges.
    // If there are, means a cycle can be formed.
    // The destination_node must not have any incoming edges ie be the destination of any other edges

    for (int i = 0; i < candidate_count; i++)
    {

        // There is an outgoing edge from the new destination
        if (locked[destination_node][i] == true)
        {
            //printf("Got edge pointing to destination: %i, %i. i is: %i\n", source_node, destination_node, i);
            return true;
        }
    }

    //printf("All clear: %i, %i \n", source_node, destination_node);
    return false;

}

// Print the winner of the election
void print_winner(void)
{

    int winner;
    int rank;

    for (int i = 0; i < candidate_count; i++)
    {
        rank = 0;
        for (int k = 0; k < candidate_count; k++)
        {
            if (locked[k][i] == false)
            {
                rank++;
            }
        }

        // Prints all the names that are have max rank ie connect and dominate all the other nodes
        if (rank == candidate_count)
        {
            printf("%s\n", candidates[i]);
        }
    }


    return;
}

// Get the starting point to start traversal for print_winner(void)
int get_starting_node(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j] == true)
            {
                return j;
            }
        }
    }
    return -1;
}

void print_preferences(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (j != i)
            {
                printf("No. of people preferring %s over %s: %i \n", candidates[i], candidates[j], preferences[i][j]);
            }
        }
    }
    return;
}

void print_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        pair current_pair = pairs[i];
        int winner = current_pair.winner;
        int loser = current_pair.loser;

        printf("%s > %s\n", candidates[winner], candidates[loser]);
    }
}

void print_graph(void)
{
    printf("Printing locked graph:\n");

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            bool edge_status = locked[i][j];
            if (edge_status == true)
            {
                printf("Edge from %s (%i) to %s (%i)\n", candidates[i], i, candidates[j], j);
            }
        }
    }
}