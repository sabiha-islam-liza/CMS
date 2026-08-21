#ifndef ADMIN_H
#define ADMIN_H
#include "md5.h"
#include "user.h"

// File names
#define ADMIN_FILE "admin.txt"
#define RATINGS_FILE "admin_ratings.txt"
#define NOTIF_FILE "notifications.txt"

// Setup keys
#define setup_key        "syntexerror"
#define General_Key      "generaladmin"
#define ExamAdmin_Key    "examadmin"
#define Accounts_Key     "accountsadmin"
#define LibraryAdmin_Key "libraryadmin"


// Liza(252-35-485)
// Admin functions

void adminregister();
void notify_team(const char *teamName, int complaint_id, const char *status);
void assign_Team(Complaint complaints[], int count, Complaint *c);
void claim_complaint(const char *admin_name);
void update_complaint_status_by_admin(const char *admin_name);
// Dashboards
void assigning_team_admin_dashboard(const char *admin_name);
void departmental_admin_dashboard(const char *admin_name, const char *dept_name, const char *category_filter);
// Sub-admin management
int sub_admin_login(int role_choice, char *logged_user);
void sub_admin_register(int role_choice);
void handle_sub_admin_role(int role_choice, const char *role_title, const char *category_filter);


//Farihana (252-35-532)
void updateStatus();
void deleteComplaint();
void viewComplaintsMenu();
void printComplaint(Complaint c);
void viewAllComplaints();
void viewComplaintsByAssignedTeam(const char *team_name);
void viewComplaintByID();
void viewComplaintsByStatus();
void viewComplaintsByDate();
int getPriorityValue(const char *priority);
void viewComplaintsByPriority();
void viewDepartmentComplaints(const char *categoryFilter);

// Main Admin Portal
void AdminPortalMenu();

#endif
