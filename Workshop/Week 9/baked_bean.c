#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int size;
    scanf("%d", &size);
    int input[size];
    for (int i = 0; i < size; i++) {
        scanf(" %d", &input[i]);
    }

    int dp[size];
    int **bundleChoices;
    bundleChoices = (int**)malloc(sizeof(int*) * size);

    for (int i = 0; i < size; i++) {
        dp[i] = input[i];
        bundleChoices[i] = calloc(size, sizeof(int));
        bundleChoices[i][i]++;

        for (int j = 0; j < i; j++) {
            // printf("%d, %d, %d\n", dp[i], dp[j], input[i - j - 1]);
            if (dp[j] + input[i - j - 1] > dp[i]) {
                dp[i] = dp[j] + input[i - j - 1];
                memcpy(bundleChoices[i], bundleChoices[j], sizeof(int) * size);
                bundleChoices[i][i - j - 1]++;
            }
        }
        // printf("Current Best Profit: %d\n", dp[i]);
        // printf("Bundle Choices: ");
        // for (int j = 0; j < size; j++) {
        //     printf("%d ", bundleChoices[i][j]);
        // }
        // printf("\n\n");

    }
    printf("%d\n", dp[size - 1]);
    for (int i = 0; i < size; i++) {
        if (bundleChoices[size - 1][i] > 0) {
            printf("%d\n", i + 1);
        }
    }
    for (int i = 0; i < size; i++) {
        free(bundleChoices[i]);
    }
    free(bundleChoices);
}