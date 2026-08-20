#include "Resolution.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>


#ifdef _WIN32
#include <windows.h>
#endif

/* =========================================================
                  RESOLUTION TEAM SYSTEM
   ========================================================= */

void teamRegister(void)
{
    char team[50];
    char username[50];
    char password[50];
    char confirm[50];
    char hash[HASH_LEN];
    char setupKey[50];

    int choice;

    printf("\n========== RESOLUTION TEAM REGISTRATION ==========\n");

    printf("1. IT Team\n");
    printf("2. Maintenance Team\n");
    printf("3. Proctor Office\n");
    printf("4. Grievance Committee\n");
    printf("5. Student Discipline Committee\n");
    printf("6. Special Committee (Sexual Harassment)\n");

    printf("Enter team choice: ");
    if (scanf("%d", &choice) != 1)
    {
        while (getchar() != '\n');
        printf("\n[!] Invalid input!\n");
        return;
    }

    switch(choice)
    {
        case 1:
            strcpy(team, "IT");

            printf("Enter IT Team Setup Key: ");
            scanf("%49s", setupKey);

            if(strcmp(setupKey, "IT@123") != 0)
            {
                printf("\n[!] Invalid IT Team setup key!\n");
                return;
            }
            break;

        case 2:
            strcpy(team, "Maintenance");

            printf("Enter Maintenance Team Setup Key: ");
            scanf("%49s", setupKey);

            if(strcmp(setupKey, "MAINT@123") != 0)
            {
                printf("\n[!] Invalid Maintenance Team setup key!\n");
                return;
            }
            break;

        case 3:
            strcpy(team, "Proctor");

            printf("Enter Proctor Office Setup Key: ");
            scanf("%49s", setupKey);

            if(strcmp(setupKey, "PROCTOR@123") != 0)
            {
                printf("\n[!] Invalid Proctor Office setup key!\n");
                return;
            }
            break;

        case 4:
            strcpy(team, "Grievance");

            printf("Enter Grievance Committee Setup Key: ");
            scanf("%49s", setupKey);

            if(strcmp(setupKey, "GRIEVANCE@123") != 0)
            {
                printf("\n[!] Invalid Grievance Committee setup key!\n");
                return;
            }
            break;

        case 5:
            strcpy(team, "StudentDiscipline");

            printf("Enter Student Discipline Committee Setup Key: ");
            scanf("%49s", setupKey);

            if(strcmp(setupKey, "DISCIPLINE@123") != 0)
            {
                printf("\n[!] Invalid Student Discipline Committee setup key!\n");
                return;
            }
            break;

        case 6:
            strcpy(team, "SpecialCommittee");

            printf("Enter Special Committee Setup Key: ");
            scanf("%49s", setupKey);

            if(strcmp(setupKey, "SPECIAL@123") != 0)
            {
                printf("\n[!] Invalid Special Committee setup key!\n");
                return;
            }
            break;

        default:
            printf("\n[!] Invalid choice!\n");
            return;
    }

    printf("\nCreate Username: ");
    scanf("%49s", username);

    printf("Create Password: ");
    scanf("%49s", password);

    printf("Confirm Password: ");
    scanf("%49s", confirm);

    if(strcmp(password, confirm) != 0)
    {
        printf("\n[!] Password did not match!\n");
        return;
    }
    get_user_md5(password, hash);

    FILE *fp = fopen("resolution_team.txt", "a");

    if(fp == NULL)
    {
        printf("\n[!] Error opening resolution_team.txt\n");
        return;
    }

    fprintf(fp, "%s %s %s\n", team, username, hash);

    fclose(fp);

    printf("\n============================================\n");
    printf("Resolution team registered successfully!\n");
    printf("Team     : %s\n", team);
    printf("Username : %s\n", username);
    printf("============================================\n");
}


/* ---------- Resolution Team Login ---------- */

void resolutionLogin(void)
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

void addCommentByTeam(const char *teamName)
{
    Complaint complaints[MAX];
    int count = 0;
    int complaintID;
    int foundIndex = -1;

    if (!load_all_complaints(complaints, &count) || count == 0)
    {
        printf("\n[!] No complaints found.\n");
        return;
    }

    printf("\n========== %s - ADD COMMENT ==========\n", teamName);

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
            strcmp(complaints[i].assignedTeam, teamName) == 0)
        {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1)
    {
        printf("\n[!] Complaint not found or this complaint is not assigned to your team.\n");
        return;
    }

    printf("\n==================================================\n");
    printf("                 COMPLAINT DETAILS\n");
    printf("==================================================\n");

    printf("Complaint ID   : %d\n", complaints[foundIndex].id);
    printf("Filed By       : %s\n", complaints[foundIndex].user);
    printf("Category       : %s\n", complaints[foundIndex].category);
    printf("Description    : %s\n", complaints[foundIndex].description);
    printf("Priority       : %s\n", complaints[foundIndex].priority);
    printf("Status         : %s\n", complaints[foundIndex].status);
    printf("Date           : %s\n", complaints[foundIndex].date);
    printf("Assigned Team  : %s\n", complaints[foundIndex].assignedTeam);

    printf("==================================================\n");

    FILE *fp = fopen("team_comments.txt", "r");

    char fileTeam[50];
    int fileComplaintID;
    char existingComment[500];

    int commentFound = 0;

    if (fp != NULL)
    {
        while (fscanf(fp, "%d|%49[^|]|%499[^\n]\n",
                      &fileComplaintID,
                      fileTeam,
                      existingComment) == 3)
        {
            if (fileComplaintID == complaintID &&
                strcmp(fileTeam, teamName) == 0)
            {
                commentFound = 1;

                printf("\n==================================================\n");
                printf("                 CURRENT COMMENT\n");
                printf("==================================================\n");
                printf("%s\n", existingComment);
                printf("==================================================\n");

                break;
            }
        }

        fclose(fp);
    }

    if (!commentFound)
    {
        printf("\nCurrent Comment : No comment added yet.\n");
    }

    char newComment[500];

    while (getchar() != '\n');

    printf("\nEnter your comment:\n");
    fgets(newComment, sizeof(newComment), stdin);

    newComment[strcspn(newComment, "\n")] = '\0';

    if (strlen(newComment) == 0)
    {
        printf("\n[!] Comment cannot be empty.\n");
        return;
    }

    FILE *readFile = fopen("team_comments.txt", "r");
    FILE *tempFile = fopen("team_comments_temp.txt", "w");

    if (tempFile == NULL)
    {
        printf("\n[!] Error creating temporary comment file.\n");

        if (readFile != NULL)
            fclose(readFile);

        return;
    }

    int updated = 0;

    if (readFile != NULL)
    {
        while (fscanf(readFile, "%d|%49[^|]|%499[^\n]\n",
                      &fileComplaintID,
                      fileTeam,
                      existingComment) == 3)
        {
            if (fileComplaintID == complaintID &&
                strcmp(fileTeam, teamName) == 0)
            {
                fprintf(tempFile, "%d|%s|%s\n",
                        complaintID,
                        teamName,
                        newComment);

                updated = 1;
            }
            else
            {
                fprintf(tempFile, "%d|%s|%s\n",
                        fileComplaintID,
                        fileTeam,
                        existingComment);
            }
        }

        fclose(readFile);
    }

    if (!updated)
    {
        fprintf(tempFile, "%d|%s|%s\n",
                complaintID,
                teamName,
                newComment);
    }

    fclose(tempFile);

    remove("team_comments.txt");
    rename("team_comments_temp.txt", "team_comments.txt");

    printf("\n[SUCCESS] Comment saved successfully.\n");
}

/* =========================================================
                         IT TEAM
   ========================================================= */

void ITMenu(const char *admin_name)
{
    (void)admin_name;
    int choice;

    do
    {
        printf("\n========== IT SUPPORT TEAM ==========\n");
        printf("1. View Complaints\n");
        printf("2. Update Status\n");
        printf("3. Add Comment\n");
        printf("4. Exit\n");

        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("Invalid choice!\n");
            continue;
        }

        switch(choice)
        {
            case 1:
                viewComplaintsByAssignedTeam("IT Support");
                break;

            case 2:
                updateStatus("IT Support");
                break;

            case 3:
                addCommentByTeam("IT Support");
                break;

            case 4:
                printf("Logging out from IT Team...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 4);
}


/* =========================================================
                     MAINTENANCE TEAM
   ========================================================= */

void MaintenanceMenu(const char *admin_name)
{
    (void)admin_name;
    int choice;

    do
    {
        printf("\n========== MAINTENANCE TEAM ==========\n");
        printf("1. View Complaints\n");
        printf("2. Update Status\n");
        printf("3. Add Comment\n");
        printf("4. Exit\n");

        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("Invalid choice!\n");
            continue;
        }

        switch(choice)
        {
            case 1:
                viewComplaintsByAssignedTeam("Maintenance Team");
                break;

            case 2:
                updateStatus("Maintenance Team");
                break;

            case 3:
                addCommentByTeam("Maintenance");
                break;

            case 4:
                printf("Logging out from Maintenance Team...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 4);
}


/* =========================================================
                       PROCTOR OFFICE
   ========================================================= */

void ProctorMenu(void)
{
    int choice;

    do
    {
        printf("\n========== PROCTOR OFFICE ==========\n");
        printf("1. View Complaints\n");
        printf("2. Give Suggestion\n");
        printf("3. Add Comment\n");
        printf("4. Exit\n");

        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("Invalid choice!\n");
            continue;
        }

        switch(choice)
        {
            case 1:
                viewComplaintsByAssignedTeam("Proctor");
                break;

            case 2:
                giveProctorSuggestion();
                break;

            case 3:
                addCommentByTeam("Proctor");
                break;

            case 4:
                printf("Logging out from Proctor Office...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 4);
}

void giveProctorSuggestion(void)
{
    Complaint complaints[MAX];
    int count = 0;
    int complaintID;
    int choice;
    int found = 0;

    const char *suggestion;

    if (!load_all_complaints(complaints, &count) || count == 0)
    {
        printf("\n[!] No complaints found.\n");
        return;
    }

    printf("\n========== PROCTOR COMPLAINT REVIEW ==========\n");

    printf("Enter Complaint ID: ");

    if (scanf("%d", &complaintID) != 1)
    {
        while (getchar() != '\n');
        printf("\n[!] Invalid Complaint ID.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
#ifdef _WIN32
        if (complaints[i].id == complaintID &&
            _stricmp(complaints[i].assignedTeam, "Proctor") == 0)
#else
        if (complaints[i].id == complaintID &&
            case_insensitive_compare(complaints[i].assignedTeam, "Proctor") == 0)
#endif
        {
            found = 1;

            printf("\n============================================\n");
            printf("Complaint ID  : %d\n", complaints[i].id);
            printf("Category      : %s\n", complaints[i].category);
            printf("Filed By      : %s\n", complaints[i].user);
            printf("Description   : %s\n", complaints[i].description);
            printf("Priority      : %s\n", complaints[i].priority);
            printf("Status        : %s\n", complaints[i].status);
            printf("Date          : %s\n", complaints[i].date);
            printf("Assigned Team : %s\n", complaints[i].assignedTeam);
            printf("============================================\n");

            printf("\n========== MODIFY PRIORITY ==========\n");
            printf("1. Low\n");
            printf("2. Medium\n");
            printf("3. High\n");
            printf("4. Keep Current Priority (%s)\n", complaints[i].priority);

            printf("Enter choice: ");

            int priority_choice;

            if (scanf("%d", &priority_choice) != 1)
            {
                while (getchar() != '\n');
                printf("\n[!] Invalid priority choice.\n");
                return;
            }

            switch(priority_choice)
            {
                case 1:
                    strcpy(complaints[i].priority, "Low");
                    break;

                case 2:
                    strcpy(complaints[i].priority, "Medium");
                    break;

                case 3:
                    strcpy(complaints[i].priority, "High");
                    break;

                case 4:
                    break;

                default:
                    printf("\n[!] Invalid priority choice.\n");
                    return;
            }

            printf("\n========== SELECT SUGGESTION ==========\n");
            printf("1. Fine\n");
            printf("2. Cancel Waiver\n");
            printf("3. Rusticate\n");
            printf("4. Call Parents\n");
            printf("5. Investigation\n");
            printf("6. Send Matter to Head Teacher\n");

            printf("Enter choice: ");

            if (scanf("%d", &choice) != 1)
            {
                while (getchar() != '\n');
                printf("\n[!] Invalid suggestion choice.\n");
                return;
            }

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
                    printf("\n[!] Invalid suggestion choice.\n");
                    return;
            }

            if (!save_complaints(complaints, count))
            {
                printf("\n[!] Failed to save complaint changes.\n");
                return;
            }

            FILE *fp = fopen("suggestion.txt", "a");

            if (fp == NULL)
            {
                printf("\n[!] Error opening suggestion.txt\n");
                return;
            }

            fprintf(fp,
                    "Complaint ID: %d | Proctor Suggestion: %s\n",
                    complaintID,
                    suggestion);

            fclose(fp);

            printf("\n============================================\n");
            printf("[SUCCESS] Complaint reviewed successfully!\n");
            printf("Complaint ID : %d\n", complaints[i].id);
            printf("New Priority : %s\n", complaints[i].priority);
            printf("Suggestion   : %s\n", suggestion);
            printf("============================================\n");

            printf("\nThe suggestion will be reviewed by the Student Discipline Committee.\n");

            return;
        }
    }

    if (!found)
    {
        printf("\n[!] Complaint #%d is not assigned to the Proctor Office.\n", complaintID);
        printf("[!] Proctor cannot review this complaint.\n");
    }
}


/* =========================================================
                   GRIEVANCE COMMITTEE
   ========================================================= */

void GrievanceMenu(void)
{
    int choice;

    do
    {
        printf("\n========== GRIEVANCE COMMITTEE ==========\n");
        printf("1. View Grievances\n");
        printf("2. Recall\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("Invalid choice!\n");
            continue;
        }

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

void viewGrievances(void)
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

void grievanceRecall(void)
{
    int complaintID;
    int choice;

    const char *action;

    printf("\n========== GRIEVANCE RECALL ==========\n");

    printf("Enter Complaint ID: ");
    if (scanf("%d", &complaintID) != 1)
    {
        while (getchar() != '\n');
        printf("Invalid Complaint ID!\n");
        return;
    }

    printf("\nSelect Action:\n");
    printf("1. Fine\n");
    printf("2. Remove Waiver\n");
    printf("3. Rusticate\n");

    printf("Enter choice: ");
    if (scanf("%d", &choice) != 1)
    {
        while (getchar() != '\n');
        printf("Invalid choice!\n");
        return;
    }

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

void StudentDisciplineMenu(void)
{
    int choice;

    do
    {
        printf("\n========== STUDENT DISCIPLINE COMMITTEE ==========\n");
        printf("1. View Proctor Suggestions\n");
        printf("2. Implement Punishment\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("Invalid choice!\n");
            continue;
        }

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

void viewSuggestions(void)
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
        if(sscanf(line, "Complaint ID: %d", &complaintID) == 1)
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
        else if(sscanf(line, "Grievance Complaint ID: %d", &complaintID) == 1)
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

void implementPunishment(void)
{
    Complaint complaints[MAX];
    int count = 0;
    int id;
    int found = 0;

    if (!load_all_complaints(complaints, &count) || count == 0)
    {
        printf("\n[!] No complaints found.\n");
        return;
    }

    printf("\n========== IMPLEMENT PUNISHMENT ==========\n");

    printf("Enter Complaint ID: ");

    if (scanf("%d", &id) != 1)
    {
        while (getchar() != '\n');
        printf("\n[!] Invalid Complaint ID.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        if (complaints[i].id == id)
        {
            found = 1;

            printf("\n===========================================\n");
            printf("Complaint ID : %d\n", complaints[i].id);
            printf("Filed By     : %s\n", complaints[i].user);
            printf("Description  : %s\n", complaints[i].description);
            printf("Category     : %s\n", complaints[i].category);
            printf("Priority     : %s\n", complaints[i].priority);
            printf("Status       : %s\n", complaints[i].status);
            printf("Assigned Team: %s\n", complaints[i].assignedTeam);
            printf("===========================================\n");

            printf("\n========== SELECT FINAL PUNISHMENT ==========\n");
            printf("1. Fine\n");
            printf("2. Cancel Waiver\n");
            printf("3. Rusticate\n");
            printf("4. Call Parents\n");
            printf("5. Investigation\n");
            printf("6. Send Matter to Head Teacher\n");

            printf("Enter Choice: ");

            int choice;

            if (scanf("%d", &choice) != 1)
            {
                while (getchar() != '\n');
                printf("\n[!] Invalid choice.\n");
                return;
            }

            char punishment[100];

            switch (choice)
            {
                case 1:
                    strcpy(punishment, "Fine");
                    break;

                case 2:
                    strcpy(punishment, "Cancel Waiver");
                    break;

                case 3:
                    strcpy(punishment, "Rusticate");
                    break;

                case 4:
                    strcpy(punishment, "Call Parents");
                    break;

                case 5:
                    strcpy(punishment, "Investigation");
                    break;

                case 6:
                    strcpy(punishment, "Send Matter to Head Teacher");
                    break;

                default:
                    printf("\n[!] Invalid punishment choice.\n");
                    return;
            }

            FILE *fp = fopen(PUNISHMENT_FILE, "a");

            if (fp == NULL)
            {
                printf("\n[!] Could not open %s\n", PUNISHMENT_FILE);
                return;
            }

            time_t now = time(NULL);
            struct tm *t = localtime(&now);

            char date[20];

            if (t != NULL)
            {
                strftime(date, sizeof(date), "%Y-%m-%d", t);
            }
            else
            {
                strcpy(date, "Unknown");
            }

            fprintf(fp,
                    "%d|%s|%s|Student Discipline Committee|%s\n",
                    complaints[i].id,
                    complaints[i].targetUser,
                    punishment,
                    date);

            fclose(fp);

            strcpy(complaints[i].status, "Closed");

            save_complaints(complaints, count);

            char notification[300];

            snprintf(notification,
                     sizeof(notification),
                     "Final punishment for Complaint #%d: %s",
                     complaints[i].id,
                     punishment);

            send_notification(
                complaints[i].targetUser,
                complaints[i].id,
                notification
            );

            printf("\n===========================================\n");
            printf("[SUCCESS] Final punishment implemented.\n");
            printf("Complaint ID : %d\n", complaints[i].id);
            printf("Punishment   : %s\n", punishment);
            printf("Status       : Closed\n");
            printf("Student      : %s\n", complaints[i].targetUser);
            printf("===========================================\n");

            return;
        }
    }

    if (!found)
    {
        printf("\n[!] Complaint ID not found.\n");
    }
}


/* =========================================================
                   SPECIAL COMMITTEE
   ========================================================= */

void special_committee_view_complaints(void)
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

void special_committee_decision(void)
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
        printf("\n[!] Pending Sexual Harassment Complaint #%d not found.\n", complaintID);
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
        printf("\n[!] Error opening %s\n", PUNISHMENT_FILE);
        return;
    }

    time_t currentTime = time(NULL);
    struct tm *currentDate = localtime(&currentTime);

    char date[20];

    if (currentDate != NULL)
    {
        strftime(date, sizeof(date), "%Y-%m-%d", currentDate);
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
    printf("Saved To     : %s\n", PUNISHMENT_FILE);
    printf("====================================================\n");
}

void special_committee_dashboard(void)
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

void ResolutionTeam(void)
{
    int choice;

    do
    {
        printf("\n========== RESOLUTION TEAM ==========\n");
        printf("1. Register Resolution Team\n");
        printf("2. Login\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("Invalid choice!\n");
            continue;
        }

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
