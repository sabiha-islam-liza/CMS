#ifndef RESOLUTION_TEAM_H
#define RESOLUTION_TEAM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Macros and constants */
#ifndef HASH_LEN
#define HASH_LEN 65
#endif

#ifndef MAX
#define MAX 100
#endif

#ifndef PUNISHMENT_FILE
#define PUNISHMENT_FILE "punishments.txt"
#endif

/* Standardized structures (Ensure these match your global CMS structs) */
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

/* External Helper Function Dependencies (Implemented in main/core module) */
extern void get_user_md5(const char *input, char *output_hash);
extern int load_all_complaints(Complaint complaints[], int *count);
extern int save_complaints(Complaint complaints[], int count);
extern void send_notification(const char *target_admin, int complaint_id, const char *msg);
extern void printComplaint(Complaint c);
extern void updateStatus(const char *teamName);
extern void viewComplaintsByAssignedTeam(const char *teamName);

#ifndef _WIN32
extern int case_insensitive_compare(const char *s1, const char *s2);
#endif

/* Function Declarations */

// Sayma (252-35-468)
void ITMenu(const char *admin_name);
void MaintenanceMenu(const char *admin_name);

// Samiha (252-35-412)
void special_committee_dashboard(void);
void special_committee_view_complaints(void);
void special_committee_decision(void);

// Fariha (252-35-532)
void teamRegister(void);
void resolutionLogin(void);
void addCommentByTeam(const char *teamName);
void ProctorMenu(void);
void giveProctorSuggestion(void);
void viewGrievances(void);
void grievanceRecall(void);
void viewSuggestions(void);
void GrievanceMenu(void);
void StudentDisciplineMenu(void);
void implementPunishment(void);
void ResolutionTeam(void);


#endif
