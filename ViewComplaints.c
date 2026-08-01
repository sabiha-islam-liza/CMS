#include <stdio.h>
#include <string.h>
#include "complaint.h"

#define FILE_NAME "complaints.csv"
#define MAX 100

/* Load all complaints from file */

int loadComplaints(Complaint complaints[]) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        printf("\nNo complaints found.\n");
        return 0;
    }

    int count = 0;

    // Read single-line CSV records until EOF or MAX capacity reached
    while (count < MAX && fscanf(fp, "%d,%49[^,],%49[^,],%99[^,],%19[^,],%19[^,],%19[^,],%49[^\n]\n",
                                 &complaints[count].id,
                                 complaints[count].user,
                                 complaints[count].category,
                                 complaints[count].description,
                                 complaints[count].priority,
                                 complaints[count].status,
                                 complaints[count].date,
                                 complaints[count].assignedTeam) == 8) {
        count++;
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

