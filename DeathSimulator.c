#include <stdio.h>
#include <stdbool.h>

#define MAX 10

int p, r;
int allocation[MAX][MAX], request[MAX][MAX], max_need[MAX][MAX];
int available[MAX];

// ---------------- VALIDATION ----------------
int is_valid(int value) {
    return value >= 0;
}

// ---------------- WAIT-FOR GRAPH ----------------
void wait_for_graph() {
    printf("\n--- Wait-For Graph ---\n");

    for (int i = 0; i < p; i++) {
        for (int j = 0; j < p; j++) {
            if (i != j) {
                for (int k = 0; k < r; k++) {
                    if (request[i][k] > 0 && allocation[j][k] > 0) {
                        printf("P%d waits for P%d\n", i, j);
                    }
                }
            }
        }
    }
}

// ---------------- DEADLOCK DETECTION ----------------
void deadlock_detection() {
    bool finish[MAX] = {false};
    int work[MAX];

    for (int i = 0; i < r; i++)
        work[i] = available[i];

    while (1) {
        bool found = false;

        for (int i = 0; i < p; i++) {
            if (!finish[i]) {
                bool can_run = true;

                for (int j = 0; j < r; j++) {
                    if (request[i][j] > work[j]) {
                        can_run = false;
                        break;
                    }
                }

                if (can_run) {
                    for (int j = 0; j < r; j++)
                        work[j] += allocation[i][j];

                    finish[i] = true;
                    found = true;
                }
            }
        }

        if (!found) break;
    }

    printf("\n--- Deadlock Detection Result ---\n");

    bool deadlock = false;
    for (int i = 0; i < p; i++) {
        if (!finish[i]) {
            printf("P%d ", i);
            deadlock = true;
        }
    }

    if (deadlock) {
        printf("\nDeadlock detected!\n");

        printf("\nSuggested Recovery:\n");
        printf("- Terminate one process\n");
        printf("- Release resources\n");
        printf("- Avoid circular wait\n");
    } else {
        printf("No deadlock detected.\n");
    }
}

// ---------------- BANKER'S ALGORITHM ----------------
void bankers_algorithm() {
    int need[MAX][MAX];
    bool finish[MAX] = {false};
    int work[MAX];
    int safe_seq[MAX], count = 0;

    // Need calculation
    for (int i = 0; i < p; i++) {
        for (int j = 0; j < r; j++) {
            if (max_need[i][j] < allocation[i][j]) {
                printf("\nError: Max must be >= Allocation!\n");
                return;
            }
            need[i][j] = max_need[i][j] - allocation[i][j];
        }
    }

    for (int i = 0; i < r; i++)
        work[i] = available[i];

    while (count < p) {
        bool found = false;

        for (int i = 0; i < p; i++) {
            if (!finish[i]) {
                bool can_run = true;

                for (int j = 0; j < r; j++) {
                    if (need[i][j] > work[j]) {
                        can_run = false;
                        break;
                    }
                }

                if (can_run) {
                    for (int j = 0; j < r; j++)
                        work[j] += allocation[i][j];

                    safe_seq[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }

        if (!found) break;
    }

    printf("\n--- Deadlock Prediction Result ---\n");

    if (count == p) {
        printf("System is SAFE\nSafe sequence: ");
        for (int i = 0; i < p; i++)
            printf("P%d ", safe_seq[i]);
        printf("\n");
    } else {
        printf("System is UNSAFE (Deadlock may occur)\n");
    }
}

// ---------------- MAIN ----------------
int main() {

    printf("===== Deadlock Simulator & Predictor =====\n");

    printf("Enter number of processes (max 10): ");
    scanf("%d", &p);

    if (p <= 0 || p > MAX) {
        printf("Invalid number of processes!\n");
        return 0;
    }

    printf("Enter number of resources (max 10): ");
    scanf("%d", &r);

    if (r <= 0 || r > MAX) {
        printf("Invalid number of resources!\n");
        return 0;
    }

    // -------- Allocation --------
    printf("\nEnter Allocation Matrix:\n");
    printf("    ");
    for (int j = 0; j < r; j++)
        printf("R%d ", j);
    printf("\n");

    for (int i = 0; i < p; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < r; j++) {
            scanf("%d", &allocation[i][j]);
            if (!is_valid(allocation[i][j])) {
                printf("Invalid allocation value!\n");
                return 0;
            }
        }
    }

    // -------- Request --------
    printf("\nEnter Request Matrix:\n");
    printf("    ");
    for (int j = 0; j < r; j++)
        printf("R%d ", j);
    printf("\n");

    for (int i = 0; i < p; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < r; j++) {
            scanf("%d", &request[i][j]);
            if (!is_valid(request[i][j])) {
                printf("Invalid request value!\n");
                return 0;
            }
        }
    }

    // -------- Available --------
    printf("\nEnter Available Resources:\n");
    for (int i = 0; i < r; i++) {
        printf("R%d: ", i);
        scanf("%d", &available[i]);
        if (!is_valid(available[i])) {
            printf("Invalid available value!\n");
            return 0;
        }
    }

    wait_for_graph();
    deadlock_detection();

    // -------- Maximum --------
    printf("\nEnter Maximum Need Matrix:\n");
    printf("    ");
    for (int j = 0; j < r; j++)
        printf("R%d ", j);
    printf("\n");

    for (int i = 0; i < p; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < r; j++) {
            scanf("%d", &max_need[i][j]);
            if (!is_valid(max_need[i][j])) {
                printf("Invalid max value!\n");
                return 0;
            }
        }
    }

    bankers_algorithm();

    return 0;
}
