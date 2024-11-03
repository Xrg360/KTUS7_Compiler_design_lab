#include <stdio.h>
#include <stdlib.h>

#define MAX_STATES 20
#define MAX_ALPHA 20

typedef struct {
    int *transitions[MAX_ALPHA];  
    int e_transitions[MAX_STATES];
    int e_count;
} State;

State states[MAX_STATES];
int e_closure[MAX_STATES][MAX_STATES];
char alphabet[MAX_ALPHA];
int noalpha, nostate, notransition;

void insert_transition(int from, char symbol, int to);
void find_e_closure(int state, int visited[]);
void print_e_closure(int state);

int main() {
    int i, from, to;
    char symbol;

    printf("Enter the number of alphabets: ");
    scanf("%d", &noalpha);

    printf("Enter the alphabets (use 'e' for epsilon):\n");
    for (i = 0; i < noalpha; i++) {
        scanf(" %c", &alphabet[i]);
    }

    printf("Enter the number of states: ");
    scanf("%d", &nostate);

    printf("Enter the number of transitions: ");
    scanf("%d", &notransition);

    printf("Enter the transitions (format: from symbol to) starting from state 0:\n");
    for (i = 0; i < notransition; i++) {
        scanf("%d %c %d", &from, &symbol, &to);
        insert_transition(from, symbol, to);
    }

    printf("\nEpsilon-closures of states:\n");
    for (i = 0; i < nostate; i++) {
        int visited[MAX_STATES] = {0};  // Reset visited array for each state
        find_e_closure(i, visited);
        print_e_closure(i);
    }

    return 0;
}

// Inserts a transition into the transition table
void insert_transition(int from, char symbol, int to) {
    int idx = -1;

    if (symbol == 'e') {
        states[from].e_transitions[states[from].e_count++] = to;
    } else {
        for (int i = 0; i < noalpha; i++) {
            if (alphabet[i] == symbol) {
                idx = i;
                break;
            }
        }

        if (idx == -1) {
            printf("Invalid symbol: %c\n", symbol);
            exit(1);
        }

        if (states[from].transitions[idx] == NULL) {
            states[from].transitions[idx] = calloc(MAX_STATES, sizeof(int));
        }

        states[from].transitions[idx][to] = 1;  // Mark transition
    }
}

// Find epsilon-closure for a given state
void find_e_closure(int state, int visited[]) {
    if (visited[state]) return;

    visited[state] = 1;
    e_closure[state][state] = 1;

    for (int i = 0; i < states[state].e_count; i++) {
        int next_state = states[state].e_transitions[i];
        find_e_closure(next_state, visited);

        for (int j = 0; j < nostate; j++) {
            if (e_closure[next_state][j]) {
                e_closure[state][j] = 1;
            }
        }
    }
}

// Print epsilon-closure for a given state
void print_e_closure(int state) {
    printf("Epsilon-closure of q%d: {", state);

    int first = 1;  // To handle comma placement
    for (int i = 0; i < nostate; i++) {
        if (e_closure[state][i]) {
            if (!first) printf(", ");
            printf("q%d", i);
            first = 0;
        }
    }
    
    printf("}\n");
}
