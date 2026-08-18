#ifndef COMPLAINT_H
#define COMPLAINT_H
//structure

typedef struct {
    int id;
    char user[50];
    char targetUser[50];
    char category[50];
    char description[200];
    char priority[20];
    char status[20];
    char date[20];
    char assignedTeam[50];
    char assignedAdmin[50];
    int rating;
} Complaint;

int generateComplaintID();
void FileComplaint(const char *logged_in_user);


#endif
