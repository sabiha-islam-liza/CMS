#include "admin.h"
#include "notifications.h"
#include "md5.h"

void adminregister()
{
    char key[20];
    printf("Enter the setup key to register as admin: ");
    scanf("%s", key);
    if (strcmp(key, setup_key) != 0)
    {
        printf("Invalid setup key.\n");
        return;
    }
    else
    {
        printf("\n Create a Username: ");
        char file_username[50];
        scanf ("%s" , file_username);

        printf("\n Create a password: ");
        char file_password[50];
        scanf("%s", file_password);

        printf("Confirm your password: ");
        char confirm_pass[50];
        scanf("%s", confirm_pass);

        if (strcmp(file_password, confirm_pass) != 0)
        {
            printf ("\n Password didn't match. Please try again.\n");
            return;
        }
         else
         {

            FILE *file = fopen("admin.txt", "a");

            fprintf(file, "%s\n%s\n", file_username, file_password);
            fclose(file);
            printf("Admin registered successfully.\n");
         }



    }
}

void viewAllComplaints() {
    Complaint complaints[MAX];
    int count = 0;

    int success = load_all_complaints(complaints, &count);

    if (!success || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printComplaint(complaints[i]);
    }

    printf("\nTotal Complaints: %d\n", count);
}

void viewComplaintsByAssignedTeam(const char *team_name) {
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < count; i++) {
#ifdef _WIN32
        if (_stricmp(complaints[i].assignedTeam, team_name) == 0) {
#else
        if (strcasecmp(complaints[i].assignedTeam, team_name) == 0) {
#endif
            printComplaint(complaints[i]);
            found++;
        }
    }

    if (!found) {
        printf("\nNo complaints assigned to %s.\n", team_name);
    } else {
        printf("\nTotal complaints assigned to %s: %d\n", team_name, found);
    }
}

void viewComplaintByID() {
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    int id, found = 0;
    printf("Enter Complaint ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (complaints[i].id == id) {
            printComplaint(complaints[i]);
            found = 1;
            break;
        }
    }

    if (!found) printf("\nComplaint not found.\n");
}

void viewComplaintsByStatus() {
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    char status[20];
    printf("Enter Status (Open/In Progress/Resolved/Closed): ");
    getchar();
    fgets(status, sizeof(status), stdin);
    status[strcspn(status, "\n")] = '\0';

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcasecmp(complaints[i].status, status) == 0) {
            printComplaint(complaints[i]);
            found = 1;
        }
    }

    if (!found) printf("\nNo complaints found with status '%s'.\n", status);
}

void viewComplaintsByDate() {
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    Complaint temp;
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(complaints[j].date, complaints[j + 1].date) > 0) {
                temp = complaints[j];
                complaints[j] = complaints[j + 1];
                complaints[j + 1] = temp;
            }
        }
    }

    printf("\n===== Complaints Sorted by Date =====\n");
    for (int i = 0; i < count; i++) {
        printComplaint(complaints[i]);
    }
}

int getPriorityValue(const char *priority) {
    if (!priority) return 0;

    // Use case-insensitive comparison (handles "High", "high", "HIGH")
#ifdef _WIN32
    if (_stricmp(priority, "High") == 0) return 3;
    if (_stricmp(priority, "Medium") == 0) return 2;
    if (_stricmp(priority, "Low") == 0) return 1;
#else
    if (strcasecmp(priority, "High") == 0) return 3;
    if (strcasecmp(priority, "Medium") == 0) return 2;
    if (strcasecmp(priority, "Low") == 0) return 1;
#endif

    return 0; // Default fallback for unassigned/unknown priorities
}

void viewComplaintsByPriority() {
    Complaint complaints[MAX];
    int count = 0;

    // Fix: Pass &count to populate total complaints, do NOT overwrite count with return status
    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    // Bubble sort complaints in descending order of priority value (3 -> 2 -> 1 -> 0)
    Complaint temp;
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (getPriorityValue(complaints[j].priority) < getPriorityValue(complaints[j + 1].priority)) {
                temp = complaints[j];
                complaints[j] = complaints[j + 1];
                complaints[j + 1] = temp;
            }
        }
    }

    printf("\n===== Complaints Sorted by Priority =====\n");
    for (int i = 0; i < count; i++) {
        printComplaint(complaints[i]);
    }
    printf("Total Complaints: %d\n", count);
}

void viewDepartmentComplaints(const char *categoryFilter) {
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(categoryFilter, "ALL") == 0 || strcasecmp(complaints[i].category, categoryFilter) == 0) {
            printComplaint(complaints[i]);
            found++;
        }
    }

    if (found == 0) {
        printf("\nNo complaints found for category: %s\n", categoryFilter);
    } else {
        printf("\nTotal Complaints Found: %d\n", found);
    }
}

void viewComplaintsMenu()
{
    int choice;

    do
    {
        printf("\n========== View Complaints ==========\n");
        printf("1. View All Complaints\n");
        printf("2. Search Complaint by ID\n");
        printf("3. View Complaints by Status\n");
        printf("4. Sort by Date\n");
        printf("5. Sort by Priority\n");
        printf("0. Back\n");

        printf("\nEnter your choice: ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                viewAllComplaints();
                break;

            case 2:
                viewComplaintByID();
                break;

            case 3:
                viewComplaintsByStatus();
                break;

            case 4:
                viewComplaintsByDate();
                break;

            case 5:
                viewComplaintsByPriority();
                break;

            case 0:
                printf("Returning...\n");
                break;

            default:
                printf("Invalid Choice!\n");
        }

    }while(choice != 0);
}

void assign_Team(Complaint complaints[], int count, Complaint *c) {
    printf("\n========== REASSIGN CATEGORY & TEAM ==========\n");
    printf("Complaint ID     : %d\n", c->id);
    printf("Current Category : %s\n", c->category);
    printf("Current Team     : %s\n", c->assignedTeam);
    printf("----------------------------------------------\n");

    int choice;
    printf("Select New Category & Responsible Team:\n");
    printf("1. Technical           -> Assign to: IT Support\n");
    printf("2. Accounts            -> Assign to: Accounts Admin\n");
    printf("3. Maintenance         -> Assign to: Maintenance Team\n");
    printf("4. Library             -> Assign to: Library Admin\n");
    printf("5. Exam                -> Assign to: Exam Head\n");
    printf("6. Security / Proctor  -> Assign to: Proctor Office\n");
    printf("Enter choice: ");

    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        printf("\n[!] Invalid input choice.\n");
        return;
    }

    switch (choice) {
        case 1:
            strcpy(c->category, "Technical");
            strcpy(c->assignedTeam, "IT Support");
            break;
        case 2:
            strcpy(c->category, "Accounts");
            strcpy(c->assignedTeam, "Accounts Admin");
            break;
        case 3:
            strcpy(c->category, "Maintenance");
            strcpy(c->assignedTeam, "Maintenance Team");
            break;
        case 4:
            strcpy(c->category, "Library");
            strcpy(c->assignedTeam, "Library Admin");
            break;
        case 5:
            strcpy(c->category, "Exam");
            strcpy(c->assignedTeam, "Exam Head");
            break;
        case 6:
            strcpy(c->category, "Security");
            strcpy(c->assignedTeam, "Proctor");
            break;
        default:
            printf("\n[!] Invalid selection.\n");
            return;
    }

    // Automatically transition status from "Open" to "In Progress" upon assignment
    if (strcasecmp(c->status, "Open") == 0) {
        strcpy(c->status, "In Progress");
    }

    // Save updated struct array back to file
    save_complaints(complaints, count);

    printf("\n[SUCCESS] Complaint #%d reclassified and reassigned!\n", c->id);
    printf("New Category: %s | New Team: %s | Status: %s\n",
           c->category, c->assignedTeam, c->status);

   notify_assigned_team(c->assignedTeam, c->id, c->status);
}

void updateStatus(const char *team_name)
{
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0)
    {
        printf("\n[!] No complaints found.\n");
        return;
    }

    printf("\n================ %s - UPDATE STATUS ================\n",
           team_name);

    int team_count = 0;

    /* Show complaints assigned to this team */
    for (int i = 0; i < count; i++)
    {
        if (strcmp(complaints[i].assignedTeam, team_name) == 0)
        {
            printf("\nComplaint ID : %d", complaints[i].id);
            printf("\nCategory     : %s", complaints[i].category);
            printf("\nFiled By     : %s", complaints[i].user);
            printf("\nStatus       : %s", complaints[i].status);
            printf("\n---------------------------------------------\n");

            team_count++;
        }
    }

    if (team_count == 0)
    {
        printf("\n[!] No complaints are assigned to %s.\n", team_name);
        return;
    }

    int id;

    printf("\nEnter Complaint ID to update: ");

    if (scanf("%d", &id) != 1)
    {
        while (getchar() != '\n');
        printf("[!] Invalid Complaint ID.\n");
        return;
    }

    /* Find the complaint belonging to this team */
    int found = -1;

    for (int i = 0; i < count; i++)
    {
        if (complaints[i].id == id &&
            strcmp(complaints[i].assignedTeam, team_name) == 0)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("\n[!] Complaint not found or this complaint is not assigned to %s.\n",
               team_name);
        return;
    }

    printf("\nCurrent Status: %s\n", complaints[found].status);

    printf("\n========== UPDATE STATUS ==========\n");
    printf("1. In Progress\n");
    printf("2. Resolved\n");
    printf("3. Closed\n");
    printf("Enter choice: ");

    int choice;

    if (scanf("%d", &choice) != 1)
    {
        while (getchar() != '\n');
        printf("[!] Invalid choice.\n");
        return;
    }

    switch (choice)
    {
        case 1:
            strcpy(complaints[found].status, "In Progress");
            break;

        case 2:
            strcpy(complaints[found].status, "Resolved");
            break;

        case 3:
            strcpy(complaints[found].status, "Closed");
            break;

        default:
            printf("\n[!] Invalid choice. Status not changed.\n");
            return;
    }

    /* Save updated status */
    if (!save_complaints(complaints, count))
    {
        printf("\n[!] Failed to save complaint status.\n");
        return;
    }

    printf("\n[SUCCESS] Complaint #%d status updated to '%s'.\n",
           complaints[found].id,
           complaints[found].status);

    /* Notify the user */
    char msg[256];

    snprintf(msg,
             sizeof(msg),
             "Your complaint #%d status has been updated to: %s",
             complaints[found].id,
             complaints[found].status);

    send_notification(
        complaints[found].user,
        complaints[found].id,
        msg
    );
}

void deleteComplaint() {
    Complaint complaints[MAX];
    int count = 0;

    // Load existing complaints from file
    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found to delete.\n");
        return;
    }

    int id;
    printf("\nEnter Complaint ID to delete: ");
    if (scanf("%d", &id) != 1) {
        while (getchar() != '\n'); // Clear invalid input buffer
        printf("\n[!] Invalid input.\n");
        return;
    }

    int found_index = -1;
    for (int i = 0; i < count; i++) {
        if (complaints[i].id == id) {
            found_index = i;
            break;
        }
    }

    if (found_index == -1) {
        printf("\n[!] Complaint ID #%d not found.\n", id);
        return;
    }

    // Display summary and ask for confirmation
    printf("\n========== COMPLAINT TO DELETE ==========\n");
    printf("ID          : %d\n", complaints[found_index].id);
    printf("User        : %s\n", complaints[found_index].user);
    printf("Category    : %s\n", complaints[found_index].category);
    printf("Description : %s\n", complaints[found_index].description);
    printf("Status      : %s\n", complaints[found_index].status);
    printf("=========================================\n");

    char confirm;
    printf("Are you sure you want to permanently delete Complaint #%d? (y/n): ", id);
    scanf(" %c", &confirm);

    if (confirm != 'y' && confirm != 'Y') {
        printf("\n[!] Deletion cancelled.\n");
        return;
    }

    // Shift array elements left to overwrite deleted item
    for (int i = found_index; i < count - 1; i++) {
        complaints[i] = complaints[i + 1];
    }
    count--;

    // Overwrite complaints.csv with updated list
    save_complaints(complaints, count);

    printf("\n[SUCCESS] Complaint #%d deleted successfully.\n", id);
}

void claim_complaint(const char *admin_name, const char *category_filter) {
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints available.\n");
        return;
    }

    int id, found = 0;
    printf("\nEnter Complaint ID to claim responsibility: ");
    if (scanf("%d", &id) != 1) {
        while (getchar() != '\n');
        printf("\n[!] Invalid input.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (complaints[i].id == id) {
            found = 1;

            // Trim potential trailing newlines on existing assignedAdmin
            complaints[i].assignedAdmin[strcspn(complaints[i].assignedAdmin, "\r\n")] = '\0';

            // Validate department scope
            if (strcmp(category_filter, "ALL") != 0 && strcasecmp(complaints[i].category, category_filter) != 0) {
                printf("\n[!] Access Denied: Complaint #%d belongs to a different department.\n", id);
                return;
            }

            // Check if already claimed
            if (strcmp(complaints[i].assignedAdmin, "Unassigned") != 0 && strcmp(complaints[i].assignedAdmin, "N/A") != 0) {
                printf("\n[!] Complaint #%d is already claimed by Admin '%s'.\n", id, complaints[i].assignedAdmin);
                return;
            }

            // Assign responsibility to logged-in admin
            strcpy(complaints[i].assignedAdmin, admin_name);
            complaints[i].assignedAdmin[strcspn(complaints[i].assignedAdmin, "\r\n")] = '\0';
            strcpy(complaints[i].status, "In Progress");

            // Check return value of save_complaints
            if (save_complaints(complaints, count)) {
                printf("\n[SUCCESS] Complaint #%d is now assigned to you (%s).\n", id, admin_name);
            } else {
                printf("\n[!] Claim failed: Could not write changes to file. Please close any open CSV files and try again.\n");
            }
            break;
        }
    }

    if (!found) printf("\nComplaint ID #%d not found.\n", id);
}

void update_complaint_status_by_admin(const char *admin_name) {
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    int id, found = 0;
    printf("\nEnter Complaint ID to update: ");
    if (scanf("%d", &id) != 1) {
        while (getchar() != '\n'); // Clear invalid input buffer
        printf("[!] Invalid ID input.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (complaints[i].id == id) {
            found = 1;

            // Trim potential trailing newlines or carriage returns
            complaints[i].assignedAdmin[strcspn(complaints[i].assignedAdmin, "\r\n")] = '\0';

            // Authorization Check:
            // Allow if admin is Super Admin ("ALL") OR if the assigned name matches (case-insensitive)
#ifdef _WIN32
            int is_authorized = (_stricmp(admin_name, "ALL") == 0) || (_stricmp(complaints[i].assignedAdmin, admin_name) == 0);
#else
            int is_authorized = (strcasecmp(admin_name, "ALL") == 0) || (strcasecmp(complaints[i].assignedAdmin, admin_name) == 0);
#endif

            if (!is_authorized) {
                printf("\n[!] Access Denied: You are not assigned to handle Complaint #%d (Assigned to: %s).\n",
                       id, complaints[i].assignedAdmin);
                return;
            }

            int status_choice;
            printf("\nSelect New Status for Complaint #%d:\n", id);
            printf("1. In Progress\n");
            printf("2. Resolved\n");
            printf("3. Closed\n");
            printf("Enter choice: ");

            if (scanf("%d", &status_choice) != 1) {
                while (getchar() != '\n');
                printf("[!] Invalid choice format.\n");
                return;
            }

            if (status_choice == 1) {
                strcpy(complaints[i].status, "In Progress");
            } else if (status_choice == 2) {
                strcpy(complaints[i].status, "Resolved");
            } else if (status_choice == 3) {
                strcpy(complaints[i].status, "Closed");
            } else {
                printf("\n[!] Invalid selection. Status not changed.\n");
                return;
            }

            // Save updated array to file
            save_complaints(complaints, count);
            printf("\n[SUCCESS] Status for Complaint #%d updated to '%s'.\n", id, complaints[i].status);

            // Notify user of status update
            char msg[256];
            snprintf(msg, sizeof(msg), "Your complaint #%d status has been updated to: %s", id, complaints[i].status);
            send_notification(complaints[i].user, id, msg);

            break;
        }
    }

    if (!found) {
        printf("\nComplaint ID #%d not found.\n", id);
    }
}

void assigning_team_admin_dashboard(const char *admin_name) {

    int choice;
    do {
        printf("\n===== ASSIGNING TEAM ADMIN DASHBOARD (%s) =====", admin_name);
        printf("\n1. View Complaints");
        printf("\n2. Assign Complaint to Team");

        printf("\n3. Delete Complaint");
        printf("\n4. Logout");
        printf("\nEnter Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewComplaintsMenu();
                break;

            case 2: {
                Complaint complaints[MAX];
                int count = 0;

                if (!load_all_complaints(complaints, &count) || count == 0) {
                    printf("\nNo complaints found to assign.\n");
                    break;
                }

                int id, found = 0;
                printf("Enter Complaint ID to assign: ");
                scanf("%d", &id);

                for (int i = 0; i < count; i++) {
                    if (complaints[i].id == id) {
                        assign_Team(complaints, count, &complaints[i]);
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    printf("\nComplaint ID %d not found.\n", id);
                }
                break;
            }



            case 3:
                deleteComplaint();
                break;

            case 4:
                printf("Logging out of Assigning Team Admin dashboard...\n");
                break;

            default:
                printf("\nInvalid Choice!");
        }
    } while (choice != 4);
}

void departmental_admin_dashboard(const char *admin_name, const char *dept_name, const char *category_filter) {


    int choice;
    do {
        printf("\n===== %s PORTAL (%s) =====", dept_name, admin_name);
        printf("\n1. View Complaints");
        printf("\n2. Claim / Take Responsibility");
        printf("\n3. Update Complaint Status");
        printf("\n4. Logout");
        printf("\nEnter Choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: viewDepartmentComplaints(category_filter); break;
            case 2: claim_complaint(admin_name, category_filter); break;
            case 3: update_complaint_status_by_admin(admin_name); break;
            case 4: printf("Exiting portal...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 4);
}

int sub_admin_login(int role_choice, char *logged_user) {
    char username[50], password[50];
    char file_username[50], file_hash[HASH_LEN];
    char input_hash[HASH_LEN];
    int file_role;

    FILE *fp = fopen(ADMIN_FILE, "r");
    if (fp == NULL) {
        printf("\nNo admins registered yet.\n");
        return 0;
    }

    printf("\nEnter Admin Username: ");
    scanf("%49s", username);
    printf("Enter Admin Password: ");
    scanf("%49s", password);

    // Convert input password to MD5 Hash for comparison
    get_user_md5(password, input_hash);

    int logged_in = 0;
    while (fscanf(fp, "%s %s %d", file_username, file_hash, &file_role) == 3) {
        if (strcmp(username, file_username) == 0 &&
            strcmp(input_hash, file_hash) == 0 &&
            file_role == role_choice) {
            logged_in = 1;
            strcpy(logged_user, username);
            break;
        }
    }
    fclose(fp);

    if (logged_in) {
        printf("\n[SUCCESS] Login successful! Welcome, %s.\n", username);
        return 1;
    } else {
        printf("\n[!] Invalid username, password, or unauthorized role.\n");
        return 0;
    }
}

void sub_admin_register(int role_choice) {
    char username[50], password[50], confirm_pass[50];
    char hashed_password[HASH_LEN];

    printf("Create Username: ");
    scanf("%49s", username);

    printf("Create Password: ");
    scanf("%49s", password);

    printf("Confirm Password: ");
    scanf("%49s", confirm_pass);

    if (strcmp(password, confirm_pass) != 0) {
        printf("\nPassword mismatch. Registration failed.\n");
        return;
    }

    get_user_md5(password, hashed_password);

    FILE *file = fopen(ADMIN_FILE, "a");
    if (!file) {
        printf("Error opening admin database file!\n");
        return;
    }

    fprintf(file, "%s %s %d\n", username, hashed_password, role_choice);
    fclose(file);

    printf("[SUCCESS] Admin registered successfully with MD5 encryption!\n");

    // Now call login
    char logged_user[50];
    printf("Please log in now:\n");
    if (sub_admin_login(role_choice, logged_user)) {
        printf("Logged in as: %s\n", logged_user);

        if (role_choice == 1) {
            assigning_team_admin_dashboard(logged_user);
        } else {
            const char *role_title;
            const char *category_filter;

            switch (role_choice) {
                case 2:
                    role_title = "Exam Office Admin";
                    category_filter = "Exam";
                    break;
                case 3:
                    role_title = "Library Admin";
                    category_filter = "Library";
                    break;
                case 4:
                    role_title = "Accounts Office Admin";
                    category_filter = "Accounts";
                    break;
                default:
                    role_title = "Admin";
                    category_filter = "ALL";
                    break;
            }

            departmental_admin_dashboard(logged_user, role_title, category_filter);
        }
    }
}

void handle_sub_admin_role(int role_choice, const char *role_title, const char *category_filter) {
    int choice;
    char logged_user[50];

    do {
        printf("\n--- %s Portal ---\n", role_title);
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Back\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                sub_admin_register(role_choice);
                break;
            case 2:
                if (sub_admin_login(role_choice, logged_user)) {
                    if (role_choice == 1) {
                        assigning_team_admin_dashboard(logged_user);
                    } else {
                        departmental_admin_dashboard(logged_user, role_title, category_filter);
                    }
                }
                break;
            case 3:
                printf("Returning to Admin menu...\n");
                break;
            default:
                printf("Invalid Choice!\n");
        }
    } while (choice != 3);
}

void AdminPortalMenu() {
    char key[50];

    printf("\n================ ADMIN PORTAL ================\n");
    printf("Enter Admin Setup Key: ");
    scanf("%49s", key);

    if (strcmp(key, General_Key) == 0) {
        handle_sub_admin_role(1, "Assigning Team Admin", "ALL");
    }
    else if (strcmp(key, ExamAdmin_Key) == 0) {
        handle_sub_admin_role(2, "Exam Office Admin", "Exam");
    }
    else if (strcmp(key, LibraryAdmin_key) == 0) {
        handle_sub_admin_role(3, "Library Admin", "Library");
    }
    else if (strcmp(key, Accounts_Key) == 0) {
        handle_sub_admin_role(4, "Accounts Office Admin", "Accounts");
    }
    else if (strcmp(key, setup_key) == 0) {
        /* Master key: let the admin pick which role portal to enter */
        int choice;
        do {
            printf("\n---- Master Key Accepted: Choose Admin Role ----\n");
            printf("1. General Admin\n");
            printf("2. Exam Office Administrator\n");
            printf("3. Library Administrator\n");
            printf("4. Accounts Office Administrator\n");
            printf("5. Back to Main Menu\n");
            printf("Enter choice: ");

            if (scanf("%d", &choice) != 1) {
                while (getchar() != '\n');
                continue;
            }

            switch (choice) {
                case 1: handle_sub_admin_role(1, "Assigning Team Admin", "ALL"); break;
                case 2: handle_sub_admin_role(2, "Exam Office Admin", "Exam"); break;
                case 3: handle_sub_admin_role(3, "Library Admin", "Library"); break;
                case 4: handle_sub_admin_role(4, "Accounts Office Admin", "Accounts"); break;
                case 5: printf("Returning to Main Menu...\n"); break;
                default: printf("Invalid choice. Please try again.\n");
            }
        } while (choice != 5);
    }
    else {
        printf("\n[!] Invalid setup key. Access denied.\n");
    }
}
