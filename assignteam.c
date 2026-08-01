void notify_team(const char *teamName, int complaint_id, const char *status) {
    char message[200];
    
    // Construct the pop-up message text
    sprintf(message, "Complaint #%d has been assigned to the %s Team.\nCurrent Status: %s",
            complaint_id, teamName, status);

    // Play an alert sound and trigger a Windows dialog box
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBox(NULL, message, "Team Assignment Notification", MB_OK | MB_ICONINFORMATION);
}
void assign_Team(Complaint complaints[], int count, Complaint *c) {
    int choice;
    char teamName[50] = "";

    printf("\n------- Assign Resolution Team -------\n");
    printf("1. Technical Support Team\n");
    printf("2. Billing and Payment Team\n");
    printf("3. IT Support Team\n");
    printf("4. Security Team\n");
    printf("5. Maintenance Team\n");
    printf("6. Administration Team\n");
    printf("Enter choice (1-6): ");

    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n'); // Clear buffer on bad input
        printf("\nInvalid input! Please enter a number.\n");
        return;
    }

    switch (choice) {
        case 1: strcpy(teamName, "Technical"); break;
        case 2: strcpy(teamName, "Billing"); break;
        case 3: strcpy(teamName, "IT"); break;
        case 4: strcpy(teamName, "Security"); break;
        case 5: strcpy(teamName, "Maintenance"); break;
        case 6: strcpy(teamName, "Administration"); break;
        default:
            printf("\nInvalid choice. Team not assigned.\n");
            return;
    }

    // 1. Update in-memory complaint data
    strcpy(c->assignedTeam, teamName);
    strcpy(c->status, "In Progress");

    // 2. Save changes to main complaints.csv
    save_complaints(complaints, count);

    // 3. Append complaint record to the team's specific CSV file (e.g., Technical.csv)
    char team_filename[100];
    sprintf(team_filename, "%s.csv", teamName);

    FILE *tfp = fopen(team_filename, "a");
    if (tfp != NULL) {
        fprintf(tfp, "%d,%s,%s,%s,%s,%s,%s,%s\n",
                c->id,
                c->user,
                c->category,
                c->description,
                c->priority,
                c->status,
                c->date,
                c->assignedTeam);
        fclose(tfp);
    } else {
        printf("\nWarning: Unable to open team file '%s'\n", team_filename);
    }

    printf("\n[SUCCESS] Complaint #%d assigned to %s Team (Status: In Progress).\n", c->id, teamName);

    // 4. Trigger notification popup
    notify_team(teamName, c->id, c->status);
}
