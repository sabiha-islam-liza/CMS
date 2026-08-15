#include "notifications.h"

void send_notification(const char *target_admin, int complaint_id, const char *msg) {
    FILE *fp = fopen(NOTIF_FILE, "a");
    if (!fp) return;
    fprintf(fp, "%s|%d|%s|0\n", target_admin, complaint_id, msg);
    fclose(fp);
}

void notify_general_admin(int complaint_id, const char *category, const char *user_id) {
    char message[256];
    snprintf(message, sizeof(message), "New %s Complaint #%d filed by User: %s", category, complaint_id, user_id);
    send_notification("General Admin", complaint_id, message);

#ifdef _WIN32
    MessageBeep(MB_ICONINFORMATION);
    MessageBox(NULL, message, "General Admin Alert", MB_OK | MB_ICONINFORMATION);
#endif
}

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

void notify_grievance_committee(int complaint_id, const char *user_id) {
    char message[256];
    snprintf(message, sizeof(message), "GRIEVANCE ALERT: New Complaint/Grievance #%d filed by %s requiring review.", complaint_id, user_id);
    send_notification("Grievance Committee", complaint_id, message);

#ifdef _WIN32
    MessageBeep(MB_ICONWARNING);
    MessageBox(NULL, message, "Grievance Committee Alert", MB_OK | MB_ICONWARNING);
#endif
}

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

void notify_team(const char *teamName, int complaint_id, const char *status) {
#ifdef _WIN32
    char message[200];
    sprintf(message, "Complaint #%d assigned to %s Team.\nStatus: %s",
            complaint_id, teamName, status);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBox(NULL, message, "Team Notification", MB_OK | MB_ICONINFORMATION);
#else
    printf("\n[NOTIFICATION] Complaint #%d assigned to %s Team. Status: %s\n",
           complaint_id, teamName, status);
#endif
}
