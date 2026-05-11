/* auth.h - authentication interface */

#ifndef AUTH_H
#define AUTH_H

#include "structs.h"

/*
 * Register a new user with given username, password, and role.
 * Returns 0 on success, -1 if username already exists.
 */
int registerUser(const char *username, const char *password, const char *role);

/*
 * Attempt login with given username and password.
 * On success, fills loggedIn with user info and returns 0.
 * On failure, returns -1.
 */
int loginUser(const char *username, const char *password, User *loggedIn);

/*
 * Check if user has admin role.
 * Returns 1 if admin, 0 otherwise.
 */
int isAdmin(User u);

#endif // AUTH_H
