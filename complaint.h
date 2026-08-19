#ifndef COMPLAINT_H
#define COMPLAINT_H
#define FILE_NAME "complaints.csv"
#define MAX 100
#define ID_LEN 50
#define PASS_LEN 50
#define HASH_LEN 65
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

//252-35-468
int generateComplaintID();
void FileComplaint(const char *logged_in_user);


//Done by Farhana Yeasmin (252-35-532)
int load_all_complaints(Complaint complaints[], int *count);
int save_complaints(Complaint complaints[], int count);
void viewAllComplaints();
void viewComplaintByID();
void viewComplaintsByStatus();
void viewComplaintsByDate();
void viewComplaintsByPriority();
void viewDepartmentComplaints(const char *categoryFilter);
void viewComplaintsMenu();
void deleteComplaint();

//Done by Liza (252-35-485)
void view_user_punishments(const char *logged_in_user);
void viewComplaints(const char *logged_in_user);  // user's portal view , they can see the complaints filed by them or their account
void viewAccusedComplaints(const char *logged_in_user); // the accused person can see the complaint filed against him
void viewComplaintsByAssignedTeam(const char *team_name);
void set_win_color(unsigned short color);
#endif
