/* auth.c - authentication implementation */

#include "auth.h"
#include <stdio.h>
#include <string.h>

static const char *USERS_FILE = "users.txt";
static const char *ADMIN_USER = "admin";
static const char *ADMIN_PASS = "admin123";
static const char *ADMIN_ROLE = "admin";

int registerUser(const char *username, const char *password, const char *role) {
    if (!username || !password || !role) return -1;

    // Check if username already exists in users.txt
    FILE *f = fopen(USERS_FILE, "r");
    if (f) {
        char line[200];
        while (fgets(line, sizeof(line), f)) {
            char user_check[50] = {0};
            sscanf(line, "%49[^|]", user_check);
            if (strcmp(user_check, username) == 0) {
                fclose(f);
                printf("Username '%s' already exists\n", username);
                return -1;
            }
        }
        fclose(f);
    }

    // Append new user to users.txt
    f = fopen(USERS_FILE, "a");
    if (!f) {
        printf("Failed to open %s for writing\n", USERS_FILE);
        return -1;
    }

    fprintf(f, "%s|%s|%s\n", username, password, role);
    fflush(f);
    fclose(f);

    printf("User '%s' registered successfully\n", username);
    return 0;
}

int loginUser(const char *username, const char *password, User *loggedIn) {
    if (!username || !password || !loggedIn) return -1;

    // Check hardcoded admin credentials first
    if (strcmp(username, ADMIN_USER) == 0 && strcmp(password, ADMIN_PASS) == 0) {
        strncpy(loggedIn->username, ADMIN_USER, sizeof(loggedIn->username) - 1);
        loggedIn->username[sizeof(loggedIn->username) - 1] = '\0';
        strncpy(loggedIn->password, ADMIN_PASS, sizeof(loggedIn->password) - 1);
        loggedIn->password[sizeof(loggedIn->password) - 1] = '\0';
        strncpy(loggedIn->role, ADMIN_ROLE, sizeof(loggedIn->role) - 1);
        loggedIn->role[sizeof(loggedIn->role) - 1] = '\0';
        printf("Admin login successful\n");
        return 0;
    }

    // Check users.txt for regular user
    FILE *f = fopen(USERS_FILE, "r");
    if (!f) {
        printf("Failed to open %s for reading\n", USERS_FILE);
        return -1;
    }

    char line[200];
    while (fgets(line, sizeof(line), f)) {
        char u[50], p[50], r[20];
        if (sscanf(line, "%49[^|]|%49[^|]|%19[^\n]", u, p, r) == 3) {
            if (strcmp(u, username) == 0 && strcmp(p, password) == 0) {
                strncpy(loggedIn->username, u, sizeof(loggedIn->username) - 1);
                loggedIn->username[sizeof(loggedIn->username) - 1] = '\0';
                strncpy(loggedIn->password, p, sizeof(loggedIn->password) - 1);
                loggedIn->password[sizeof(loggedIn->password) - 1] = '\0';
                strncpy(loggedIn->role, r, sizeof(loggedIn->role) - 1);
                loggedIn->role[sizeof(loggedIn->role) - 1] = '\0';
                fclose(f);
                printf("User '%s' logged in successfully\n", username);
                return 0;
            }
        }
    }

    fclose(f);
    printf("Invalid username or password\n");
    return -1;
}

int isAdmin(User u) {
    return strcmp(u.role, "admin") == 0 ? 1 : 0;
}
