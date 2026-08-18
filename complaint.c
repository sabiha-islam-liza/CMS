#include "complaint.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

int generateComplaintID() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) return 1;

    int max_id = 0;
    int id;
    char buffer[500];
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (sscanf(buffer, "%d,", &id) == 1) {
            if (id > max_id) {
                max_id = id;
            }
        }
    }
    fclose(fp);
    return max_id + 1;
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
    strftime(c.date, sizeof(c.date), "%Y-%m-%d", &tm);

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

        // Targeted Person Notification (Only for Violence/Harassment)
        if (strcmp(c.targetUser, "N/A") != 0 && strcmp(c.targetUser, logged_in_user) != 0) {
            notify_accused_person_updated(c.targetUser, c.id, c.category);
        }

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
    }
    notify_assigned_team(c.assignedTeam, c.id, c.status);

    notify_filing_user(c.user, c.id, c.category);

    printf("\n[SUCCESS] %s complaint #%d submitted successfully.\n", c.category, c.id);
}
