#include <stdio.h>
#include <string.h>
#include "md5.h"        // MD5 library for hashing
#include "complaint.h"  // Constants (DB_FILE, ID_LEN, PASS_LEN, HASH_LEN)

void forgetPassword() {
    FILE *file = fopen(DB_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!file || !temp) {
        printf("\n[ERROR] Unable to access database files.\n");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    char input_id[ID_LEN];
    char new_pass[PASS_LEN], confirm_pass[PASS_LEN];
    char new_hash[HASH_LEN];

    char file_id[ID_LEN], file_hash[65];
    int first_login_flag;
    int found = 0;

    printf("\n===== FORGOT PASSWORD =====\n");
    printf("Enter Registration ID: ");
    scanf("%49s", input_id);

    // Scan through database line by line
    while (fscanf(file, "%s %s %d", file_id, file_hash, &first_login_flag) == 3) {
        if (strcmp(file_id, input_id) == 0) {
            found = 1;

            printf("Create New Password: ");
            scanf("%49s", new_pass);
            printf("Confirm Password: ");
            scanf("%49s", confirm_pass);

            if (strcmp(new_pass, confirm_pass) == 0) {
                // Hash the new password before storing
                md5_hash(new_pass, new_hash);
                
                // Write updated record: new hash, reset first_login_flag to 0
                fprintf(temp, "%s %s 0\n", file_id, new_hash);
                printf("\n[SUCCESS] Password Changed Successfully!\n");
            } else {
                // Keep the old hash intact if passwords didn't match
                fprintf(temp, "%s %s %d\n", file_id, file_hash, first_login_flag);
                printf("\n[ERROR] Passwords do not match! Password left unchanged.\n");
            }
        } else {
            // Write untouched records to temp file
            fprintf(temp, "%s %s %d\n", file_id, file_hash, first_login_flag);
        }
    }

    fclose(file);
    fclose(temp);

    // Replace old user database with the updated temp file
    remove(DB_FILE);
    rename("temp.txt", DB_FILE);

    if (!found) {
        printf("\n[ERROR] Invalid Registration ID! User not found.\n");
    }
}
