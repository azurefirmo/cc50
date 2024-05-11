#include <cs50.h>
#include <stdio.h>
#include <string.h>

#define MAX 9

typedef struct
{
    string name;
    int votes;
}
candidate;

candidate candidates[MAX];

int candidate_count;

bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

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

    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    print_winner();
}

bool vote(string name)
{


    for (int currCandidate = 0; currCandidate < candidate_count; currCandidate++) {

        if (strcmp(name, candidates[currCandidate].name) == 0) {
            candidates[currCandidate].votes++;

            return true;
        }
    }

    return false;
}

void print_winner(void)
{

    int topCandidateNumberOfVotes = 0;

    int currCandidate = 0;
    while(currCandidate < candidate_count)
    {
        if(topCandidateNumberOfVotes < candidates[currCandidate].votes)
        {
            topCandidateNumberOfVotes = candidates[currCandidate].votes;
        }

        currCandidate++;
    }


    for (currCandidate = 0; currCandidate < candidate_count; currCandidate++)
    {
        if(candidates[currCandidate].votes == topCandidateNumberOfVotes)
        {
            printf("%s\n", candidates[currCandidate].name);
        }
    }

    return;
}
