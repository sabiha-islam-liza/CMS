#include <stdio.h>
#include <string.h>
#include "complaint.h"

#define FILE_NAME "complaints.csv"
#define MAX 100

/* Load all complaints from file */

int loadComplaints(Complaint complaints[])
{
    FILE *fp;

    int count = 0;

    fp = fopen(FILE_NAME, "r");

    if(fp == NULL)
    {
        printf("\nNo complaints found.\n");
        return 0;
    }

    while(1)
    {
        Complaint c;

        if(fscanf(fp,"%d\n",&c.id) != 1)
        {
            break;
        }

        fgets(c.category,sizeof(c.category),fp);
        fgets(c.description,sizeof(c.description),fp);
        fgets(c.priority,sizeof(c.priority),fp);
        fgets(c.status,sizeof(c.status),fp);
        fgets(c.date,sizeof(c.date),fp);
        fgets(c.assignedTeam,sizeof(c.assignedTeam),fp);

        /* Remove newline */

        c.category[strcspn(c.category,"\n")] = '\0';
        c.description[strcspn(c.description,"\n")] = '\0';
        c.priority[strcspn(c.priority,"\n")] = '\0';
        c.status[strcspn(c.status,"\n")] = '\0';
        c.date[strcspn(c.date,"\n")] = '\0';
        c.assignedTeam[strcspn(c.assignedTeam,"\n")] = '\0';

        complaints[count] = c;
        count++;

        if(count >= MAX)
        {
            break;
        }

        /* Skip blank line between complaints */

        fscanf(fp,"\n");
    }

    fclose(fp);

    return count;
}

void printComplaint(Complaint c)
{
    printf("\n==============================\n");
    printf("Complaint ID : %d\n", c.id);
    printf("Category     : %s\n", c.category);
    printf("Description  : %s\n", c.description);
    printf("Priority     : %s\n", c.priority);
    printf("Status       : %s\n", c.status);
    printf("Date         : %s\n", c.date);
    printf("Assigned Team: %s\n", c.assignedTeam);
    printf("==============================\n");
}

void viewAllComplaints()
{
    Complaint complaints[MAX];

    int count = loadComplaints(complaints);

    if(count == 0)
    {
        printf("\nNo complaints found.\n");
        return;
    }

    int i;

    for(i = 0; i < count; i++)
    {
        printComplaint(complaints[i]);
    }

    printf("\nTotal Complaints: %d\n", count);
}

void viewComplaintByID()
{
    Complaint complaints[MAX];

    int count = loadComplaints(complaints);

    if(count == 0)
    {
        printf("\nNo complaints found.\n");
        return;
    }

    int id;
    int i;
    int found = 0;

    printf("Enter Complaint ID: ");
    scanf("%d",&id);

    for(i = 0; i < count; i++)
    {
        if(complaints[i].id == id)
        {
            printComplaint(complaints[i]);
            found = 1;
            break;
        }
    }

    if(found == 0)
    {
        printf("\nComplaint not found.\n");
    }
}

void viewComplaintsByStatus()
{
    Complaint complaints[MAX];

    int count = loadComplaints(complaints);

    if(count == 0)
    {
        printf("\nNo complaints found.\n");
        return;
    }

    char status[20];

    printf("Enter Status (Open/Pending/In Progress/Closed): ");

    getchar();

    fgets(status,sizeof(status),stdin);

    status[strcspn(status,"\n")] = '\0';

    int i;
    int found = 0;

    for(i = 0; i < count; i++)
    {
        if(strcmp(complaints[i].status,status) == 0)
        {
            printComplaint(complaints[i]);
            found = 1;
        }
    }

    if(found == 0)
    {
        printf("\nNo complaints found.\n");
    }
}

void viewComplaintsByDate()
{
    Complaint complaints[MAX];

    int count = loadComplaints(complaints);

    if(count == 0)
    {
        printf("\nNo complaints found.\n");
        return;
    }

    int i, j;
    Complaint temp;

    for(i = 0; i < count - 1; i++)
    {
        for(j = 0; j < count - i - 1; j++)
        {
            if(strcmp(complaints[j].date, complaints[j + 1].date) > 0)
            {
                temp = complaints[j];
                complaints[j] = complaints[j + 1];
                complaints[j + 1] = temp;
            }
        }
    }

    printf("\n===== Complaints Sorted by Date =====\n");

    for(i = 0; i < count; i++)
    {
        printComplaint(complaints[i]);
    }
}

int getPriorityValue(char priority[])
{
    if(strcmp(priority,"High") == 0)
        return 3;

    if(strcmp(priority,"Medium") == 0)
        return 2;

    if(strcmp(priority,"Low") == 0)
        return 1;

    return 0;
}

void viewComplaintsByPriority()
{
    Complaint complaints[MAX];

    int count = loadComplaints(complaints);

    if(count == 0)
    {
        printf("\nNo complaints found.\n");
        return;
    }

    int i, j;
    Complaint temp;

    for(i = 0; i < count - 1; i++)
    {
        for(j = 0; j < count - i - 1; j++)
        {
            if(getPriorityValue(complaints[j].priority) < getPriorityValue(complaints[j + 1].priority))
            {
                temp = complaints[j];
                complaints[j] = complaints[j + 1];
                complaints[j + 1] = temp;
            }
        }
    }

    printf("\n===== Complaints Sorted by Priority =====\n");

    for(i = 0; i < count; i++)
    {
        printComplaint(complaints[i]);
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

