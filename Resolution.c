#include "Resolution.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>


#ifdef _WIN32
#include <windows.h>
#endif
void teamRegister()
{
    char team[50];
    char username[50];
    char password[50];
    char confirm[50];
    char hash[HASH_LEN];

    int choice;

    printf("\n========== RESOLUTION TEAM REGISTRATION ==========\n");

    printf("1. IT Team\n");
    printf("2. Maintenance Team\n");
    printf("3. Proctor Office\n");
    printf("4. Grievance Committee\n");
    printf("5. Student Discipline Committee\n");
    printf("6. Special Committee (Sexual Harassment)\n");

    printf("Enter team choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            strcpy(team, "IT");
            break;

        case 2:
            strcpy(team, "Maintenance");
            break;

        case 3:
            strcpy(team, "Proctor");
            break;

        case 4:
            strcpy(team, "Grievance");
            break;

        case 5:
            strcpy(team, "StudentDiscipline");
            break;

        case 6:
            strcpy(team, "SpecialCommittee");
            break;

        default:
            printf("Invalid choice!\n");
            return;
    }

    printf("Create Username: ");
    scanf("%49s", username);

    printf("Create Password: ");
    scanf("%49s", password);

    printf("Confirm Password: ");
    scanf("%49s", confirm);

    if(strcmp(password, confirm) != 0)
    {
        printf("\nPassword did not match!\n");
        return;
    }

    /* Use the MD5 function you already have */
    get_user_md5(password, hash);

    FILE *fp = fopen("resolution_team.txt", "a");

    if(fp == NULL)
    {
        printf("\nError opening resolution_team.txt\n");
        return;
    }

    /*
       Stored format:

       Team Username MD5_Hash
    */

    fprintf(fp, "%s %s %s\n", team, username, hash);

    fclose(fp);

    printf("\nResolution team registered successfully!\n");
    printf("Password has been stored using MD5.\n");
}


/* ---------- Resolution Team Login ---------- */

void resolutionLogin()
{
    char username[50];
    char password[50];

    char fileTeam[50];
    char fileUsername[50];
    char fileHash[HASH_LEN];

    char inputHash[HASH_LEN];

    int found = 0;

    printf("\n========== RESOLUTION TEAM LOGIN ==========\n");

    printf("Username: ");
    scanf("%49s", username);

    printf("Password: ");
    scanf("%49s", password);

    /* Convert entered password into MD5 */
    get_user_md5(password, inputHash);

    FILE *fp = fopen("resolution_team.txt", "r");

    if(fp == NULL)
    {
        printf("\nNo resolution team account found.\n");
        printf("Please register a team first.\n");
        return;
    }

    char line[256];

    while (fgets(line, sizeof(line), fp))
    {
        char team[50];
        char savedUsername[50];
        char savedHash[HASH_LEN];

        /* Normal format: Team Username Hash */
        if (sscanf(line, "%49s %49s %64s",
                   team, savedUsername, savedHash) == 3)
        {
            if (strcmp(username, savedUsername) == 0 &&
                strcmp(inputHash, savedHash) == 0)
            {
                strcpy(fileTeam, team);
                strcpy(fileUsername, savedUsername);
                strcpy(fileHash, savedHash);
                found = 1;
                break;
            }
        }

        /*
           Backward compatibility for an older account stored as:
           Special Committee Username Hash
        */
        char oldUser[50];
        char oldHash[HASH_LEN];

        if (sscanf(line, "Special Committee %49s %64s",
                   oldUser, oldHash) == 2)
        {
            if (strcmp(username, oldUser) == 0 &&
                strcmp(inputHash, oldHash) == 0)
            {
                strcpy(fileTeam, "SpecialCommittee");
                strcpy(fileUsername, oldUser);
                strcpy(fileHash, oldHash);
                found = 1;
                break;
            }
        }
    }

    fclose(fp);

    if(!found)
    {
        printf("\nInvalid username or password.\n");
        return;
    }

    printf("\nLogin successful!\n");
    printf("Welcome to %s.\n", fileTeam);

    if(strcmp(fileTeam, "IT") == 0)
    {
        ITMenu(fileUsername);
    }
    else if(strcmp(fileTeam, "Maintenance") == 0)
    {
        MaintenanceMenu(fileUsername);
    }
    else if(strcmp(fileTeam, "Proctor") == 0)
    {
        ProctorMenu();
    }
    else if(strcmp(fileTeam, "Grievance") == 0)
    {
        GrievanceMenu();
    }
    else if(strcmp(fileTeam, "StudentDiscipline") == 0)
    {
        StudentDisciplineMenu();
    }
    else if(strcmp(fileTeam, "SpecialCommittee") == 0)
    {
        special_committee_dashboard();
    }
}


/* =========================================================
                         IT TEAM
   ========================================================= */

void ITMenu(const char *admin_name)
{
    int choice;

    do
    {
        printf("\n========== IT SUPPORT TEAM ==========\n");
        printf("1. View Complaints\n");
        printf("2. Update Status\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                viewComplaintsByAssignedTeam("IT Support");
                break;

            case 2:
                updateStatus("IT Support");
                break;

            case 3:
                printf("Logging out from IT Team...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 3);
}


/* =========================================================
                     MAINTENANCE TEAM
   ========================================================= */

void MaintenanceMenu(const char *admin_name)
{
    int choice;

    do
    {
        printf("\n========== MAINTENANCE TEAM ==========\n");
        printf("1. View Complaints\n");
        printf("2. Update Status\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                viewComplaintsByAssignedTeam("Maintenance Team");
                break;

            case 2:
                updateStatus("Maintenance Team");
                break;

            case 3:
                printf("Logging out from Maintenance Team...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 3);
}


/* =========================================================
                       PROCTOR OFFICE
   ========================================================= */

void ProctorMenu()
{
    int choice;

    do
    {
        printf("\n========== PROCTOR OFFICE ==========\n");
        printf("1. View Complaints\n");
        printf("2. Give Suggestion\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                viewComplaintsByAssignedTeam("Proctor");

                break;

            case 2:
                giveProctorSuggestion();
                break;

            case 3:
                printf("Logging out from Proctor Office...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 3);
}


/* ---------- Proctor Gives Suggestion ---------- */

void giveProctorSuggestion()
{
    int complaintID;
    int choice;

    const char *suggestion;

    printf("\n========== GIVE PROCTOR SUGGESTION ==========\n");

    printf("Enter Complaint ID: ");
    scanf("%d", &complaintID);

    printf("\nSelect Suggestion:\n");
    printf("1. Fine\n");
    printf("2. Cancel Waiver\n");
    printf("3. Rusticate\n");
    printf("4. Call Parents\n");
    printf("5. Investigation\n");
    printf("6. Send Matter to Head Teacher\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            suggestion = "Fine";
            break;

        case 2:
            suggestion = "Cancel Waiver";
            break;

        case 3:
            suggestion = "Rusticate";
            break;

        case 4:
            suggestion = "Call Parents";
            break;

        case 5:
            suggestion = "Investigation";
            break;

        case 6:
            suggestion = "Send Matter to Head Teacher";
            break;

        default:
            printf("Invalid choice!\n");
            return;
    }

    FILE *fp = fopen("suggestion.txt", "a");

    if(fp == NULL)
    {
        printf("\nError opening suggestion.txt\n");
        return;
    }

    fprintf(fp,
            "Complaint ID: %d | Proctor Suggestion: %s\n",
            complaintID,
            suggestion);

    fclose(fp);

    printf("\nSuggestion saved successfully!\n");
    printf("The suggestion will be reviewed by the Student Discipline Committee.\n");
}


/* =========================================================
                   GRIEVANCE COMMITTEE
   ========================================================= */

void GrievanceMenu()
{
    int choice;

    do
    {
        printf("\n========== GRIEVANCE COMMITTEE ==========\n");
        printf("1. View Grievances\n");
        printf("2. Recall\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                viewGrievances();
                break;

            case 2:
                grievanceRecall();
                break;

            case 3:
                printf("Logging out from Grievance Committee...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 3);
}


/* ---------- View Grievances Directly From complaints.csv ---------- */

void viewGrievances()
{
    Complaint complaints[MAX];
    int count = 0;
    int found = 0;

    if(!load_all_complaints(complaints, &count) || count == 0)
    {
        printf("\nNo complaints found.\n");
        return;
    }

    printf("\n========== GRIEVANCES ==========\n");

    for(int i = 0; i < count; i++)
    {
        if(strcmp(complaints[i].category, "Grievance") == 0)
        {
            printComplaint(complaints[i]);
            found = 1;
        }
    }

    if(!found)
    {
        printf("\nNo grievances found.\n");
    }
}


/* ---------- Grievance Recall ---------- */

void grievanceRecall()
{
    int complaintID;
    int choice;

    const char *action;

    printf("\n========== GRIEVANCE RECALL ==========\n");

    printf("Enter Complaint ID: ");
    scanf("%d", &complaintID);

    printf("\nSelect Action:\n");
    printf("1. Fine\n");
    printf("2. Remove Waiver\n");
    printf("3. Rusticate\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            action = "Fine";
            break;

        case 2:
            action = "Remove Waiver";
            break;

        case 3:
            action = "Rusticate";
            break;

        default:
            printf("Invalid choice!\n");
            return;
    }

    FILE *fp = fopen("suggestion.txt", "a");

    if(fp == NULL)
    {
        printf("\nError opening suggestion.txt\n");
        return;
    }

    fprintf(fp,
            "Grievance Complaint ID: %d | Committee Action: %s\n",
            complaintID,
            action);

    fclose(fp);

    printf("\nAction recorded successfully!\n");
}


/* =========================================================
               STUDENT DISCIPLINE COMMITTEE
   ========================================================= */

void StudentDisciplineMenu()
{
    int choice;

    do
    {
        printf("\n========== STUDENT DISCIPLINE COMMITTEE ==========\n");
        printf("1. View Proctor Suggestions\n");
        printf("2. Implement Punishment\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                viewSuggestions();
                break;

            case 2:
                implementPunishment();
                break;

            case 3:
                printf("Logging out from Student Discipline Committee...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 3);
}


/* ---------- View Proctor Suggestions ---------- */

void viewSuggestions()
{
    FILE *fp = fopen("suggestion.txt", "r");

    if(fp == NULL)
    {
        printf("\nNo suggestions found.\n");
        return;
    }

    Complaint complaints[MAX];
    int count = 0;

    if(!load_all_complaints(complaints, &count))
    {
        printf("\nUnable to load complaints.\n");
        fclose(fp);
        return;
    }

    char line[300];
    int complaintID;
    int found;

    printf("\n========== PROCTOR SUGGESTIONS ==========\n");

    while(fgets(line, sizeof(line), fp))
    {
        /*
           Check whether this is a Proctor suggestion
           or a grievance committee action.
        */

        if(sscanf(line,
                  "Complaint ID: %d",
                  &complaintID) == 1)
        {
            found = 0;

            for(int i = 0; i < count; i++)
            {
                if(complaints[i].id == complaintID)
                {
                    printComplaint(complaints[i]);
                    found = 1;
                    break;
                }
            }

            if(found)
            {
                printf("Suggestion/Action: %s", line);
                printf("--------------------------------\n");
            }
            else
            {
                printf("\nComplaint #%d not found.\n", complaintID);
                printf("Suggestion/Action: %s", line);
            }
        }
        else if(sscanf(line,
                       "Grievance Complaint ID: %d",
                       &complaintID) == 1)
        {
            found = 0;

            for(int i = 0; i < count; i++)
            {
                if(complaints[i].id == complaintID)
                {
                    printComplaint(complaints[i]);
                    found = 1;
                    break;
                }
            }

            if(found)
            {
                printf("Committee Action: %s", line);
                printf("--------------------------------\n");
            }
            else
            {
                printf("\nComplaint #%d not found.\n", complaintID);
                printf("Committee Action: %s", line);
            }
        }
        else
        {
            printf("%s", line);
        }
    }

    fclose(fp);
}


/* ---------- Student Discipline Implements Final Punishment ---------- */

void implementPunishment()
{
    int complaintID;
    int choice;

    const char *punishment;

    printf("\n========== IMPLEMENT FINAL PUNISHMENT ==========\n");

    printf("Enter Complaint ID: ");
    scanf("%d", &complaintID);

    printf("\nSelect Final Punishment:\n");
    printf("1. Fine\n");
    printf("2. Cancel Waiver\n");
    printf("3. Rusticate\n");
    printf("4. Call Parents\n");
    printf("5. Investigation\n");
    printf("6. Send Matter to Head Teacher\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            punishment = "Fine";
            break;

        case 2:
            punishment = "Cancel Waiver";
            break;

        case 3:
            punishment = "Rusticate";
            break;

        case 4:
            punishment = "Call Parents";
            break;

        case 5:
            punishment = "Investigation";
            break;

        case 6:
            punishment = "Send Matter to Head Teacher";
            break;

        default:
            printf("Invalid choice!\n");
            return;
    }

    FILE *fp = fopen("suggestion.txt", "a");

    if(fp == NULL)
    {
        printf("\nError opening suggestion.txt\n");
        return;
    }

    fprintf(fp,
            "Complaint ID: %d | FINAL PUNISHMENT: %s\n",
            complaintID,
            punishment);

    fclose(fp);

    printf("\nFinal punishment recorded successfully.\n");
}

/* =========================================================
          SPECIAL COMMITTEE - SEXUAL HARASSMENT
   ========================================================= */

void special_committee_view_complaints()
{
    Complaint complaints[MAX];
    int count = 0;
    int found = 0;

    if (!load_all_complaints(complaints, &count) || count == 0)
    {
        printf("\n[!] No complaints found.\n");
        return;
    }

    printf("\n========================================================\n");
    printf("       SEXUAL HARASSMENT COMPLAINTS\n");
    printf("========================================================\n");

    for (int i = 0; i < count; i++)
    {
        if (strcmp(complaints[i].category, "Sexual Harassment") == 0 &&
            strcmp(complaints[i].assignedTeam, "SpecialCommittee") == 0 &&
            strcmp(complaints[i].status, "Closed") != 0)
        {
            found = 1;

            printf("\n--------------------------------------------------------\n");
            printf("Complaint ID  : #%d\n", complaints[i].id);
            printf("Filed By      : %s\n", complaints[i].user);
            printf("Against User  : %s\n", complaints[i].targetUser);
            printf("Category      : %s\n", complaints[i].category);
            printf("Description   : %s\n", complaints[i].description);
            printf("Priority      : %s\n", complaints[i].priority);
            printf("Status        : %s\n", complaints[i].status);
            printf("Date          : %s\n", complaints[i].date);
            printf("Assigned Team : Special Committee\n");
            printf("--------------------------------------------------------\n");
        }
    }

    if (!found)
    {
        printf("\n[!] No pending Sexual Harassment complaints found.\n");
    }
}


void special_committee_decision()
{
    Complaint complaints[MAX];
    int count = 0;
    int complaintID;
    int found = -1;

    if (!load_all_complaints(complaints, &count) || count == 0)
    {
        printf("\n[!] No complaints found.\n");
        return;
    }

    printf("\n========================================================\n");
    printf("       REVIEW SEXUAL HARASSMENT COMPLAINT\n");
    printf("========================================================\n");

    printf("Enter Complaint ID: ");

    if (scanf("%d", &complaintID) != 1)
    {
        while (getchar() != '\n');
        printf("\n[!] Invalid Complaint ID.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        if (complaints[i].id == complaintID &&
            strcmp(complaints[i].category, "Sexual Harassment") == 0 &&
            strcmp(complaints[i].assignedTeam, "SpecialCommittee") == 0 &&
            strcmp(complaints[i].status, "Closed") != 0)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("\n[!] Pending Sexual Harassment Complaint #%d not found.\n",
               complaintID);
        return;
    }

    printf("\n================ COMPLAINT DETAILS ================\n");
    printf("Complaint ID : #%d\n", complaints[found].id);
    printf("Filed By     : %s\n", complaints[found].user);
    printf("Against User : %s\n", complaints[found].targetUser);
    printf("Category     : %s\n", complaints[found].category);
    printf("Description  : %s\n", complaints[found].description);
    printf("Priority     : %s\n", complaints[found].priority);
    printf("Status       : %s\n", complaints[found].status);
    printf("Date         : %s\n", complaints[found].date);
    printf("====================================================\n");

    char comment[256];

    while (getchar() != '\n');

    printf("\nEnter Special Committee Comment:\n");
    fgets(comment, sizeof(comment), stdin);
    comment[strcspn(comment, "\n")] = '\0';

    if (strlen(comment) == 0)
    {
        strcpy(comment, "Complaint reviewed by Special Committee.");
    }

    int punishmentChoice;

    printf("\n====================================================\n");
    printf("                 COMMITTEE DECISION\n");
    printf("====================================================\n");
    printf("1. Official Warning\n");
    printf("2. Fine\n");
    printf("3. Account Suspension\n");
    printf("4. Proctorial Inquiry\n");
    printf("5. Disciplinary Action\n");
    printf("6. No Punishment\n");
    printf("====================================================\n");
    printf("Enter Choice: ");

    if (scanf("%d", &punishmentChoice) != 1)
    {
        while (getchar() != '\n');
        printf("\n[!] Invalid choice.\n");
        return;
    }

    char punishment[100];

    switch (punishmentChoice)
    {
        case 1:
            strcpy(punishment, "Official Warning");
            break;
        case 2:
            strcpy(punishment, "Fine");
            break;
        case 3:
            strcpy(punishment, "Account Suspension");
            break;
        case 4:
            strcpy(punishment, "Proctorial Inquiry");
            break;
        case 5:
            strcpy(punishment, "Disciplinary Action");
            break;
        case 6:
            strcpy(punishment, "No Punishment");
            break;
        default:
            printf("\n[!] Invalid punishment choice.\n");
            return;
    }

    FILE *fp = fopen(PUNISHMENT_FILE, "a");

    if (fp == NULL)
    {
        printf("\n[!] Error opening punishments.txt\n");
        return;
    }

    time_t currentTime = time(NULL);
    struct tm *currentDate = localtime(&currentTime);

    char date[20];

    if (currentDate != NULL)
    {
        sprintf(date, "%04d-%02d-%02d",
                currentDate->tm_year + 1900,
                currentDate->tm_mon + 1,
                currentDate->tm_mday);
    }
    else
    {
        strcpy(date, "Unknown");
    }

    fprintf(fp, "%d|%s|%s|Committee Comment: %s|%s\n",
            complaints[found].id,
            complaints[found].targetUser,
            punishment,
            comment,
            date);

    fclose(fp);

    /*
       Final step of the workflow:
       After the Special Committee makes its decision,
       the complaint becomes Closed.
    */
    strcpy(complaints[found].status, "Closed");

    if (!save_complaints(complaints, count))
    {
        printf("\n[!] Punishment saved, but complaint status could not be updated.\n");
        return;
    }

    char notification[256];

    snprintf(notification,
             sizeof(notification),
             "Special Committee decision for Sexual Harassment Complaint #%d: %s",
             complaints[found].id,
             punishment);

    send_notification(complaints[found].targetUser,
                      complaints[found].id,
                      notification);

    printf("\n====================================================\n");
    printf("       SPECIAL COMMITTEE DECISION RECORDED\n");
    printf("====================================================\n");
    printf("Complaint ID : #%d\n", complaints[found].id);
    printf("Punishment   : %s\n", punishment);
    printf("Comment      : %s\n", comment);
    printf("Status       : Closed\n");
    printf("Saved To     : punishments.txt\n");
    printf("====================================================\n");
}


void special_committee_dashboard()
{
    int choice;

    do
    {
        printf("\n====================================================\n");
        printf("          SPECIAL COMMITTEE PORTAL\n");
        printf("====================================================\n");
        printf("1. View Sexual Harassment Complaints\n");
        printf("2. Review Complaint & Make Decision\n");
        printf("3. Logout\n");
        printf("====================================================\n");
        printf("Enter Choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("\n[!] Invalid input.\n");
            continue;
        }

        switch (choice)
        {
            case 1:
                special_committee_view_complaints();
                break;

            case 2:
                special_committee_decision();
                break;

            case 3:
                printf("\nLogging out from Special Committee...\n");
                break;

            default:
                printf("\n[!] Invalid choice.\n");
        }

    } while (choice != 3);
}

/* =========================================================
                  RESOLUTION TEAM MENU
   ========================================================= */

void ResolutionTeam()
{
    int choice;

    do
    {
        printf("\n========== RESOLUTION TEAM ==========\n");
        printf("1. Register Resolution Team\n");
        printf("2. Login\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                teamRegister();
                break;

            case 2:
                resolutionLogin();
                break;

            case 3:
                printf("Returning to Main Menu...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 3);
}

