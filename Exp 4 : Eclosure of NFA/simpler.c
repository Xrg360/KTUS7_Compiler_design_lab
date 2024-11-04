#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STATES 10

int epsilonTransitions[MAX_STATES][MAX_STATES]; 
int epsilonClosure[MAX_STATES][MAX_STATES];     
bool visited[MAX_STATES];

void dfs(int state, int startState) {
    if (visited[state])
        return;
    visited[state] = true;
    epsilonClosure[startState][state] = 1;
    
    for (int i = 0; i < MAX_STATES; i++) {
        if (epsilonTransitions[state][i] == 1) { 
            dfs(i, startState);
        }
    }
}

void computeEpsilonClosure(int numStates) {
    for (int i = 0; i < numStates; i++) {
        for (int j = 0; j < numStates; j++) {
            visited[j] = false; 
        }
        dfs(i, i);
    }
}

int main() {
    int numStates, numTransitions;
    printf("Enter the number of states: ");
    scanf("%d", &numStates);
    
    for (int i = 0; i < numStates; i++) {
        for (int j = 0; j < numStates; j++) {
            epsilonTransitions[i][j] = 0;
            epsilonClosure[i][j] = 0;
        }
    }
    
    printf("Enter the number of epsilon transitions: ");
    scanf("%d", &numTransitions);
    
    printf("Enter the epsilon transitions (state1 state2 for each transition):\n");
    for (int i = 0; i < numTransitions; i++) {
        int state1, state2;
        scanf("%d %d", &state1, &state2);
        epsilonTransitions[state1][state2] = 1; 
    }
    
    computeEpsilonClosure(numStates);
    
    printf("\nEpsilon Closures:\n");
    for (int i = 0; i < numStates; i++) {
        printf("Epsilon closure of state %d: { ", i);
        for (int j = 0; j < numStates; j++) {
            if (epsilonClosure[i][j] == 1) {
                printf("%d ", j);
            }
        }
        printf("}\n");
    }
    
    return 0;
}
