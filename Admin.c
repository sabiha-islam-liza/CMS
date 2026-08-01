#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"        // External MD5 library
#include "complaint.h"

#define SETUP_KEY "syntexerror"
#define ADMIN_DB "admin.txt"
#define USERNAME_LEN 50
#define PASSWORD_LEN 50
#define HASH_LEN 33

/* Helper wrapper for MD5 hashing */
static void get_admin_md5(const char *input, char *output_hash) {
    unsigned char digest[16];
    MD5_CTX ctx;

    MD5_Init(&ctx);
    MD5_Update(&ctx, input, strlen(input));
    MD5_Final(digest, &ctx);

    for (int i = 0; i < 16; i++) {
        sprintf(output_hash + (i * 2), "%02x", digest[i]);
    }
    output_hash[32] = '\0';
}


void adminregister() {
    char key[20];
    
    printf("\nEnter the setup key to register as admin: ");
    scanf("%19s", key);

    if (strcmp(key, SETUP_KEY) != 0) {
        printf("[ERROR] Invalid setup key.\n");
        return;
    }

    char file_username[USERNAME_LEN];
    char file_password[PASSWORD_LEN], confirm_pass[PASSWORD_LEN];
    char hashed_password[HASH_LEN];

    printf("\nCreate a Username: ");
    scanf("%49s", file_username);

    printf("Create a password: ");
    scanf("%49s", file_password);

    printf("Confirm your password: ");
    scanf("%49s", confirm_pass);

    if (strcmp(file_password, confirm_pass) != 0) {
        printf("\n[ERROR] Passwords didn't match. Please try again.\n");
        return;
    }

    // Hash the admin password before storing
    get_admin_md5(file_password, hashed_password);

    FILE *file = fopen(ADMIN_DB, "a");
    if (!file) {
        printf("\n[ERROR] Could not open database file.\n");
        return;
    }

    // Write: USERNAME HASHED_PASSWORD
    fprintf(file, "%s %s\n", file_username, hashed_password);
    fclose(file);

    printf("\n[SUCCESS] Admin registered successfully.\n");
}


int admin_login() {
    char username[USERNAME_LEN], password[PASSWORD_LEN];
    char input_hash[HASH_LEN];
    char file_username[USERNAME_LEN], file_hash[HASH_LEN];

    FILE *fp = fopen(ADMIN_DB, "r");
    
    // FIX #1: Check if file exists BEFORE attempting to read from it
    if (fp == NULL) {
        printf("\n[ERROR] No admin registered yet. Please register first.\n");
        return 0;
    }

    printf("\nEnter your username: ");
    scanf("%49s", username);
    printf("Enter your password: ");
    scanf("%49s", password);

    get_admin_md5(password, input_hash);

    int login_successful = 0;

    // FIX #2: Loop through file to support multiple admin accounts
    while (fscanf(fp, "%s %s", file_username, file_hash) == 2) {
        if (strcmp(username, file_username) == 0 && strcmp(input_hash, file_hash) == 0) {
            login_successful = 1;
            break;
        }
    }
    fclose(fp);

    if (login_successful) {
        printf("\n[SUCCESS] Login successful. Welcome Admin, %s!\n", username);
        // Handoff to Admin Control Panel (e.g., admin_dashboard_menu();)
        return 1;
    } else {
        printf("\n[ERROR] Invalid username or password.\n");
        return 0;
    }
}


void Admin() {
    int choice;

    do {
        printf("\n------ Admin Panel ------\n");
        printf("1. Admin register\n");
        printf("2. Admin login\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear invalid input buffer
            choice = -1;
        }

        switch (choice) {
            case 1:
                adminregister();
                break;
            case 2:
                admin_login();
                break;
            case 0:
                printf("\nExiting Admin Panel...\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 0);
}
