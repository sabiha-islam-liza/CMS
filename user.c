#include "user.h"
#include "MD5.h"
#include "complaint.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif
#define DB_FILE "user.txt"
#define ID_LEN 50
#define PASS_LEN 50
#define HASH_LEN 65.
#define RATINGS_FILE "admin_ratings.txt"
#define FILE_NAME "complaints.csv"
#define MAX 100
// 1. Notify General Admin
void notify_general_admin(int complaint_id, const char *category, const char *user_id) {
    char message[256];
    snprintf(message, sizeof(message), "New %s Complaint #%d filed by User: %s", category, complaint_id, user_id);
    send_notification("General Admin", complaint_id, message);

#ifdef _WIN32
    MessageBeep(MB_ICONINFORMATION);
    MessageBox(NULL, message, "General Admin Alert", MB_OK | MB_ICONINFORMATION);
#endif
}

// 2. Notify Assigned Team (Proctor, Special Committee, IT Support, etc.)
void notify_assigned_team(const char *teamName, int complaintID, const char *status) {

    FILE *fp = fopen("notifications.txt", "a");
    if (!fp) {
        printf("\n[!] Error: Could not open notifications.txt\n");
        return;
    }

    fprintf(fp, "%s|New complaint #%d assigned to your team (Status: %s)\n",
            teamName, complaintID, status);

    fclose(fp);
char alert_msg[256];
    snprintf(alert_msg, sizeof(alert_msg), "%s|New complaint #%d assigned to your team (Status: %s)\n",
            teamName, complaintID, status);
    send_notification(teamName, complaintID, alert_msg);

#ifdef _WIN32
    char msg[256];
    snprintf(msg, sizeof(msg), "ATTENTION: You have confidential notices or complaints filed against your Registration ID!\n\nPlease check Option 3 in your dashboard.");
    MessageBeep(MB_ICONWARNING);
    MessageBox(NULL, msg, "Confidential Notice Alert", MB_OK | MB_ICONWARNING);
#endif
}

// 3. Notify Accused Person (Targeted Warning Popup)
void notify_accused_person_updated(const char *target_user, int complaint_id, const char *category) {
    char filename[100];
    snprintf(filename, sizeof(filename), "notice_%s.txt", target_user);
    FILE *fp = fopen(filename, "a");
    if (fp) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        fprintf(fp, "[%04d-%02d-%02d] OFFICIAL NOTICE: Complaint #%d (%s) has been filed regarding your account/id.\n",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, complaint_id, category);
        fclose(fp);
    }

    char alert_msg[256];
    snprintf(alert_msg, sizeof(alert_msg), "A serious %s complaint #%d has been filed regarding your ID.", category, complaint_id);
    send_notification(target_user, complaint_id, alert_msg);

#ifdef _WIN32
    char msg[256];
    snprintf(msg, sizeof(msg), "ATTENTION: You have confidential notices or complaints filed against your Registration ID!\n\nPlease check Option 3 in your dashboard.");
    MessageBeep(MB_ICONWARNING);
    MessageBox(NULL, msg, "Confidential Notice Alert", MB_OK | MB_ICONWARNING);
#endif
}

// 4. Notify Grievance Committee
void notify_grievance_committee(int complaint_id, const char *user_id) {
    char message[256];
    snprintf(message, sizeof(message), "GRIEVANCE ALERT: New Complaint/Grievance #%d filed by %s requiring review.", complaint_id, user_id);
    send_notification("Grievance Committee", complaint_id, message);

#ifdef _WIN32
    MessageBeep(MB_ICONWARNING);
    MessageBox(NULL, message, "Grievance Committee Alert", MB_OK | MB_ICONWARNING);
#endif
}

// 5. Notify Filing User (The person who filed/accused)
void notify_filing_user(const char *user_id, int complaint_id, const char *category) {
    char message[256];
    snprintf(message, sizeof(message), "Confirmation: Your %s complaint #%d has been submitted successfully.", category, complaint_id);
    send_notification(user_id, complaint_id, message);

#ifdef _WIN32
    char title[100];
    snprintf(title, sizeof(title), "Complaint Confirmation - #%d", complaint_id);
    MessageBeep(MB_ICONINFORMATION);
    MessageBox(NULL, message, title, MB_OK | MB_ICONINFORMATION);
#endif
}
void check_user_notifications(const char *username) {
    FILE *fp = fopen("notifications.txt", "r");
    if (!fp) {
        return; // No notifications file exists yet
    }

    FILE *temp = fopen("notifications_temp.txt", "w");
    if (!temp) {
        fclose(fp);
        return;
    }

    char line[512];
    char target[50];
    int complaint_id;
    char message[256];
    int is_read;
    int unread_count = 0;

    printf("\n=========================================\n");
    printf("         UNREAD NOTIFICATIONS            \n");
    printf("=========================================\n");

    while (fgets(line, sizeof(line), fp)) {
        // Parse line format: target_user|complaint_id|message|is_read
        if (sscanf(line, "%49[^|]|%d|%255[^|]|%d\n", target, &complaint_id, message, &is_read) == 4) {
            // Check if notification belongs to current user/role AND is unread (0)
            if (strcmp(target, username) == 0 && is_read == 0) {
                unread_count++;
                printf("[%d] Complaint #%d: %s\n", unread_count, complaint_id, message);

                // Mark as read (1) when copying back
                fprintf(temp, "%s|%d|%s|1\n", target, complaint_id, message);
            } else {
                // Keep unchanged for other users or already read items
                fprintf(temp, "%s|%d|%s|%d\n", target, complaint_id, message, is_read);
            }
        }
    }

    if (unread_count == 0) {
        printf("No new notifications.\n");
    }
    printf("=========================================\n\n");

    fclose(fp);
    fclose(temp);

    // Update notifications.txt with updated read flags
    remove("notifications.txt");
    rename("notifications_temp.txt", "notifications.txt");
}
void check_and_alert_accused(const char *username) {
    char filename[100];
    snprintf(filename, sizeof(filename), "notice_%s.txt", username);

    FILE *fp = fopen(filename, "r");
    if (fp) {
        printf("\n=========================================\n");
        printf("  [!] CONFIDENTIAL LEGAL/NOTICE ALERT    \n");
        printf("=========================================\n");

        char line[256];
        while (fgets(line, sizeof(line), fp)) {
            printf("%s", line);
        }
        printf("=========================================\n\n");
        fclose(fp);

        #ifdef _WIN32
            MessageBeep(MB_ICONWARNING);
            MessageBox(NULL,
                       "ATTENTION: You have confidential notices or complaints filed regarding your account!",
                       "Confidential Notice Alert",
                       MB_OK | MB_ICONWARNING);
        #endif
    }
}

// Function to directly apply for a grievance without requiring target ID input
void apply_for_grievance(const char *logged_in_user) {
    // Step 1: Check punishments.txt
    FILE *pf = fopen("punishments.txt", "r");
    if (!pf) {
        printf("\n[!] Punishment records not found. Cannot apply for grievance.\n");
        return;
    }

    int has_punishment = 0;
    char line[256], punished_user[50], reason[200];
    while (fgets(line, sizeof(line), pf)) {
        // Example format in punishments.txt: username,reason
        if (sscanf(line, "%49[^,],%199[^\n]", punished_user, reason) == 2) {
            if (strcmp(punished_user, logged_in_user) == 0) {
                has_punishment = 1;
                break;
            }
        }
    }
    fclose(pf);

    if (!has_punishment) {
        printf("\n[!] You have no punishment record. Only punished users can apply for grievance.\n");
        return;
    }

    // Step 2: Load existing complaints
    Complaint complaints[MAX];
    int count = 0;
    load_all_complaints(complaints, &count);

    if (count >= MAX) {
        printf("\n[!] System storage is full. Cannot submit grievance.\n");
        return;
    }

    // Step 3: Create new grievance complaint
    Complaint c;
    c.id = (count == 0) ? 101 : complaints[count - 1].id + 1;
    strcpy(c.user, logged_in_user);
    strcpy(c.targetUser, "N/A");
    strcpy(c.category, "Grievance");
    strcpy(c.priority, "Medium");
    strcpy(c.status, "Open");
    strcpy(c.assignedTeam, "Grievance Committee");
    strcpy(c.assignedAdmin, "Unassigned");
    c.rating = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(c.date, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    while (getchar() != '\n'); // clear buffer
    printf("\n================ APPLY FOR GRIEVANCE ================\n");
    printf("Enter detailed grievance description (Academic / Administrative): ");
    fgets(c.description, sizeof(c.description), stdin);
    c.description[strcspn(c.description, "\n")] = '\0';

    // Step 4: Save complaint
    complaints[count] = c;
    save_complaints(complaints, count + 1);

    // Step 5: Notifications
    char admin_notif[256];
    snprintf(admin_notif, sizeof(admin_notif), "New Grievance #%d filed by %s.", c.id, c.user);
    send_notification("General Admin", c.id, admin_notif);

    char committee_notif[256];
    snprintf(committee_notif, sizeof(committee_notif),
             "GRIEVANCE ALERT: New Grievance #%d filed by %s requiring review.", c.id, c.user);
    send_notification("Grievance Committee", c.id, committee_notif);

    printf("\n[SUCCESS] Grievance #%d submitted successfully and routed to Grievance Committee.\n", c.id);
}

void FileComplaint(const char *logged_in_user) {
    Complaint complaints[MAX];
    int count = 0;
    load_all_complaints(complaints, &count);

    if (count >= MAX) {
        printf("\n[!] Error: System storage full. Cannot file more complaints.\n");
        return;
    }

    Complaint c;
    c.id = (count == 0) ? 101 : complaints[count - 1].id + 1;

    strcpy(c.user, logged_in_user);
    strcpy(c.targetUser, "N/A");
    strcpy(c.status, "Open");
    strcpy(c.assignedAdmin, "Unassigned");
    c.rating = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(c.date, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    int cat_choice;
    printf("\nSelect Complaint Category:\n");
    printf("1. Technical Issue\n");
    printf("2. Accounts\n");
    printf("3. Maintenance\n");
    printf("4. Library Problem\n");
    printf("5. Exam Related Issue\n");
    printf("6. Violence (Targeted)\n");
    printf("7. Sexual Harassment (Targeted)\n");
    printf("8. Grievance (Academic/Admin)\n");
    printf("Enter choice: ");

    if (scanf("%d", &cat_choice) != 1) {
        while (getchar() != '\n');
        printf("\n[!] Invalid choice. Returning to menu.\n");
        return;
    }
    while (getchar() != '\n');

    /* FIX: Handle grievance option early and cleanly */
    if (cat_choice == 8) {
        apply_for_grievance(logged_in_user);
        return;
    }

    if (cat_choice == 6 || cat_choice == 7) {
        if (cat_choice == 6) {
            strcpy(c.category, "Violence");
            strcpy(c.assignedTeam, "Proctor");
        } else {
            strcpy(c.category, "Sexual Harassment");
            strcpy(c.assignedTeam, "SpecialCommittee");
        }

        strcpy(c.priority, "High");

        printf("\nEnter Registration ID of the person to file against: ");
        scanf("%49s", c.targetUser);
        while (getchar() != '\n');

        printf("Enter Incidents / Details: ");
        fgets(c.description, sizeof(c.description), stdin);
        c.description[strcspn(c.description, "\n")] = '\0';

        complaints[count] = c;
        save_complaints(complaints, count + 1);

        if (strcmp(c.targetUser, "N/A") != 0 && strcmp(c.targetUser, logged_in_user) != 0) {
            notify_accused_person_updated(c.targetUser, c.id, c.category);
        }
        notify_assigned_team(c.assignedTeam, c.id, c.status);
    } else {
        if (cat_choice == 1) {
            strcpy(c.category, "Technical");
            strcpy(c.assignedTeam, "IT Support");
            strcpy(c.priority, "Low");
        } else if (cat_choice == 2) {
            strcpy(c.category, "Accounts");
            strcpy(c.assignedTeam, "Accounts Admin");
            strcpy(c.priority, "Medium");
        } else if (cat_choice == 3) {
            strcpy(c.category, "Maintenance");
            strcpy(c.assignedTeam, "Maintenance Team");
            strcpy(c.priority, "Low");
        } else if (cat_choice == 4) {
            strcpy(c.category, "Library");
            strcpy(c.assignedTeam, "Library Admin");
            strcpy(c.priority, "Low");
        } else if (cat_choice == 5) {
            strcpy(c.category, "Exam");
            strcpy(c.assignedTeam, "Exam Head");
            strcpy(c.priority, "High");
        } else {
            printf("\n[!] Invalid menu choice.\n");
            return;
        }

        printf("Enter Description: ");
        fgets(c.description, sizeof(c.description), stdin);
        c.description[strcspn(c.description, "\n")] = '\0';

        complaints[count] = c;
        save_complaints(complaints, count + 1);

        notify_general_admin(c.id, c.category, c.user);
        notify_assigned_team(c.assignedTeam, c.id, c.status);
    }

    printf("\n[SUCCESS] %s complaint #%d submitted successfully.\n", c.category, c.id);
}
// Check if Registration ID exists in user database
int is_id_registered(const char *user_id) {
    FILE *fp = fopen(DB_FILE, "r");
    if (!fp) return 0; // File doesn't exist yet

    char file_id[ID_LEN], file_hash[HASH_LEN];
    int flag;

    while (fscanf(fp, "%s %s %d", file_id, file_hash, &flag) == 3) {
        if (strcmp(file_id, user_id) == 0) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}


// Helper function to convert text to MD5 hex string
void get_user_md5(const char *input, char *output_hash) {
    unsigned char digest[16];
    MD5_CTX_INLINE ctx;

    MD5Init_Inline(&ctx);
    MD5Update_Inline(&ctx, (const unsigned char*)input, strlen(input));
    MD5Final_Inline(digest, &ctx);

    for (int i = 0; i < 16; i++) {
        sprintf(output_hash + (i * 2), "%02x", digest[i]);
    }
    output_hash[32] = '\0';
}

// Register a new user with default password set as their Registration ID
void user_register() {
    char user_id[ID_LEN];
    char hashed_default_pass[HASH_LEN];

    printf("\n================ REGISTER =================\n");
    printf("Enter your Registration ID (e.g., 252-35-485): ");

    if (scanf("%49s", user_id) != 1) {
        printf("\n[!] Invalid input. Returning to menu...\n");
        while (getchar() != '\n'); // Clear input buffer
        return; // Returns back to User menu
    }

    // Check if the Registration ID already exists in DB_FILE
    if (is_id_registered(user_id)) {
        printf("\n[!] Error: Registration ID '%s' is already registered!\n", user_id);
        printf("Returning to menu...\n");
        return; // Returns back to User menu
    }

    // Hash the default password (set to Registration ID)
    get_user_md5(user_id, hashed_default_pass);

    FILE *file = fopen(DB_FILE, "a");
    if (!file) {
        printf("\n[!] Error: Unable to access user database file.\n");
        printf("Returning to menu...\n");
        return; // Returns back to User menu
    }

    // Save record format: USER_ID MD5_HASH FIRST_LOGIN_FLAG (1)
    fprintf(file, "%s %s 1\n", user_id, hashed_default_pass);
    fclose(file);

    printf("\n[SUCCESS] User registered successfully!\n");
    printf("Your initial password is set to: %s\n", user_id);
    printf("Note: You will be required to change your password on first login.\n");
    printf("===========================================\n");

    printf("\nRegistration complete. Returning to main menu...\n");
    return; // Returns back to User menu
}
// Enforce new password on first login
void force_password_reset(const char *user_id) {
    char new_pass[PASS_LEN], confirm_pass[PASS_LEN];
    char new_hash[HASH_LEN];


    printf("\n[SECURITY NOTICE] First Login Detected!!!");
    printf("\nYou must set a new private password before continuing.");


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

    // Generate MD5 hash for the new password
    get_user_md5(new_pass, new_hash);

    FILE *fp = fopen(DB_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("\nError updating security credentials.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    char file_id[ID_LEN], file_hash[HASH_LEN];
    int first_login_flag;

    while (fscanf(fp, "%s %s %d", file_id, file_hash, &first_login_flag) == 3) {
        if (strcmp(file_id, user_id) == 0) {
            // Update hash and set first_login_flag to 0
            fprintf(temp, "%s %s 0\n", file_id, new_hash);
        } else {
            fprintf(temp, "%s %s %d\n", file_id, file_hash, first_login_flag);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(DB_FILE);
    rename("temp.txt", DB_FILE);

    printf("\n[SUCCESS] Password updated successfully!");
    printf("\nPlease use your new password for all future logins.\n");
}

#define PUNISHMENT_FILE "punishments.txt"

void view_user_punishments(const char *logged_in_user) {
    FILE *fp = fopen(PUNISHMENT_FILE, "r");
    if (!fp) {
        printf("\n================ OFFICIAL RESOLUTION & DISCIPLINARY NOTICES ================");
        printf("\nNo formal notices or punishments found.\n");
        printf("============================================================================\n");
        return;
    }

    typedef struct {
        int complaint_id;
        char target_user[50];
        char action_type[50];   // e.g., "Warning", "Account Suspension", "Fine", "Proctorial Inquiry"
        char reason[256];
        char date[20];
    } PunishmentRecord;

    PunishmentRecord record;
    int found = 0;

    printf("\n================ OFFICIAL RESOLUTION & DISCIPLINARY NOTICES ================\n");

    // File Format: ComplaintID|TargetUser|ActionType|Reason|Date
    while (fscanf(fp, "%d|%49[^|]|%49[^|]|%255[^|]|%19[^\n]\n",
                  &record.complaint_id,
                  record.target_user,
                  record.action_type,
                  record.reason,
                  record.date) == 5) {

        if (strcmp(record.target_user, logged_in_user) == 0) {
            found++;
            printf("\n Notice #%d", found);
            printf("\n ----------------------------------------------------");
            printf("\n Date Issued   : %s", record.date);
            printf("\n Complaint ID  : #%d", record.complaint_id);
            printf("\n Action Taken  : %s", record.action_type);
            printf("\n Details/Reason: %s", record.reason);
            printf("\n ----------------------------------------------------\n");
        }
    }

    if (found == 0) {
        printf("No disciplinary actions or warnings have been recorded for account: %s\n", logged_in_user);
    } else {
        printf("Total Active Notices: %d\n", found);
    }

    printf("============================================================================\n");
    fclose(fp);
}

void update_admin_rating_record(const char *admin_id, int new_rating) {
    if (admin_id == NULL || strlen(admin_id) == 0 ||
        strcmp(admin_id, "Unassigned") == 0 || strcmp(admin_id, "N/A") == 0) {
        return;
    }

    // Step 1: Look up the assigned admin's category/role from admin.txt
    char category[50] = "General";
    FILE *admin_fp = fopen(ADMIN_FILE, "r");
    if (admin_fp) {
        char name[50], pass[HASH_LEN];
        int role;
        while (fscanf(admin_fp, "%49s %64s %d", name, pass, &role) >= 3) {
            if (strcmp(name, admin_id) == 0) {
                switch (role) {
                    case 1: strcpy(category, "General Admin"); break;
                    case 2: strcpy(category, "Exam Office"); break;
                    case 3: strcpy(category, "Library Admin"); break;
                    case 4: strcpy(category, "Accounts Office"); break;
                    default: strcpy(category, "General Admin"); break;
                }
                break;
            }
        }
        fclose(admin_fp);
    }

    // Step 2: Read existing records into temporary memory array
    typedef struct {
        char admin_name[50];
        char category[50];
        int total_sum;
        int rating_count;
        float avg_rating;
    } AdminRating;

    AdminRating records[MAX];
    int record_count = 0;
    int found = 0;

    FILE *rfp = fopen(RATINGS_FILE, "r");
    if (rfp) {
        // Parsing format: AdminName|Category|TotalSum|Count|AvgRating
        while (fscanf(rfp, "%49[^|]|%49[^|]|%d|%d|%f\n",
                      records[record_count].admin_name,
                      records[record_count].category,
                      &records[record_count].total_sum,
                      &records[record_count].rating_count,
                      &records[record_count].avg_rating) == 5) {

            if (strcmp(records[record_count].admin_name, admin_id) == 0) {
                records[record_count].total_sum += new_rating;
                records[record_count].rating_count += 1;
                records[record_count].avg_rating = (float)records[record_count].total_sum / records[record_count].rating_count;
                strcpy(records[record_count].category, category);
                found = 1;
            }
            record_count++;
        }
        fclose(rfp);
    }

    // Step 3: Append as a new record if this is the admin's first rating
    if (!found && record_count < MAX) {
        strcpy(records[record_count].admin_name, admin_id);
        strcpy(records[record_count].category, category);
        records[record_count].total_sum = new_rating;
        records[record_count].rating_count = 1;
        records[record_count].avg_rating = (float)new_rating;
        record_count++;
    }

    // Step 4: Overwrite admin_ratings.txt with the updated data
    rfp = fopen(RATINGS_FILE, "w");
    if (!rfp) {
        printf("\n[!] Error opening ratings file for writing.\n");
        return;
    }

    for (int i = 0; i < record_count; i++) {
        fprintf(rfp, "%s|%s|%d|%d|%.2f\n",
                records[i].admin_name,
                records[i].category,
                records[i].total_sum,
                records[i].rating_count,
                records[i].avg_rating);
    }
    fclose(rfp);
}
void rate_resolved_complaint(const char *logged_in_user) {
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints available.\n");
        return;
    }

    int eligible = 0;
    printf("\n========== RATE RESOLVED SERVICES ==========\n");
    for (int i = 0; i < count; i++) {
       // Simplify the rating eligibility check to only target Closed complaints:
if (strcmp(complaints[i].user, logged_in_user) == 0 &&
    strcasecmp(complaints[i].status, "Closed") == 0)
    // Allow user rating
 {
            printf("ID: %d | Category: %s | Handled By Admin: %s | Current Rating: %d/5\n",
                   complaints[i].id, complaints[i].category, complaints[i].assignedAdmin, complaints[i].rating);
            eligible++;
        }
    }

    if (eligible == 0) {
        printf("No resolved or closed complaints available for rating.\n");
        return;
    }

    int target_id;
    printf("\nEnter Complaint ID to rate: ");
    if (scanf("%d", &target_id) != 1) {
        while (getchar() != '\n');
        return;
    }

    for (int i = 0; i < count; i++) {
        if (complaints[i].id == target_id && strcmp(complaints[i].user, logged_in_user) == 0) {
            int rating_val;
            printf("Enter Rating for Admin '%s' (1 to 5): ", complaints[i].assignedAdmin);
            scanf("%d", &rating_val);

            if (rating_val < 1 || rating_val > 5) {
                printf("[!] Invalid rating. Must be between 1 and 5.\n");
                return;
            }

            // Save rating into the complaint ticket (complaints.csv)
            complaints[i].rating = rating_val;
            save_complaints(complaints, count);

            // Update cumulative totals and averages in admin_ratings.txt
            update_admin_rating_record(complaints[i].assignedAdmin, rating_val);

            printf("[SUCCESS] Rating %d/5 saved. Thank you for your feedback!\n", rating_val);
            return;
        }
    }

    printf("[!] Complaint ID not found or access denied.\n");
}

#ifdef _WIN32
#include <windows.h>
#endif

// Helper function to set terminal text color via Windows API
void set_win_color(unsigned short color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
#endif
}

// 1. viewComplaints: Displays complaints filed BY the user in standard formatted console output
void viewComplaints(const char *logged_in_user) {
    Complaint complaints[MAX];
    int count = 0;

    // Reset color
    set_win_color(7);

    if (!load_all_complaints(complaints, &count) || count == 0) {
        set_win_color(12); // Red text
        printf("\n[!] No complaints found in the system.\n");
        set_win_color(7);
        return;
    }

    int found = 0;

    set_win_color(11); // Light Cyan
    printf("\n========================================================");
    printf("\n             MY FILED COMPLAINTS LIST                   ");
    printf("\n========================================================\n");
    set_win_color(7);

    for (int i = 0; i < count; i++) {
        if (strcmp(complaints[i].user, logged_in_user) == 0) {
            found++;

            set_win_color(14); // Yellow header
            printf("\n--- [ Complaint ID: #%d ] ---", complaints[i].id);
            set_win_color(7);

            printf("\n Category      : %s", complaints[i].category);
            printf("\n Priority      : %s", complaints[i].priority);

            // Highlight status based on resolution state
            printf("\n Status        : ");
            if (strcmp(complaints[i].status, "Resolved") == 0) set_win_color(10); // Green
            else set_win_color(12); // Red/Pending
            printf("%s", complaints[i].status);
            set_win_color(7);

            printf("\n Assigned Admin: %s", complaints[i].assignedAdmin);
            printf("\n Rating        : %d/5", complaints[i].rating);
            printf("\n Date Filed    : %s", complaints[i].date);
            printf("\n Description   : %s\n", complaints[i].description);
        }
    }

    if (found == 0) {
        set_win_color(12);
        printf("\n[!] No complaints filed yet by account: %s\n", logged_in_user);
        set_win_color(7);
    } else {
        set_win_color(11);
        printf("========================================================\n");
        set_win_color(7);
    }
}

// 2. viewAccusedComplaints: Displays complaints/notices filed AGAINST the user in formatted console output
void viewAccusedComplaints(const char *logged_in_user) {
    char file_name[100];
    sprintf(file_name, "notice_%s.txt", logged_in_user);
    FILE *fp = fopen(file_name, "r");

    if (fp == NULL) {
        set_win_color(10); // Green
        printf("\n========================================================");
        printf("\n [INFO] No official notices or targeted complaints against ID: %s", logged_in_user);
        printf("\n========================================================\n");
        set_win_color(7);
        return;
    }

    set_win_color(12); // Red border for warnings/accused complaints
    printf("\n========================================================");
    printf("\n           CONFIDENTIAL NOTICES / ACCUSATIONS          ");
    printf("\n========================================================\n");
    set_win_color(7);

    char line[256];
    int line_num = 1;
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf(" %d. %s", line_num++, line);
    }
    fclose(fp);

    set_win_color(12);
    printf("========================================================\n");
    set_win_color(7);
}

// Updated User Dashboard without color codes
void user_dashboard(const char *username) {
    // 1. Checks and displays unread alerts sent to this specific user ID
    check_user_notifications(username);

    // 2. Checks if file notices (notice_<username>.txt) exist for the user
    check_and_alert_accused(username);
    int choice;
    do {
        printf("\n===== USER DASHBOARD (%s) =====", username);
        printf("\n1. File a New Complaint");
        printf("\n2. View My Complaint Statuses");
        printf("\n3. Rate Resolved Complaints");
        printf("\n4. View Accused Complaints");
        printf("\n5. View My Punishments");
        printf("\n6. Apply For Grievance");
        printf("\n7. Logout");
        printf("\nEnter Choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
               FileComplaint(username);
                break;
            case 2:
                viewComplaints(username);
                break;

            case 3:
                rate_resolved_complaint(username);
                break;
            case 4:
               viewAccusedComplaints(username);
                break;
            case 5:
                 view_user_punishments(username);
                break;
            case 6:
                apply_for_grievance(username);
                break;
            case 7:
                printf("Logging out...\n");
                break;
            default:
                printf("\nInvalid choice!\n");
        }
    } while (choice != 7);
}
void user_login() {
    char input_id[ID_LEN], input_pass[PASS_LEN];
    char file_id[ID_LEN], file_hash[HASH_LEN];
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

    get_user_md5(input_pass, input_hash);

    int login_success = 0;
    while (fscanf(fp, "%s %s %d", file_id, file_hash, &first_login_flag) == 3) {
        if (strcmp(input_id, file_id) == 0 && strcmp(input_hash, file_hash) == 0) {
            login_success = 1;
            break;
        }
    }
    fclose(fp);

    if (!login_success) {
        printf("\nInvalid Registration ID or password. Returning to menu...\n");
        return;
    }

    printf("\nCredentials verified. Welcome, %s!\n", input_id);

    /* FIX: Force password reset if flag is 1 */
    if (first_login_flag == 1) {
        force_password_reset(input_id);
    }

    /* FIX: Launch dashboard after login */
    user_dashboard(input_id);
}

void User()
{
    int choice;

    do {
        printf("\n================ USER PORTAL ================\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear input buffer on non-numeric input
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice)
        {
            case 1:
                user_register(); // Goes to register, then comes back here
                break;
            case 2:
                user_login();
                break;
            case 3:
                printf("Returning to Main Menu...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 3);
}
