#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_LINE 200
#define MAX_SUGGESTION 200

typedef struct {
    int id;
    char name[MAX_NAME];
    int hasVoted;
} Voter;

typedef struct {
    int id;
    char name[MAX_NAME];
    int votes;
} Candidate;

int loadVoters(Voter voters[], int max);
int loadCandidates(Candidate candidates[], int max);
void saveVoters(Voter voters[], int count);
void saveCandidates(Candidate candidates[], int count);
void castVote(Voter voters[], int vCount, Candidate candidates[], int cCount);
void viewResults(Candidate candidates[], int count);
void saveSuggestion(int voterId, char *suggestion);

int main() {
    Voter voters[100];
    Candidate candidates[50];    
    int vCount = loadVoters(voters, 100);
    int cCount = loadCandidates(candidates, 50);
    char password[MAX_NAME];
    char ch;
    int i = 0;

    printf("Please set your admin password use only numbers:");
    while ((ch = getch()) != 13)
        {
            password[i++] = ch;
            printf("*");
        }
    password[i] = '\0';
    printf("\n");
    printf("----Admin password set successfully!-----\n");

    int choice;
    do {
        printf("\n==== Voting System ====\n");
        printf("1. Submit your Vote\n");
        printf("2. View Results\n");
        printf("3. Exit\n");
        printf("Choose: ");
        if (scanf("%d", &choice) != 1) { return 0; }
        getchar();

        if (choice == 1) {
            castVote(voters, vCount, candidates, cCount);
            saveVoters(voters, vCount);
            saveCandidates(candidates, cCount);
        } else if (choice == 2) {
            int pass;
            int attempts = 0;
            printf("Please enter your admin password must be integer:");
            scanf("%d", &pass);

            while (attempts < 2) {
                if (pass == (atoi(password))) {
                    viewResults(candidates, cCount);
                    return 0;
                } else {
                    attempts++;
                    printf("Incorrect password. Attempts left: %d\n", 3 - attempts);
                    printf("Please enter your admin password must be integer:) ");
                    scanf("%d", &pass);
                    if (attempts == 2) {
                        printf("\n");
                        printf("--------Too many incorrect attempts. Exiting------\n");
                        continue;
                    }
                }
            }
        }
    } while (choice != 3);

    return 0;
}

int loadVoters(Voter voters[], int max) {
    FILE *f = fopen("voters.txt", "r");
    if (!f) { printf("Error: voters.txt not found\n"); exit(1); }
    int count = 0;
    while (fscanf(f, "%d,%49[^,],%d\n", &voters[count].id, voters[count].name, &voters[count].hasVoted) == 3) {
        count++;;
        if (count >= max) break;
    }
    fclose(f);
    return count;
}

int loadCandidates(Candidate candidates[], int max) {
    FILE *f = fopen("candidates.txt", "r");
    if (!f) { printf("Error: candidates.txt not found\n"); exit(1); }
    int count = 0;
    while (fscanf(f, "%d,%49[^,],%d\n", &candidates[count].id, candidates[count].name, &candidates[count].votes) == 3) {
        count++;
        if (count >= max) break;
    }
    fclose(f);
    return count;
}

void saveVoters(Voter voters[], int count) {
    FILE *f = fopen("voters.txt", "w");
    if (!f) { printf("Error saving voters!\n"); return; }
    for (int i = 0; i < count; i++) {
        fprintf(f, "%d,%s,%d\n", voters[i].id, voters[i].name, voters[i].hasVoted);
    }
    fclose(f);
}

void saveCandidates(Candidate candidates[], int count) {
    FILE *f = fopen("candidates.txt", "w");
    if (!f) { printf("Error saving candidates!\n"); return; }
    for (int i = 0; i < count; i++) {
        fprintf(f, "%d,%s,%d\n", candidates[i].id, candidates[i].name, candidates[i].votes);
    }
    fclose(f);
}

void saveSuggestion(int voterId, char *suggestion) {
    FILE *f = fopen("suggestions.txt", "a");
    if (!f) { printf("Error saving suggestion!\n"); return; }
    fprintf(f, "%d,%s\n", voterId, suggestion);
    fclose(f);
}

void castVote(Voter voters[], int vCount, Candidate candidates[], int cCount) {
    int voterId;
    printf("Enter your Voter ID: ");
    scanf("%d", &voterId);
    getchar();

    int vIndex = -1;
    for (int i = 0; i < vCount; i++) {
        if (voters[i].id == voterId) {
            vIndex = i;
            break;
        }
    }
    if (vIndex == -1) {
        printf("Voter not found!\n");
        return;
    }
    if (voters[vIndex].hasVoted) {
        printf("You have already voted!\n");
        return;
    }

    printf("\nCandidates:\n");
    for (int i = 0; i < cCount; i++) {
        printf("%d. %s\n", candidates[i].id, candidates[i].name);
    }

    int choice;
    printf("Enter Candidate ID to vote: ");
    scanf("%d", &choice);
    getchar();

    int cIndex = -1;
    for (int i = 0; i < cCount; i++) {
        if (candidates[i].id == choice) {
            cIndex = i;
            break;
        }
    }

    if (cIndex == -1) {
        printf("Invalid Candidate ID!\n");
        return;
    }

    candidates[cIndex].votes++;
    voters[vIndex].hasVoted = 1;

    printf("Vote cast successfully!\n");

    char suggestion[MAX_SUGGESTION];
    printf("Would you like to leave a suggestion? (y/n): ");
    char ans;
    scanf(" %c", &ans);
    getchar();
    if (ans == 'y' || ans == 'Y') {
        printf("Enter suggestion: ");
        fgets(suggestion, MAX_SUGGESTION, stdin);
        suggestion[strcspn(suggestion, "\n")] = '\0';
        saveSuggestion(voterId, suggestion);
    }
}

void viewResults(Candidate candidates[], int count) {
    printf("\n==== Voting Results ====\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d votes\n", candidates[i].name, candidates[i].votes);
    }
}

