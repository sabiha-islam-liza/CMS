#ifndef USER_H
#define USER_H
#define RATINGS_FILE "admin_ratings.txt"
#define DB_FILE "user.txt"
#define RECOVERY_FILE "user_recovery.txt"
#define PUNISHMENT_FILE "punishments.txt"
// Liza (252-35-485)
void apply_for_grievance(const char *logged_in_user);
int is_id_registered(const char *user_id);
void user_register();
void force_password_reset(const char *user_id);
void update_admin_rating_record(const char *admin_id, int new_rating);
void rate_resolved_complaint(const char *logged_in_user);
void user_dashboard(const char *username);
void user_login();
void User();

// Samiha (252-35-412)
static int case_insensitive_compare(const char *a, const char *b)
int recovery_code_exists(const char *user_id);
int save_recovery_code(const char *user_id, const char *recovery_code);
void set_recovery_code(const char *user_id);
void forgot_password();
void force_password_reset(const char *user_id);
#endif
