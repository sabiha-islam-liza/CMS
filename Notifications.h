#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include "common.h"

void send_notification(const char *target_admin, int complaint_id, const char *msg);
void notify_general_admin(int complaint_id, const char *category, const char *user_id);
void notify_assigned_team(const char *teamName, int complaintID, const char *status);
void notify_accused_person_updated(const char *target_user, int complaint_id, const char *category);
void notify_grievance_committee(int complaint_id, const char *user_id);
void notify_filing_user(const char *user_id, int complaint_id, const char *category);
void check_user_notifications(const char *username);
void check_and_alert_accused(const char *username);
void notify_team(const char *teamName, int complaint_id, const char *status);
#endif
