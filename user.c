#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"        // External MD5 library
#include "complaint.h"  // System definitions

#define ID_LEN 50
#define PASS_LEN 50
#define HASH_LEN 33
#define DB_FILE "user.txt"

void md5_hash(const char *input, char *output_32_char_hex) {
    unsigned char digest[16];
    
    // Calls standard MD5 context operations from external md5.h / md5.c
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, input, strlen(input));
    MD5_Final(digest, &ctx);

    // Convert raw 16-byte digest to 32-character Hexadecimal string
    for (int i = 0; i < 16; i++) {
        sprintf(output_32_char_hex + (i * 2), "%02x", digest[i]);
    }
    output_32_char_hex[32] = '\0';
}

int is_id_registered(const char *user_id) {
    FILE *fp = fopen(DB_FILE, "r");
    if (!fp) return 0; // File doesn't exist yet, ID is available

    char file_id[ID_LEN], file_hash[65];
    int flag;
    while (fscanf(fp, "%s %s %d", file_id, file_hash, &flag) == 3) {
        if (strcmp(file_id, user_id) == 0) {
            fclose(fp);
            return 1; // ID found
        }
    }

    fclose(fp);
    return 0;
}

void user_register() {
    char user_id[ID_LEN];
    char hashed_default_pass[HASH_LEN];

    printf("\nEnter your Registration ID (e.g., 252-35-485): ");
    scanf("%49s", user_id);

    if (is_id_registered(user_id)) {
        printf("\nError: Registration ID '%s' is already registered!\n", user_id);
        return;
    }

    // Hash the default password (which is the Registration ID itself)
    md5_hash(user_id, hashed_default_pass);

    FILE *file = fopen(DB_FILE, "a");
    if (!file) {
        printf("\nError accessing user database.\n");
        return;
    }

    // Write format: USER_ID MD5_HASH FIRST_LOGIN_FLAG(1)
    fprintf(file, "%s %s 1\n", user_id, hashed_default_pass);
    fclose(file);

    printf("\nUser registered successfully!");
    printf("\nYour initial password is set to your Registration ID: %s", user_id);
    printf("\nNote: You will be required to set a new private password on your first login.\n");
}

void force_password_reset(const char *user_id) {
    char new_pass[PASS_LEN], confirm_pass[PASS_LEN];
    char new_hash[HASH_LEN];

    printf("\n==================================================");
    printf("\n[SECURITY NOTICE] First Login Detected!");
    printf("\nYou must set a new private password before continuing.");
    printf("\n==================================================\n");

    while (1) {
        printf("\nEnter new password: ");
        scanf("%49s", new_pass);
        printf("Confirm new password: ");
        scanf("%49s", confirm_pass);

        if (strcmp(new_pass, confirm_pass) == 0) {
            break;
        }
        printf("\n[!] Passwords do not match. Please try again.\n");
    }

    // Hash the new private password
    md5_hash(new_pass, new_hash);

    // Read user.txt and write updated credentials to temp.txt
    FILE *fp = fopen(DB_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("\nError updating security credentials.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    char file_id[ID_LEN], file_hash[65];
    int first_login_flag;

    while (fscanf(fp, "%s %s %d", file_id, file_hash, &first_login_flag) == 3) {
        if (strcmp(file_id, user_id) == 0) {
            // Update hash and flip first_login flag from 1 -> 0
            fprintf(temp, "%s %s 0\n", file_id, new_hash);
        } else {
            fprintf(temp, "%s %s %d\n", file_id, file_hash, first_login_flag);
        }
    }

    fclose(fp);
    fclose(temp);

    // Replace original database with temp file
    remove(DB_FILE);
    rename("temp.txt", DB_FILE);

    printf("\n[SUCCESS] Password updated successfully!");
    printf("\nPlease use your new password for all future logins.\n");
}
void user_dashboard(const char *logged_in_user) {
    int choice;

    do {
        printf("\n========================================\n");
        printf("   WELCOME, %s\n", logged_in_user);
        printf("========================================\n");
        printf("1. File a Complaint\n");
        printf("2. View My Complaints\n");
        printf ("3. Frgot Password\n");
        printf("4. Logout\n");

        printf("Enter Choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear buffer on invalid input
            continue;
        }

        switch (choice) {
            case 1:
                FileComplaint(logged_in_user);
                break;

            case 2:
                viewComplaints(logged_in_user);
                break;

            case 3 :
                ForgotPassword();
            break;
            case 4:
                printf("\nLogging out... Returning to portal.\n");
                break;

            default:
                printf("Invalid Choice!\n");
        }

    } while (choice != 4);
}
void user_login() {
    char input_id[ID_LEN], input_pass[PASS_LEN];
    char file_id[ID_LEN], file_hash[65];
    char input_hash[HASH_LEN];
    int first_login_flag = 0;

    FILE *fp = fopen(DB_FILE, "r");
    if (!fp) {
        printf("\nNo registered users found. Please register first.\n");
        return;
    }

    printf("\nEnter Registration ID: ");
    scanf("%49s", input_id);
    printf("Enter Password: ");
    scanf("%49s", input_pass);

    // Hash input password to compare against stored hash
    md5_hash(input_pass, input_hash);

    int login_success = 0;
    while (fscanf(fp, "%s %s %d", file_id, file_hash, &first_login_flag) == 3) {
        if (strcmp(input_id, file_id) == 0 && strcmp(input_hash, file_hash) == 0) {
            login_success = 1;
            break;
        }
    }
    fclose(fp);

    if (login_success) {
        printf("\nCredentials verified. Welcome, %s!\n", input_id);

        // Enforce password change if it's their first login
        if (first_login_flag == 1) {
            force_password_reset(input_id);
        }

        // Pass authenticated ID to user_dashboard
        user_dashboard(input_id);
    } else {
        printf("\nInvalid Registration ID or password. Please try again.\n");
    }
}
user_dashboard(input_username);
        // Handoff: Pass authenticated 'input_id' directly to user complaint module
        // userComplaintPortal(input_id);
    } else {
        printf("\nInvalid Registration ID or password. Please try again.\n");
    }
}

/* =========================================================
 * 4. USER PORTAL ENTRY MENU
 * ========================================================= */
void User() {
    int choice;
    do {
        printf("\n------- Welcome to the User Portal! -------\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear invalid input buffer
            choice = -1;
        }

        switch (choice) {
            case 1:
                user_register();
                break;
            case 2:
                user_login();
                break;
            case 0:
                printf("Returning...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}
