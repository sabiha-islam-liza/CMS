#ifndef ADMIN_H
#define ADMIN_H

#include "complaint.h"

void adminregister();
void viewAllComplaints();
void viewComplaintsByAssignedTeam(const char *team_name);
void viewComplaintByID();
void viewComplaintsByStatus();
void viewComplaintsByDate();
int getPriorityValue(const char *priority);
void viewComplaintsByPriority();
void viewDepartmentComplaints(const char *categoryFilter);
void viewComplaintsMenu();
void assign_Team(Complaint complaints[], int count, Complaint *c);
void updateStatus(const char *team_name);
void deleteComplaint();
void claim_complaint(const char *admin_name, const char *category_filter);
void update_complaint_status_by_admin(const char *admin_name);
void assigning_team_admin_dashboard(const char *admin_name);
void departmental_admin_dashboard(const char *admin_name, const char *dept_name, const char *category_filter);
int sub_admin_login(int role_choice, char *logged_user);
void sub_admin_register(int role_choice);
void handle_sub_admin_role(int role_choice, const char *role_title, const char *category_filter);
void AdminPortalMenu();
#endif
