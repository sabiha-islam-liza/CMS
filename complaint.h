#ifndef COMPLAINT_H
#define COMPLAINT_H

typedef struct
{
    int id;
    char user[50];
    char category[50];
    char description[200];
    char priority[20];
    char status[20];
    char date[20];
    char assignedTeam[50];
} Complaint;

void FileComplaint();
void TrackComplaint();
void Admin();
void adminregister();
int admin_login();

#endif
