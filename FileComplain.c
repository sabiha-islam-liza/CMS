#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX 100 // Added missing MAX definition

typedef struct {
    int id;
    char user[50];        // Complainant Registration ID
    char targetUser[50];  // Accused Registration ID (for Violence/Harassment)
    char category[50];
    char description[200];
    char priority[20];
    char status[20];
    char date[20];
    char assignedTeam[50];
} Complaint;

int load_all_complaints(Complaint complaints[], int *count) {
    FILE *fp = fopen("complaints.csv", "r");
    if (!fp) return 0;

    *count = 0;
    while (fscanf(fp, "%d,%49[^,],%49[^,],%49[^,],%199[^,],%19[^,],%19[^,],%19[^,],%49[^\n]\n",
                  &complaints[*count].id,
                  complaints[*count].user,
                  complaints[*count].targetUser,
                  complaints[*count].category,
                  complaints[*count].description,
                  complaints[*count].priority,
                  complaints[*count].status,
                  complaints[*count].date,
                  complaints[*count].assignedTeam) == 9) {
        (*count)++;
    }

    fclose(fp);
    return 1;
}

void save_complaints(Complaint complaints[], int count) {
    FILE *fp = fopen("complaints.csv", "w");
    if (!fp) {
        printf("Error opening file for writing!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%s,%s,%s,%s,%s,%s,%s,%s\n",
                complaints[i].id,
                complaints[i].user,
                complaints[i].targetUser,
                complaints[i].category,
                complaints[i].description,
                complaints[i].priority,
                complaints[i].status,
                complaints[i].date,
                complaints[i].assignedTeam);
    }

    fclose(fp);
}

void notify_accused_person(const char *accused_id, int complaint_id, const char *category) {
    char file_name[100];
    sprintf(file_name, "notice_%s.txt", accused_id);
    FILE *fp = fopen(file_name, "a");
    if (fp) {
        fprintf(fp, "\n[CONFIDENTIAL NOTICE] A %s complaint (#%d) has been filed involving your Registration ID.\n", category, complaint_id);
        fprintf(fp, "The Complaint is now under Investigation by Proctor Office\n");
        fclose(fp);
    }
}

void notify_proctor_office(int complaint_id, const char *category, const char *target_id) {
    FILE *fp = fopen("proctor_notifications.txt", "a");
    if (fp) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        
        fprintf(fp, "[ALERT %04d-%02d-%02d %02d:%02d] New %s Case (#%d) Filed.\n", 
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
                category, complaint_id);
        fprintf(fp, "Accused ID: %s | Priority: High | Status: Pending Review\n", target_id);
        fprintf(fp, "------------------------------------------------------------------\n");
        fclose(fp);
    }
}

void FileComplaint(const char *logged_in_user) {
    Complaint complaints[MAX];
    int count = 0;
    load_all_complaints(complaints, &count);

    Complaint c;
    c.id = (count == 0) ? 101 : complaints[count - 1].id + 1;
    strcpy(c.user, logged_in_user);
    strcpy(c.targetUser, "N/A"); // Default for general complaints
    strcpy(c.status, "Open");
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(c.date, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    int cat_choice;
    printf("\nSelect Complaint Category:\n");
    printf("1. Technical Issue\n");
    printf("2. Billing Issue\n");
    printf("3. Maintenance\n");
    printf("4. Violence (Targeted)\n");
    printf("5. Harassment (Targeted)\n");
    printf("6. Grievance (Academic/Admin)\n");
    printf("Enter choice: ");
    scanf("%d", &cat_choice);
    getchar(); // Clear newline buffer

    // CASE A: Violence or Harassment
    if (cat_choice == 4 || cat_choice == 5) {
        strcpy(c.category, (cat_choice == 4) ? "Violence" : "Harassment");
        strcpy(c.priority, "High");
        strcpy(c.assignedTeam, "Proctor");

        printf("\nEnter Registration ID of the person to file against: ");
        scanf("%49s", c.targetUser);
        getchar();

        printf("Enter Incidents/Details: ");
        fgets(c.description, sizeof(c.description), stdin);
        c.description[strcspn(c.description, "\n")] = '\0';

        complaints[count] = c;
        save_complaints(complaints, count + 1);

        notify_accused_person(c.targetUser, c.id, c.category);
        notify_proctor_office(c.id, c.category, c.targetUser);
        printf("\n[SUCCESS] Complaint #%d submitted anonymously to the Proctor Office.\n", c.id);
    } 
    // CASE B: Grievance
    else if (cat_choice == 6) {
        strcpy(c.category, "Grievance");
        strcpy(c.priority, "Medium");
        strcpy(c.assignedTeam, "Grievance Committee");

        printf("Enter detailed grievance description: ");
        fgets(c.description, sizeof(c.description), stdin);
        c.description[strcspn(c.description, "\n")] = '\0';

        complaints[count] = c;
        save_complaints(complaints, count + 1);

        printf("\n[SUCCESS] Grievance #%d filed and routed to the Grievance Committee.\n", c.id);
    } 
    // CASE C: Standard Categories
    else {
        if (cat_choice == 1) strcpy(c.category, "Technical");
        else if (cat_choice == 2) strcpy(c.category, "Billing");
        else strcpy(c.category, "Maintenance");

        strcpy(c.priority, "Low");
        strcpy(c.assignedTeam, "Unassigned");

        printf("Enter Description: ");
        fgets(c.description, sizeof(c.description), stdin);
        c.description[strcspn(c.description, "\n")] = '\0';

        complaints[count] = c;
        save_complaints(complaints, count + 1);

        printf("\n[SUCCESS] Complaint #%d submitted successfully.\n", c.id);
    }
}

void viewComplaints(const char *logged_in_user) {
    FILE *fp = fopen("complaints.csv", "r");
    if (fp == NULL) {
        printf("\nNo Complaints Found!\n");
        return;
    }

    Complaint c;
    char line[500];
    int found = 0;

    printf("\n================ YOUR COMPLAINTS (%s) ================\n", logged_in_user);

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d,%49[^,],%49[^,],%49[^,],%199[^,],%19[^,],%19[^,],%19[^,],%49[^\n]",
                   &c.id, c.user, c.targetUser, c.category,
                   c.description, c.priority, c.status, c.date, c.assignedTeam) == 9) {
            
            if (strcmp(c.user, logged_in_user) == 0) {
                printf("ID: %d | Category: %s | Priority: %s | Status: %s | Team: %s | Date: %s\n", 
                       c.id, c.category, c.priority, c.status, c.assignedTeam, c.date);
                printf("Description: %s\n", c.description);
                printf("------------------------------------------------------------------\n");
                found = 1;
            }
        }
    }

    if (!found) {
        printf("You have not filed any complaints yet.\n");
    }

    fclose(fp);
}

int main() {
    int choice;
    char logged_in_user[50] = "STUDENT_101"; // Simulated session user for testing

    do {
        printf("\n=== Complaint Management System ===\n");
        printf("Logged in as: %s\n", logged_in_user);
        printf("1. File Complaint\n");
        printf("2. View Complaints\n");
        printf("3. Exit\n");

        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                FileComplaint(logged_in_user);
                break;

            case 2:
                viewComplaints(logged_in_user);
                break;

            case 3:
                printf("Program Closed.\n");
                break;

            default:
                printf("Invalid Choice!\n");
        }

    } while (choice != 3);

    return 0;
}
