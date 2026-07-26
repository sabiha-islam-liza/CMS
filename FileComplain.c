#include <stdio.h>
#include <string.h>
struct Complaint
{
    int id;
    char name[50];
    char category[50];
    char description[200];
    int priority;

    char status[30];
    char date[20];
    char assignedTeam[50];
};
int idCounter = 0;
int generateComplaintID()
{
    idCounter++;
    return idCounter;
}
void fileComplaint()
{
    struct Complaint c;

    FILE *fp;

    fp = fopen("complaints.csv","a");

    if(fp==NULL)
    {
        printf("File could not be opened!\n");
        return;
    }

    c.id = generateComplaintID();

    printf("Enter Name: ");
    scanf("%s",c.name);

    printf("Enter Category: ");
    scanf("%s",c.category);

    printf("Enter Description: ");
    scanf("%s",c.description);

    printf("Enter Priority: ");
    scanf("%d",&c.priority);

    printf("Enter Status: ");
    scanf("%s",c.status);

    printf("Enter Date (DD/MM/YYYY): ");
    scanf("%s",c.date);

    printf("Enter Assigned Team: ");
    scanf("%s",c.assignedTeam);
        fprintf(fp,"%d,%s,%s,%s,%d,%s,%s,%s\n",
            c.id,
            c.name,
            c.category,
            c.description,
            c.priority,
            c.status,
            c.date,
            c.assignedTeam);

    fclose(fp);

    printf("\nComplaint Filed Successfully!\n");
}
void viewComplaints()
{
    FILE *fp;
    char line[500];

    fp = fopen("complaints.csv","r");

    if(fp==NULL)
    {
        printf("No Complaint Found!\n");
        return;
    }

   printf("All Complaints\n");
    while(fgets(line,sizeof(line),fp))
    {
        printf("%s",line);
    }

    fclose(fp);
}
int main()
{
    int choice;

    do
    {
        printf("Complaint Management\n");
        printf("1. File Complaint\n");
        printf("2. View Complaints\n");
        printf("3. Exit\n");

        printf("Enter Choice: ");
        scanf("%d",&choice);

        switch(choice)
        {
        case 1:
            fileComplaint();
            break;

        case 2:
            viewComplaints();
            break;

        case 3:
            printf("Program Closed.\n");
            break;

        default:
            printf("Invalid Choice!\n");
        }

    }while(choice!=3);

    return 0;
}
