#ifndef COMPLAINT_H
#define COMPLAINT_H
#ifndef NOTIF_FILE
#define NOTIF_FILE "notifications.txt"
#endif
#define FILE_NAME "complaints.csv"
#define RECOVERY_FILE "user_recovery.txt"
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
void send_notification(const char *target_admin, int complaint_id, const char *msg);

void notify_general_admin(int complaint_id, const char *category, const char *user_id);
void notify_assigned_team(const char *teamName, int complaintID, const char *status);
void notify_accused_person_updated(const char *targetUser, int complaintId, const char *category);
void trigger_accused_popup_on_login(const char *logged_in_user);
void notify_grievance_committee(int complaint_id, const char *user_id);
void notify_filing_user(const char *user_id, int complaint_id, const char *category);

void check_user_notifications(const char *username);
void check_and_alert_accused(const char *username);




#endif
