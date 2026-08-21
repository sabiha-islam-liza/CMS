#ifndef USER_H
#define USER_H

// --- File Path Definitions ---
#define RATINGS_FILE    "admin_ratings.txt"
#define DB_FILE         "user.txt"
#define RECOVERY_FILE   "user_recovery.txt"
#define PUNISHMENT_FILE "punishments.txt"
#define NOTIF_FILE      "notifications.txt"
#define FILE_NAME       "complaints.csv"
// --- Buffer & ID Length Limits ---
#define MAX      100
#define ID_LEN   50
#define PASS_LEN 50
#define HASH_LEN 65

// --- Struct Definitions ---
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

// --- Function Prototypes ---



// Sayma (252-35-468)
// Complaint File & Load Operations
int generateComplaintID();
void FileComplaint(const char *logged_in_user);
int load_all_complaints(Complaint complaints[], int *count);
int save_complaints(Complaint complaints[], int count);







// Liza (252-35-485)
// Notification & Alert Operations
void notify_general_admin(int complaint_id, const char *category, const char *user_id);
void notify_assigned_team(const char *teamName, int complaintID, const char *status);
void notify_accused_person_updated(const char *targetUser, int complaintId, const char *category);
void trigger_accused_popup_on_login(const char *logged_in_user);
void notify_grievance_committee(int complaint_id, const char *user_id);
void notify_filing_user(const char *user_id, int complaint_id, const char *category);
void check_user_notifications(const char *username);
void check_and_alert_accused(const char *username);
void send_notification(const char *target_admin, int complaint_id, const char *msg);
// User Authentication & Account Management
int is_id_registered(const char *user_id);
void user_register();
void user_login();
void user_dashboard(const char *username);
void User();
void apply_for_grievance(const char *logged_in_user);
// Ratings & Punishments
void update_admin_rating_record(const char *admin_id, int new_rating);
void rate_resolved_complaint(const char *logged_in_user);
void view_user_punishments(const char *logged_in_user);
// Complaint View Helpers
void viewComplaints(const char *logged_in_user);
void viewAccusedComplaints(const char *logged_in_user);
void viewComplaintsByAssignedTeam(const char *team_name);
// System & String Utilities
void set_win_color(unsigned short color);





//Samiha (252-35-412)
// Password & Recovery System
int recovery_code_exists(const char *user_id);
int save_recovery_code(const char *user_id, const char *recovery_code);
void set_recovery_code(const char *user_id);
void forgot_password();
void force_password_reset(const char *user_id);
int case_insensitive_compare(const char *a, const char *b);

#endif // USER_H
