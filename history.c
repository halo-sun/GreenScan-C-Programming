/* history.c - history management implementation */

#include "history.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

static const char *HISTORY_FILE = "history.txt";
static const char *SEARCHCOUNT_FILE = "searchcount.txt";

int saveHistory(char *username, Product p) {
    if (!username) return -1;

    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    char timestamp[50];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

    FILE *f = fopen(HISTORY_FILE, "a");
    if (!f) {
        printf("Failed to open %s for writing\n", HISTORY_FILE);
        return -1;
    }

    fprintf(f, "%s|%s|%s|%s\n", username, p.barcode, p.name, timestamp);
    fflush(f);
    fclose(f);
    return 0;
}

int viewHistory(char *username) {
    if (!username) return -1;

    FILE *f = fopen(HISTORY_FILE, "r");
    if (!f) {
        printf("No history file found\n");
        return -1;
    }

    char line[300];
    int found = 0;

    printf("\n=== Search History for %s ===\n", username);

    while (fgets(line, sizeof(line), f)) {
        char u[50], b[20], pn[100], ts[50];
        if (sscanf(line, "%49[^|]|%19[^|]|%99[^|]|%49[^\n]", u, b, pn, ts) == 4) {
            if (strcmp(u, username) == 0) {
                printf("%s - %s (Barcode: %s)\n", ts, pn, b);
                found = 1;
            }
        }
    }

    printf("=============================\n\n");
    fclose(f);

    if (!found) {
        printf("No history found for user '%s'\n", username);
        return -1;
    }
    return 0;
}

int updateSearchCount(Product p) {
    SearchCount counts[1024];
    int count = 0;

    // Read existing search counts
    FILE *f = fopen(SEARCHCOUNT_FILE, "r");
    if (f) {
        char line[200];
        while (fgets(line, sizeof(line), f) && count < 1024) {
            char b[20], pn[100];
            int c;
            if (sscanf(line, "%19[^|]|%99[^|]|%d", b, pn, &c) == 3) {
                strcpy(counts[count].barcode, b);
                strcpy(counts[count].productName, pn);
                counts[count].count = c;
                count++;
            }
        }
        fclose(f);
    }

    // Find and update or add new entry
    int found = 0;
    for (int i = 0; i < count; ++i) {
        if (strcmp(counts[i].barcode, p.barcode) == 0) {
            counts[i].count++;
            found = 1;
            break;
        }
    }

    if (!found && count < 1024) {
        strcpy(counts[count].barcode, p.barcode);
        strcpy(counts[count].productName, p.name);
        counts[count].count = 1;
        count++;
    }

    // Write back all counts
    f = fopen(SEARCHCOUNT_FILE, "w");
    if (!f) {
        printf("Failed to open %s for writing\n", SEARCHCOUNT_FILE);
        return -1;
    }

    for (int i = 0; i < count; ++i) {
        fprintf(f, "%s|%s|%d\n", counts[i].barcode, counts[i].productName, counts[i].count);
    }
    fflush(f);
    fclose(f);
    return 0;
}

int viewMostSearched(void) {
    SearchCount counts[1024];
    int count = 0;

    FILE *f = fopen(SEARCHCOUNT_FILE, "r");
    if (!f) {
        printf("No search count file found\n");
        return -1;
    }

    char line[200];
    while (fgets(line, sizeof(line), f) && count < 1024) {
        char b[20], pn[100];
        int c;
        if (sscanf(line, "%19[^|]|%99[^|]|%d", b, pn, &c) == 3) {
            strcpy(counts[count].barcode, b);
            strcpy(counts[count].productName, pn);
            counts[count].count = c;
            count++;
        }
    }
    fclose(f);

    if (count == 0) {
        printf("No search history available\n");
        return -1;
    }

    // Sort by count descending (simple bubble sort for top 10)
    for (int i = 0; i < count - 1; ++i) {
        for (int j = i + 1; j < count; ++j) {
            if (counts[j].count > counts[i].count) {
                SearchCount temp = counts[i];
                counts[i] = counts[j];
                counts[j] = temp;
            }
        }
    }

    printf("\n=== Top 10 Most Searched Products ===\n");
    int limit = count < 10 ? count : 10;
    for (int i = 0; i < limit; ++i) {
        printf("%d. %s (Barcode: %s, Searches: %d)\n",
               i + 1, counts[i].productName, counts[i].barcode, counts[i].count);
    }
    printf("=====================================\n\n");
    return 0;
}
