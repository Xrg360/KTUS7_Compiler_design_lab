#include <stdio.h>
#include <stdlib.h>

struct Node {
    int state;
    struct Node *next;
};

void findClosure(int currentState, int startState);
void insertTransition(int fromState, char symbol, int toState);
int findAlphabetIndex(char symbol);
void computeFinalStates(void);
void unionClosure(int state);
void printClosure(int state);

static int closureSet[20], numStates, numAlphabets, startState, numTransitions, numFinalStates;
static int finalStates[20], buffer[20], eClosure[20][20] = {0};
char alphabet[20];
struct Node *transitions[20][20] = {NULL};

int main() {
    int i, j, stateFrom, stateTo;
    char symbol;

    printf("Enter the number of alphabets: ");
    scanf("%d", &numAlphabets);
    getchar();  // consume newline

    printf("NOTE: Use letter 'e' for epsilon. 'e' must be the last alphabet if present.\n");
    printf("Enter the alphabets: ");
    for (i = 0; i < numAlphabets; i++) {
        alphabet[i] = getchar();
        getchar();  // consume newline
    }

    printf("Enter the number of states: ");
    scanf("%d", &numStates);

    printf("Enter the start state: ");
    scanf("%d", &startState);

    printf("Enter the number of final states: ");
    scanf("%d", &numFinalStates);

    printf("Enter the final states: ");
    for (i = 0; i < numFinalStates; i++) {
        scanf("%d", &finalStates[i]);
    }

    printf("Enter the number of transitions: ");
    scanf("%d", &numTransitions);

    printf("Enter transitions (format: fromState symbol toState):\n");
    for (i = 0; i < numTransitions; i++) {
        scanf("%d %c%d", &stateFrom, &symbol, &stateTo);
        insertTransition(stateFrom, symbol, stateTo);
    }

    // Compute epsilon closures
    for (i = 1; i <= numStates; i++) {
        for (j = 0; j < 20; j++) {
            buffer[j] = 0;
            eClosure[i][j] = 0;
        }
        findClosure(i, i);
    }

    // Print equivalent NFA without epsilon transitions
    printf("\nEquivalent NFA without epsilon transitions\n");
    printf("--------------------------------------------\n");
    printf("Start state: ");
    printClosure(startState);
    
    printf("\nAlphabets: ");
    for (i = 0; i < numAlphabets; i++) {
        printf("%c ", alphabet[i]);
    }

    printf("\nStates: ");
    for (i = 1; i <= numStates; i++) {
        printClosure(i);
    }

    printf("\nTransitions:\n");
    for (i = 1; i <= numStates; i++) {
        for (j = 0; j < numAlphabets - 1; j++) {
            for (int m = 1; m <= numStates; m++) {
                closureSet[m] = 0;
            }

            for (int k = 0; eClosure[i][k] != 0; k++) {
                int state = eClosure[i][k];
                struct Node *temp = transitions[state][j];
                while (temp != NULL) {
                    unionClosure(temp->state);
                    temp = temp->next;
                }
            }

            printf("\nq%d --%c--> {", i, alphabet[j]);
            for (int n = 1; n <= numStates; n++) {
                if (closureSet[n]) {
                    printf("q%d,", n);
                }
            }
            printf("}");
        }
    }

    printf("\nFinal states: ");
    computeFinalStates();

    return 0;
}

void findClosure(int currentState, int startState) {
    static int c = 0;  // Declare and initialize c

    if (buffer[currentState]) return;

    eClosure[startState][c++] = currentState;
    buffer[currentState] = 1;

    // Check epsilon transitions (last alphabet is epsilon if present)
    if (alphabet[numAlphabets - 1] == 'e' && transitions[currentState][numAlphabets - 1] != NULL) {
        struct Node *temp = transitions[currentState][numAlphabets - 1];
        while (temp != NULL) {
            findClosure(temp->state, startState);
            temp = temp->next;
        }
    }
}
void insertTransition(int fromState, char symbol, int toState) {
    int alphaIndex = findAlphabetIndex(symbol);
    if (alphaIndex == -1) {
        printf("Invalid alphabet: %c\n", symbol);
        exit(1);
    }

    struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));
    newNode->state = toState;
    newNode->next = transitions[fromState][alphaIndex];
    transitions[fromState][alphaIndex] = newNode;
}

int findAlphabetIndex(char symbol) {
    for (int i = 0; i < numAlphabets; i++) {
        if (alphabet[i] == symbol) {
            return i;
        }
    }
    return -1;
}

void unionClosure(int state) {
    for (int i = 0; eClosure[state][i] != 0; i++) {
        closureSet[eClosure[state][i]] = 1;
    }
}

void computeFinalStates() {
    for (int i = 0; i < numFinalStates; i++) {
        for (int j = 1; j <= numStates; j++) {
            for (int k = 0; eClosure[j][k] != 0; k++) {
                if (eClosure[j][k] == finalStates[i]) {
                    printClosure(j);
                }
            }
        }
    }
}

void printClosure(int state) {
    printf("{");
    for (int i = 0; eClosure[state][i] != 0; i++) {
        printf("q%d,", eClosure[state][i]);
    }
    printf("}\t");
}
