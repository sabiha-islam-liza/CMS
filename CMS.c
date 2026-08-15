#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>


#ifdef _WIN32
#include <windows.h>
#endif
#define ADMIN_FILE "admin.txt"
#define DB_FILE "user.txt"
#define ID_LEN 50
#define PASS_LEN 50
#define HASH_LEN 65
#define setup_key "syntexerror"
#define General_Key "generaladmin"
#define ExamAdmin_Key "examadmin"
#define LibraryAdmin_key "libraryadmin"
#define Accounts_Key "accountsadmin"

#define RATINGS_FILE "admin_ratings.txt"
#define FILE_NAME "complaints.csv"
#define MAX 100


typedef struct {
    uint32_t state[4];
    uint32_t count[2];
    unsigned char buffer[64];
} MD5_CTX_INLINE;

static void MD5Transform(uint32_t state[4], const unsigned char block[64]);

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, x, s, ac) { \
    (a) += F ((b), (c), (d)) + (x) + (uint32_t)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
    (a) += G ((b), (c), (d)) + (x) + (uint32_t)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
    (a) += H ((b), (c), (d)) + (x) + (uint32_t)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
    (a) += I ((b), (c), (d)) + (x) + (uint32_t)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}

void MD5Init_Inline(MD5_CTX_INLINE *context) {
    context->count[0] = context->count[1] = 0;
    context->state[0] = 0x67452301;
    context->state[1] = 0xefcdab89;
    context->state[2] = 0x98badcfe;
    context->state[3] = 0x10325476;
}

void MD5Update_Inline(MD5_CTX_INLINE *context, const unsigned char *input, unsigned int inputLen) {
    unsigned int i, index, partLen;
    index = (unsigned int)((context->count[0] >> 3) & 0x3F);

    if ((context->count[0] += ((uint32_t)inputLen << 3)) < ((uint32_t)inputLen << 3))
        context->count[1]++;
    context->count[1] += ((uint32_t)inputLen >> 29);

    partLen = 64 - index;

    if (inputLen >= partLen) {
        memcpy(&context->buffer[index], input, partLen);
        MD5Transform(context->state, context->buffer);
        for (i = partLen; i + 63 < inputLen; i += 64)
            MD5Transform(context->state, &input[i]);
        index = 0;
    } else {
        i = 0;
    }

    memcpy(&context->buffer[index], &input[i], inputLen - i);
}

void MD5Final_Inline(unsigned char digest[16], MD5_CTX_INLINE *context) {
    unsigned char bits[8];
    unsigned int index, padLen;
    static unsigned char PADDING[64] = { 0x80 };

    for (int i = 0; i < 4; i++) {
        bits[i]     = (unsigned char)((context->count[0] >> (i * 8)) & 0xFF);
        bits[i + 4] = (unsigned char)((context->count[1] >> (i * 8)) & 0xFF);
    }

    index = (unsigned int)((context->count[0] >> 3) & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);
    MD5Update_Inline(context, PADDING, padLen);
    MD5Update_Inline(context, bits, 8);

    for (int i = 0; i < 4; i++) {
        digest[i*4]     = (unsigned char)(context->state[i] & 0xFF);
        digest[i*4 + 1] = (unsigned char)((context->state[i] >> 8) & 0xFF);
        digest[i*4 + 2] = (unsigned char)((context->state[i] >> 16) & 0xFF);
        digest[i*4 + 3] = (unsigned char)((context->state[i] >> 24) & 0xFF);
    }

    memset(context, 0, sizeof(*context));
}

static void MD5Transform(uint32_t state[4], const unsigned char block[64]) {
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3], x[16];

    for (int i = 0, j = 0; j < 64; i++, j += 4)
        x[i] = ((uint32_t)block[j]) | (((uint32_t)block[j+1]) << 8) |
               (((uint32_t)block[j+2]) << 16) | (((uint32_t)block[j+3]) << 24);

    /* Round 1 */
    FF(a, b, c, d, x[ 0],  7, 0xd76aa478); FF(d, a, b, c, x[ 1], 12, 0xe8c7b756);
    FF(c, d, a, b, x[ 2], 17, 0x242070db); FF(b, c, d, a, x[ 3], 22, 0xc1bdceee);
    FF(a, b, c, d, x[ 4],  7, 0xf57c0faf); FF(d, a, b, c, x[ 5], 12, 0x4787c62a);
    FF(c, d, a, b, x[ 6], 17, 0xa8304613); FF(b, c, d, a, x[ 7], 22, 0xfd469501);
    FF(a, b, c, d, x[ 8],  7, 0x698098d8); FF(d, a, b, c, x[ 9], 12, 0x8b44f7af);
    FF(c, d, a, b, x[10], 17, 0xffff5bb1); FF(b, c, d, a, x[11], 22, 0x895cd7be);
    FF(a, b, c, d, x[12],  7, 0x6b901122); FF(d, a, b, c, x[13], 12, 0xfd987193);
    FF(c, d, a, b, x[14], 17, 0xa679438e); FF(b, c, d, a, x[15], 22, 0x49b40821);

    /* Round 2 */
    GG(a, b, c, d, x[ 1],  5, 0xf61e2562); GG(d, a, b, c, x[ 6],  9, 0xc040b340);
    GG(c, d, a, b, x[11], 14, 0x265e5a51); GG(b, c, d, a, x[ 0], 20, 0xe9b6c7aa);
    GG(a, b, c, d, x[ 5],  5, 0xd62f105d); GG(d, a, b, c, x[10],  9, 0x02441453);
    GG(c, d, a, b, x[15], 14, 0xd8a1e681); GG(b, c, d, a, x[ 4], 20, 0xe7d3fbc8);
    GG(a, b, c, d, x[ 9],  5, 0x21e1cde6); GG(d, a, b, c, x[14],  9, 0xc33707d6);
    GG(c, d, a, b, x[ 3], 14, 0xf4d50d87); GG(b, c, d, a, x[ 8], 20, 0x455a14ed);
    GG(a, b, c, d, x[13],  5, 0xa9e3e905); GG(d, a, b, c, x[ 2],  9, 0xfcefa3f8);
    GG(c, d, a, b, x[ 7], 14, 0x676f02d9); GG(b, c, d, a, x[12], 20, 0x8d2a4c8a);

    /* Round 3 */
    HH(a, b, c, d, x[ 5],  4, 0xfffa3942); HH(d, a, b, c, x[ 8], 11, 0x8771f681);
    HH(c, d, a, b, x[11], 16, 0x6d9d6122); HH(b, c, d, a, x[14], 23, 0xfde5380c);
    HH(a, b, c, d, x[ 1],  4, 0xa4beea44); HH(d, a, b, c, x[ 4], 11, 0x4bdecfa9);
    HH(c, d, a, b, x[ 7], 16, 0xf6bb4b60); HH(b, c, d, a, x[10], 23, 0xbebfbc70);
    HH(a, b, c, d, x[13],  4, 0x289b7ec6); HH(d, a, b, c, x[ 0], 11, 0xeaa127fa);
    HH(c, d, a, b, x[ 3], 16, 0xd4ef3085); HH(b, c, d, a, x[ 6], 23, 0x04881d05);
    HH(a, b, c, d, x[ 9],  4, 0xd9d4d039); HH(d, a, b, c, x[12], 11, 0xe6db99e5);
    HH(c, d, a, b, x[15], 16, 0x1fa27cf8); HH(b, c, d, a, x[ 2], 23, 0xc4ac5665);

    /* Round 4 */
    II(a, b, c, d, x[ 0],  6, 0xf4292244); II(d, a, b, c, x[ 7], 10, 0x432aff97);
    II(c, d, a, b, x[14], 15, 0xab9423a7); II(b, c, d, a, x[ 5], 21, 0xfc93a039);
    II(a, b, c, d, x[12],  6, 0x655b59c3); II(d, a, b, c, x[ 3], 10, 0x8f0ccc92);
    II(c, d, a, b, x[10], 15, 0xffeff47d); II(b, c, d, a, x[ 1], 21, 0x85845dd1);
    II(a, b, c, d, x[ 8],  6, 0x6fa87e4f); II(d, a, b, c, x[15], 10, 0xfe2ce6e0);
    II(c, d, a, b, x[ 6], 15, 0xa3014314); II(b, c, d, a, x[13], 21, 0x4e0811a1);
    II(a, b, c, d, x[ 4],  6, 0xf7537e82); II(d, a, b, c, x[11], 10, 0xbd3af235);
    II(c, d, a, b, x[ 2], 15, 0x2ad7d2bb); II(b, c, d, a, x[ 9], 21, 0xeb86d391);

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    memset(x, 0, sizeof(x));
}

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
    char assignedAdmin[50]; // NEW: Admin username handling the complaint
    int rating;             // NEW: Rating (0 = unrated, 1-5 = rated)
} Complaint;


/* Resolution Team Function Declarations */

void ResolutionTeam();
void resolutionLogin();
void teamRegister();

void ITMenu(const char *admin_name);
void MaintenanceMenu(const char *admin_name);
void ProctorMenu();
void GrievanceMenu();
void StudentDisciplineMenu();

void giveProctorSuggestion();
void viewGrievances();
void grievanceRecall();
void viewSuggestions();
void implementPunishment();

void special_committee_view_complaints();
void special_committee_decision();
void special_committee_dashboard();

int generateComplaintID() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) return 1;

    int max_id = 0;
    int id;
    char buffer[500];
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (sscanf(buffer, "%d,", &id) == 1) {
            if (id > max_id) {
                max_id = id;
            }
        }
    }
    fclose(fp);
    return max_id + 1;
}


/* Load all complaints from file */

int load_all_complaints(Complaint complaints[], int *count) {
    FILE *fp = fopen("complaints.csv", "r");
    if (!fp) return 0;

    *count = 0;
    while (fscanf(fp, "%d,%49[^,],%49[^,],%49[^,],%199[^,],%19[^,],%19[^,],%19[^,],%49[^,],%49[^,],%d\n",
                  &complaints[*count].id,
                  complaints[*count].user,
                  complaints[*count].targetUser,
                  complaints[*count].category,
                  complaints[*count].description,
                  complaints[*count].priority,
                  complaints[*count].status,
                  complaints[*count].date,
                  complaints[*count].assignedTeam,
                  complaints[*count].assignedAdmin,
                  &complaints[*count].rating) == 11) {
        (*count)++;
    }

    fclose(fp);
    return 1;
}
int save_complaints(Complaint complaints[], int count) {
    FILE *fp = fopen("complaints.csv", "w"); // Ensure file name matches exactly
    if (!fp) {
        printf("\n[!] Error opening file for writing! (Make sure complaints.csv is not open in Excel)\n");
        return 0; // Failure
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d\n",
                complaints[i].id,
                complaints[i].user,
                complaints[i].targetUser,
                complaints[i].category,
                complaints[i].description,
                complaints[i].priority,
                complaints[i].status,
                complaints[i].date,
                complaints[i].assignedTeam,
                complaints[i].assignedAdmin,
                complaints[i].rating);
    }

    fclose(fp);
    return 1; // Success
}
#ifndef NOTIF_FILE
#define NOTIF_FILE "notifications.txt"
#endif

// Base notification logger
void send_notification(const char *target_admin, int complaint_id, const char *msg) {
    FILE *fp = fopen(NOTIF_FILE, "a");
    if (!fp) return;
    fprintf(fp, "%s|%d|%s|0\n", target_admin, complaint_id, msg);
    fclose(fp);
}

// 1. Notify General Admin
void notify_general_admin(int complaint_id, const char *category, const char *user_id) {
    char message[256];
    snprintf(message, sizeof(message), "New %s Complaint #%d filed by User: %s", category, complaint_id, user_id);
    send_notification("General Admin", complaint_id, message);

#ifdef _WIN32
    MessageBeep(MB_ICONINFORMATION);
    MessageBox(NULL, message, "General Admin Alert", MB_OK | MB_ICONINFORMATION);
#endif
}

// 2. Notify Assigned Team (Proctor, Special Committee, IT Support, etc.)
void notify_assigned_team(const char *teamName, int complaintID, const char *status) {

    FILE *fp = fopen("notifications.txt", "a");
    if (!fp) {
        printf("\n[!] Error: Could not open notifications.txt\n");
        return;
    }

    fprintf(fp, "%s|New complaint #%d assigned to your team (Status: %s)\n",
            teamName, complaintID, status);

    fclose(fp);
char alert_msg[256];
    snprintf(alert_msg, sizeof(alert_msg), "%s|New complaint #%d assigned to your team (Status: %s)\n",
            teamName, complaintID, status);
    send_notification(teamName, complaintID, alert_msg);

#ifdef _WIN32
    char msg[256];
    snprintf(msg, sizeof(msg), "ATTENTION: You have confidential notices or complaints filed against your Registration ID!\n\nPlease check Option 3 in your dashboard.");
    MessageBeep(MB_ICONWARNING);
    MessageBox(NULL, msg, "Confidential Notice Alert", MB_OK | MB_ICONWARNING);
#endif
}

// 3. Notify Accused Person (Targeted Warning Popup)
void notify_accused_person_updated(const char *target_user, int complaint_id, const char *category) {
    char filename[100];
    snprintf(filename, sizeof(filename), "notice_%s.txt", target_user);
    FILE *fp = fopen(filename, "a");
    if (fp) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        fprintf(fp, "[%04d-%02d-%02d] OFFICIAL NOTICE: Complaint #%d (%s) has been filed regarding your account/id.\n",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, complaint_id, category);
        fclose(fp);
    }

    char alert_msg[256];
    snprintf(alert_msg, sizeof(alert_msg), "A serious %s complaint #%d has been filed regarding your ID.", category, complaint_id);
    send_notification(target_user, complaint_id, alert_msg);

#ifdef _WIN32
    char msg[256];
    snprintf(msg, sizeof(msg), "ATTENTION: You have confidential notices or complaints filed against your Registration ID!\n\nPlease check Option 3 in your dashboard.");
    MessageBeep(MB_ICONWARNING);
    MessageBox(NULL, msg, "Confidential Notice Alert", MB_OK | MB_ICONWARNING);
#endif
}

// 4. Notify Grievance Committee
void notify_grievance_committee(int complaint_id, const char *user_id) {
    char message[256];
    snprintf(message, sizeof(message), "GRIEVANCE ALERT: New Complaint/Grievance #%d filed by %s requiring review.", complaint_id, user_id);
    send_notification("Grievance Committee", complaint_id, message);

#ifdef _WIN32
    MessageBeep(MB_ICONWARNING);
    MessageBox(NULL, message, "Grievance Committee Alert", MB_OK | MB_ICONWARNING);
#endif
}

// 5. Notify Filing User (The person who filed/accused)
void notify_filing_user(const char *user_id, int complaint_id, const char *category) {
    char message[256];
    snprintf(message, sizeof(message), "Confirmation: Your %s complaint #%d has been submitted successfully.", category, complaint_id);
    send_notification(user_id, complaint_id, message);

#ifdef _WIN32
    char title[100];
    snprintf(title, sizeof(title), "Complaint Confirmation - #%d", complaint_id);
    MessageBeep(MB_ICONINFORMATION);
    MessageBox(NULL, message, title, MB_OK | MB_ICONINFORMATION);
#endif
}
void check_user_notifications(const char *username) {
    FILE *fp = fopen("notifications.txt", "r");
    if (!fp) {
        return; // No notifications file exists yet
    }

    FILE *temp = fopen("notifications_temp.txt", "w");
    if (!temp) {
        fclose(fp);
        return;
    }

    char line[512];
    char target[50];
    int complaint_id;
    char message[256];
    int is_read;
    int unread_count = 0;

    printf("\n=========================================\n");
    printf("         UNREAD NOTIFICATIONS            \n");
    printf("=========================================\n");

    while (fgets(line, sizeof(line), fp)) {
        // Parse line format: target_user|complaint_id|message|is_read
        if (sscanf(line, "%49[^|]|%d|%255[^|]|%d\n", target, &complaint_id, message, &is_read) == 4) {
            // Check if notification belongs to current user/role AND is unread (0)
            if (strcmp(target, username) == 0 && is_read == 0) {
                unread_count++;
                printf("[%d] Complaint #%d: %s\n", unread_count, complaint_id, message);

                // Mark as read (1) when copying back
                fprintf(temp, "%s|%d|%s|1\n", target, complaint_id, message);
            } else {
                // Keep unchanged for other users or already read items
                fprintf(temp, "%s|%d|%s|%d\n", target, complaint_id, message, is_read);
            }
        }
    }

    if (unread_count == 0) {
        printf("No new notifications.\n");
    }
    printf("=========================================\n\n");

    fclose(fp);
    fclose(temp);

    // Update notifications.txt with updated read flags
    remove("notifications.txt");
    rename("notifications_temp.txt", "notifications.txt");
}
void check_and_alert_accused(const char *username) {
    char filename[100];
    snprintf(filename, sizeof(filename), "notice_%s.txt", username);

    FILE *fp = fopen(filename, "r");
    if (fp) {
        printf("\n=========================================\n");
        printf("  [!] CONFIDENTIAL LEGAL/NOTICE ALERT    \n");
        printf("=========================================\n");

        char line[256];
        while (fgets(line, sizeof(line), fp)) {
            printf("%s", line);
        }
        printf("=========================================\n\n");
        fclose(fp);

        #ifdef _WIN32
            MessageBeep(MB_ICONWARNING);
            MessageBox(NULL,
                       "ATTENTION: You have confidential notices or complaints filed regarding your account!",
                       "Confidential Notice Alert",
                       MB_OK | MB_ICONWARNING);
        #endif
    }
}
/*
#define NOTIF_FILE "notifications.txt"

void send_notification(const char *target_admin, int complaint_id, const char *msg) {
    FILE *fp = fopen(NOTIF_FILE, "a");
    if (!fp) return;

    // Record format: TargetAdmin|ComplaintID|Message|IsRead(0=Unread, 1=Read)
    fprintf(fp, "%s|%d|%s|0\n", target_admin, complaint_id, msg);
    fclose(fp);
}
// Helper function to append notification records for the Special Committee
void notify_special_committee(int complaint_id, const char *category, const char *accused_user) {
    FILE *fp = fopen("special_committee_notices.txt", "a");
    if (fp == NULL) {
        printf("\n[!] Error: Unable to record notification for Special Committee.\n");
        return;
    }

    // Get current timestamp
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char date_str[20];
    sprintf(date_str, "%04d-%02d-%02d %02d:%02d",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min);

    fprintf(fp, "[%s] ALERT: New %s Complaint #%d filed against User ID: %s\n",
            date_str, category, complaint_id, accused_user);

    fclose(fp);
}
void check_admin_notifications(const char *admin_identifier) {
    FILE *fp = fopen(NOTIF_FILE, "r");
    if (!fp) return;

    typedef struct {
        char target[50];
        int complaint_id;
        char message[256];
        int is_read;
    } Notification;

    Notification notifs[MAX];
    int count = 0;
    int unread_count = 0;

    // Read all notifications
    while (fscanf(fp, "%49[^|]|%d|%255[^|]|%d\n",
                  notifs[count].target,
                  &notifs[count].complaint_id,
                  notifs[count].message,
                  &notifs[count].is_read) == 4) {

        if (strcmp(notifs[count].target, admin_identifier) == 0 && notifs[count].is_read == 0) {
            unread_count++;
        }
        count++;
    }
    fclose(fp);

    if (unread_count == 0) return;

    // Display Windows API Popup for each unread notification
    for (int i = 0; i < count; i++) {
        if (strcmp(notifs[i].target, admin_identifier) == 0 && notifs[i].is_read == 0) {
#ifdef _WIN32
            char title[100];
            snprintf(title, sizeof(title), "Admin Alert - Complaint #%d", notifs[i].complaint_id);

            MessageBeep(MB_ICONWARNING);
            MessageBox(NULL, notifs[i].message, title, MB_OK | MB_ICONWARNING);
#else
            printf("\n[ADMIN ALERT] %s\n", notifs[i].message);
#endif
            notifs[i].is_read = 1; // Mark as read
        }
    }

    // Rewrite file to update read status
    fp = fopen(NOTIF_FILE, "w");
    if (!fp) return;

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s|%d|%s|%d\n",
                notifs[i].target,
                notifs[i].complaint_id,
                notifs[i].message,
                notifs[i].is_read);
    }
    fclose(fp);
}

void check_resolution_team_notifications() {
    FILE *fp = fopen(NOTIF_FILE, "r");
    if (!fp) return;

    typedef struct {
        char target[50];
        int complaint_id;
        char message[256];
        int is_read;
    } Notification;

    Notification notifs[MAX];
    int count = 0;
    int unread_count = 0;

    // Read pending notifications
    while (fscanf(fp, "%49[^|]|%d|%255[^|]|%d\n",
                  notifs[count].target,
                  &notifs[count].complaint_id,
                  notifs[count].message,
                  &notifs[count].is_read) == 4) {

        if (strcmp(notifs[count].target, "Resolution Team") == 0 && notifs[count].is_read == 0) {
            unread_count++;
        }
        count++;
    }
    fclose(fp);

    if (unread_count == 0) return;

    // Display Windows API Popup for each unread notification
    for (int i = 0; i < count; i++) {
        if (strcmp(notifs[i].target, "Resolution Team") == 0 && notifs[i].is_read == 0) {
#ifdef _WIN32
            char title[100];
            snprintf(title, sizeof(title), "Resolution Team Alert - Complaint #%d", notifs[i].complaint_id);

            MessageBeep(MB_ICONEXCLAMATION);
            MessageBox(NULL, notifs[i].message, title, MB_OK | MB_ICONEXCLAMATION);
#else
            printf("\n[RESOLUTION ALERT] %s\n", notifs[i].message);
#endif
            notifs[i].is_read = 1; // Mark as read
        }
    }

    // Rewrite notification file with updated read statuses
    fp = fopen(NOTIF_FILE, "w");
    if (!fp) return;

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s|%d|%s|%d\n",
                notifs[i].target,
                notifs[i].complaint_id,
                notifs[i].message,
                notifs[i].is_read);
    }
    fclose(fp);
}


void notify_accused_person(const char *target_user, int complaint_id, const char *category) {
    char filename[100];
    sprintf(filename, "notice_%s.txt", target_user);
    FILE *fp = fopen(filename, "a");
    if (!fp) return;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(fp, "[%04d-%02d-%02d] OFFICIAL NOTICE: Complaint #%d (%s) has been filed regarding your account/id.\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, complaint_id, category);
    fclose(fp);
}
void notify_proctor_office(int complaint_id, const char *category, const char *accused_user) {
    // 1. Append to Proctor's dedicated audit text file
    FILE *fp = fopen("proctor_notices.txt", "a");
    if (fp) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        fprintf(fp, "[%04d-%02d-%02d] PROCTOR ALERT: Serious %s Complaint #%d filed against User ID: %s\n",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, category, complaint_id, accused_user);
        fclose(fp);
    }

    // 2. Queue live notification for Proctor's portal login
    char alert_msg[256];
    snprintf(alert_msg, sizeof(alert_msg), "PROCTOR ALERT: Serious %s complaint filed against User: %s", category, accused_user);

    // Target both specific account name or general "ProctorOffice" role
    send_notification("ProctorOffice", complaint_id, alert_msg);
}
*/

// Function to directly apply for a grievance without requiring target ID input
void apply_for_grievance(const char *logged_in_user) {
    // Step 1: Check punishments.txt
    FILE *pf = fopen("punishments.txt", "r");
    if (!pf) {
        printf("\n[!] Punishment records not found. Cannot apply for grievance.\n");
        return;
    }

    int has_punishment = 0;
    char line[256], punished_user[50], reason[200];
    while (fgets(line, sizeof(line), pf)) {
        // Example format in punishments.txt: username,reason
        if (sscanf(line, "%49[^,],%199[^\n]", punished_user, reason) == 2) {
            if (strcmp(punished_user, logged_in_user) == 0) {
                has_punishment = 1;
                break;
            }
        }
    }
    fclose(pf);

    if (!has_punishment) {
        printf("\n[!] You have no punishment record. Only punished users can apply for grievance.\n");
        return;
    }

    // Step 2: Load existing complaints
    Complaint complaints[MAX];
    int count = 0;
    load_all_complaints(complaints, &count);

    if (count >= MAX) {
        printf("\n[!] System storage is full. Cannot submit grievance.\n");
        return;
    }

    // Step 3: Create new grievance complaint
    Complaint c;
    c.id = (count == 0) ? 101 : complaints[count - 1].id + 1;
    strcpy(c.user, logged_in_user);
    strcpy(c.targetUser, "N/A");
    strcpy(c.category, "Grievance");
    strcpy(c.priority, "Medium");
    strcpy(c.status, "Open");
    strcpy(c.assignedTeam, "Grievance Committee");
    strcpy(c.assignedAdmin, "Unassigned");
    c.rating = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(c.date, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    while (getchar() != '\n'); // clear buffer
    printf("\n================ APPLY FOR GRIEVANCE ================\n");
    printf("Enter detailed grievance description (Academic / Administrative): ");
    fgets(c.description, sizeof(c.description), stdin);
    c.description[strcspn(c.description, "\n")] = '\0';

    // Step 4: Save complaint
    complaints[count] = c;
    save_complaints(complaints, count + 1);

    // Step 5: Notifications
    char admin_notif[256];
    snprintf(admin_notif, sizeof(admin_notif), "New Grievance #%d filed by %s.", c.id, c.user);
    send_notification("General Admin", c.id, admin_notif);

    char committee_notif[256];
    snprintf(committee_notif, sizeof(committee_notif),
             "GRIEVANCE ALERT: New Grievance #%d filed by %s requiring review.", c.id, c.user);
    send_notification("Grievance Committee", c.id, committee_notif);

    printf("\n[SUCCESS] Grievance #%d submitted successfully and routed to Grievance Committee.\n", c.id);
}

void FileComplaint(const char *logged_in_user) {
    Complaint complaints[MAX];
    int count = 0;
    load_all_complaints(complaints, &count);

    if (count >= MAX) {
        printf("\n[!] Error: System storage full. Cannot file more complaints.\n");
        return;
    }

    Complaint c;
    c.id = (count == 0) ? 101 : complaints[count - 1].id + 1;

    strcpy(c.user, logged_in_user);
    strcpy(c.targetUser, "N/A");
    strcpy(c.status, "Open");
    strcpy(c.assignedAdmin, "Unassigned");
    c.rating = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(c.date, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    int cat_choice;
    printf("\nSelect Complaint Category:\n");
    printf("1. Technical Issue\n");
    printf("2. Accounts\n");
    printf("3. Maintenance\n");
    printf("4. Library Problem\n");
    printf("5. Exam Related Issue\n");
    printf("6. Violence (Targeted)\n");
    printf("7. Sexual Harassment (Targeted)\n");
    printf("8. Grievance (Academic/Admin)\n");
    printf("Enter choice: ");

    if (scanf("%d", &cat_choice) != 1) {
        while (getchar() != '\n');
        printf("\n[!] Invalid choice. Returning to menu.\n");
        return;
    }
    while (getchar() != '\n');

    /* FIX: Handle grievance option early and cleanly */
    if (cat_choice == 8) {
        apply_for_grievance(logged_in_user);
        return;
    }

    if (cat_choice == 6 || cat_choice == 7) {
        if (cat_choice == 6) {
            strcpy(c.category, "Violence");
            strcpy(c.assignedTeam, "Proctor");
        } else {
            strcpy(c.category, "Sexual Harassment");
            strcpy(c.assignedTeam, "SpecialCommittee");
        }

        strcpy(c.priority, "High");

        printf("\nEnter Registration ID of the person to file against: ");
        scanf("%49s", c.targetUser);
        while (getchar() != '\n');

        printf("Enter Incidents / Details: ");
        fgets(c.description, sizeof(c.description), stdin);
        c.description[strcspn(c.description, "\n")] = '\0';

        complaints[count] = c;
        save_complaints(complaints, count + 1);

        if (strcmp(c.targetUser, "N/A") != 0 && strcmp(c.targetUser, logged_in_user) != 0) {
            notify_accused_person_updated(c.targetUser, c.id, c.category);
        }
        notify_assigned_team(c.assignedTeam, c.id, c.status);
    } else {
        if (cat_choice == 1) {
            strcpy(c.category, "Technical");
            strcpy(c.assignedTeam, "IT Support");
            strcpy(c.priority, "Low");
        } else if (cat_choice == 2) {
            strcpy(c.category, "Accounts");
            strcpy(c.assignedTeam, "Accounts Admin");
            strcpy(c.priority, "Medium");
        } else if (cat_choice == 3) {
            strcpy(c.category, "Maintenance");
            strcpy(c.assignedTeam, "Maintenance Team");
            strcpy(c.priority, "Low");
        } else if (cat_choice == 4) {
            strcpy(c.category, "Library");
            strcpy(c.assignedTeam, "Library Admin");
            strcpy(c.priority, "Low");
        } else if (cat_choice == 5) {
            strcpy(c.category, "Exam");
            strcpy(c.assignedTeam, "Exam Head");
            strcpy(c.priority, "High");
        } else {
            printf("\n[!] Invalid menu choice.\n");
            return;
        }

        printf("Enter Description: ");
        fgets(c.description, sizeof(c.description), stdin);
        c.description[strcspn(c.description, "\n")] = '\0';

        complaints[count] = c;
        save_complaints(complaints, count + 1);

        notify_general_admin(c.id, c.category, c.user);
        notify_assigned_team(c.assignedTeam, c.id, c.status);
    }

    printf("\n[SUCCESS] %s complaint #%d submitted successfully.\n", c.category, c.id);
}
// Check if Registration ID exists in user database
int is_id_registered(const char *user_id) {
    FILE *fp = fopen(DB_FILE, "r");
    if (!fp) return 0; // File doesn't exist yet

    char file_id[ID_LEN], file_hash[HASH_LEN];
    int flag;

    while (fscanf(fp, "%s %s %d", file_id, file_hash, &flag) == 3) {
        if (strcmp(file_id, user_id) == 0) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}


// Helper function to convert text to MD5 hex string
void get_user_md5(const char *input, char *output_hash) {
    unsigned char digest[16];
    MD5_CTX_INLINE ctx;

    MD5Init_Inline(&ctx);
    MD5Update_Inline(&ctx, (const unsigned char*)input, strlen(input));
    MD5Final_Inline(digest, &ctx);

    for (int i = 0; i < 16; i++) {
        sprintf(output_hash + (i * 2), "%02x", digest[i]);
    }
    output_hash[32] = '\0';
}

// Register a new user with default password set as their Registration ID
void user_register() {
    char user_id[ID_LEN];
    char hashed_default_pass[HASH_LEN];

    printf("\n================ REGISTER =================\n");
    printf("Enter your Registration ID (e.g., 252-35-485): ");

    if (scanf("%49s", user_id) != 1) {
        printf("\n[!] Invalid input. Returning to menu...\n");
        while (getchar() != '\n'); // Clear input buffer
        return; // Returns back to User menu
    }

    // Check if the Registration ID already exists in DB_FILE
    if (is_id_registered(user_id)) {
        printf("\n[!] Error: Registration ID '%s' is already registered!\n", user_id);
        printf("Returning to menu...\n");
        return; // Returns back to User menu
    }

    // Hash the default password (set to Registration ID)
    get_user_md5(user_id, hashed_default_pass);

    FILE *file = fopen(DB_FILE, "a");
    if (!file) {
        printf("\n[!] Error: Unable to access user database file.\n");
        printf("Returning to menu...\n");
        return; // Returns back to User menu
    }

    // Save record format: USER_ID MD5_HASH FIRST_LOGIN_FLAG (1)
    fprintf(file, "%s %s 1\n", user_id, hashed_default_pass);
    fclose(file);

    printf("\n[SUCCESS] User registered successfully!\n");
    printf("Your initial password is set to: %s\n", user_id);
    printf("Note: You will be required to change your password on first login.\n");
    printf("===========================================\n");

    printf("\nRegistration complete. Returning to main menu...\n");
    return; // Returns back to User menu
}
// Enforce new password on first login
void force_password_reset(const char *user_id) {
    char new_pass[PASS_LEN], confirm_pass[PASS_LEN];
    char new_hash[HASH_LEN];


    printf("\n[SECURITY NOTICE] First Login Detected!!!");
    printf("\nYou must set a new private password before continuing.");


    while (1) {
        printf("\nEnter new password: ");
        scanf("%49s", new_pass);
        printf("Confirm new password: ");
        scanf("%49s", confirm_pass);

        if (strcmp(new_pass, confirm_pass) == 0) {
            break;
        }
        printf("\n[!] Passwords do not match. Please try again.\n");
    }

    // Generate MD5 hash for the new password
    get_user_md5(new_pass, new_hash);

    FILE *fp = fopen(DB_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("\nError updating security credentials.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    char file_id[ID_LEN], file_hash[HASH_LEN];
    int first_login_flag;

    while (fscanf(fp, "%s %s %d", file_id, file_hash, &first_login_flag) == 3) {
        if (strcmp(file_id, user_id) == 0) {
            // Update hash and set first_login_flag to 0
            fprintf(temp, "%s %s 0\n", file_id, new_hash);
        } else {
            fprintf(temp, "%s %s %d\n", file_id, file_hash, first_login_flag);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(DB_FILE);
    rename("temp.txt", DB_FILE);

    printf("\n[SUCCESS] Password updated successfully!");
    printf("\nPlease use your new password for all future logins.\n");
}

/*void check_and_alert_accused(const char *logged_in_user) {
    char file_name[100];
    sprintf(file_name, "notice_%s.txt", logged_in_user);
    FILE *fp = fopen(file_name, "r");

    if (fp != NULL) {
        fclose(fp); // File exists, which means notices are present

#ifdef _WIN32
        MessageBeep(MB_ICONWARNING);
        MessageBox(NULL,
                   "ATTENTION: You have confidential notices or complaints filed against your Registration ID!\n\nPlease check Option 3 in your dashboard.",
                   "Confidential Notice Alert",
                   MB_OK | MB_ICONWARNING);
#else
        printf("\n\n****************************************************************");
        printf("\n[!] ALERT: You have confidential notices/complaints filed against you!");
        printf("\nPlease view Option 3 in the main menu for details.");
        printf("\n****************************************************************\n");
#endif
    }
}*/
/*void check_user_notifications(const char *username) {
    FILE *fp = fopen(NOTIF_FILE, "r");
    if (!fp) return;

    typedef struct {
        char target[50];
        int complaint_id;
        char message[256];
        int is_read;
    } Notification;

    Notification notifs[MAX];
    int count = 0;
    int unread_count = 0;

    // Read pending user notifications
    while (fscanf(fp, "%49[^|]|%d|%255[^|]|%d\n",
                  notifs[count].target,
                  &notifs[count].complaint_id,
                  notifs[count].message,
                  &notifs[count].is_read) == 4) {

        if (strcmp(notifs[count].target, username) == 0 && notifs[count].is_read == 0) {
            unread_count++;
        }
        count++;
    }
    fclose(fp);

    if (unread_count == 0) return;

    // Display Windows API Popup for each unread notification
    for (int i = 0; i < count; i++) {
        if (strcmp(notifs[i].target, username) == 0 && notifs[i].is_read == 0) {
#ifdef _WIN32
            char title[100];
            snprintf(title, sizeof(title), "Notification - Complaint #%d", notifs[i].complaint_id);

            MessageBeep(MB_ICONINFORMATION);
            MessageBox(NULL, notifs[i].message, title, MB_OK | MB_ICONINFORMATION);
#else
            printf("\n[NOTIFICATION] %s\n", notifs[i].message);
#endif
            notifs[i].is_read = 1; // Mark as read
        }
    }

    // Rewrite notification file with updated read statuses
    fp = fopen(NOTIF_FILE, "w");
    if (!fp) return;

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s|%d|%s|%d\n",
                notifs[i].target,
                notifs[i].complaint_id,
                notifs[i].message,
                notifs[i].is_read);
    }
    fclose(fp);
}


*/

#define PUNISHMENT_FILE "punishments.txt"

void view_user_punishments(const char *logged_in_user) {
    FILE *fp = fopen(PUNISHMENT_FILE, "r");
    if (!fp) {
        printf("\n================ OFFICIAL RESOLUTION & DISCIPLINARY NOTICES ================");
        printf("\nNo formal notices or punishments found.\n");
        printf("============================================================================\n");
        return;
    }

    typedef struct {
        int complaint_id;
        char target_user[50];
        char action_type[50];   // e.g., "Warning", "Account Suspension", "Fine", "Proctorial Inquiry"
        char reason[256];
        char date[20];
    } PunishmentRecord;

    PunishmentRecord record;
    int found = 0;

    printf("\n================ OFFICIAL RESOLUTION & DISCIPLINARY NOTICES ================\n");

    // File Format: ComplaintID|TargetUser|ActionType|Reason|Date
    while (fscanf(fp, "%d|%49[^|]|%49[^|]|%255[^|]|%19[^\n]\n",
                  &record.complaint_id,
                  record.target_user,
                  record.action_type,
                  record.reason,
                  record.date) == 5) {

        if (strcmp(record.target_user, logged_in_user) == 0) {
            found++;
            printf("\n Notice #%d", found);
            printf("\n ----------------------------------------------------");
            printf("\n Date Issued   : %s", record.date);
            printf("\n Complaint ID  : #%d", record.complaint_id);
            printf("\n Action Taken  : %s", record.action_type);
            printf("\n Details/Reason: %s", record.reason);
            printf("\n ----------------------------------------------------\n");
        }
    }

    if (found == 0) {
        printf("No disciplinary actions or warnings have been recorded for account: %s\n", logged_in_user);
    } else {
        printf("Total Active Notices: %d\n", found);
    }

    printf("============================================================================\n");
    fclose(fp);
}


void update_admin_rating_record(const char *admin_id, int new_rating) {
    if (admin_id == NULL || strlen(admin_id) == 0 ||
        strcmp(admin_id, "Unassigned") == 0 || strcmp(admin_id, "N/A") == 0) {
        return;
    }

    // Step 1: Look up the assigned admin's category/role from admin.txt
    char category[50] = "General";
    FILE *admin_fp = fopen(ADMIN_FILE, "r");
    if (admin_fp) {
        char name[50], pass[HASH_LEN];
        int role;
        while (fscanf(admin_fp, "%49s %64s %d", name, pass, &role) >= 3) {
            if (strcmp(name, admin_id) == 0) {
                switch (role) {
                    case 1: strcpy(category, "General Admin"); break;
                    case 2: strcpy(category, "Exam Office"); break;
                    case 3: strcpy(category, "Library Admin"); break;
                    case 4: strcpy(category, "Accounts Office"); break;
                    default: strcpy(category, "General Admin"); break;
                }
                break;
            }
        }
        fclose(admin_fp);
    }

    // Step 2: Read existing records into temporary memory array
    typedef struct {
        char admin_name[50];
        char category[50];
        int total_sum;
        int rating_count;
        float avg_rating;
    } AdminRating;

    AdminRating records[MAX];
    int record_count = 0;
    int found = 0;

    FILE *rfp = fopen(RATINGS_FILE, "r");
    if (rfp) {
        // Parsing format: AdminName|Category|TotalSum|Count|AvgRating
        while (fscanf(rfp, "%49[^|]|%49[^|]|%d|%d|%f\n",
                      records[record_count].admin_name,
                      records[record_count].category,
                      &records[record_count].total_sum,
                      &records[record_count].rating_count,
                      &records[record_count].avg_rating) == 5) {

            if (strcmp(records[record_count].admin_name, admin_id) == 0) {
                records[record_count].total_sum += new_rating;
                records[record_count].rating_count += 1;
                records[record_count].avg_rating = (float)records[record_count].total_sum / records[record_count].rating_count;
                strcpy(records[record_count].category, category);
                found = 1;
            }
            record_count++;
        }
        fclose(rfp);
    }

    // Step 3: Append as a new record if this is the admin's first rating
    if (!found && record_count < MAX) {
        strcpy(records[record_count].admin_name, admin_id);
        strcpy(records[record_count].category, category);
        records[record_count].total_sum = new_rating;
        records[record_count].rating_count = 1;
        records[record_count].avg_rating = (float)new_rating;
        record_count++;
    }

    // Step 4: Overwrite admin_ratings.txt with the updated data
    rfp = fopen(RATINGS_FILE, "w");
    if (!rfp) {
        printf("\n[!] Error opening ratings file for writing.\n");
        return;
    }

    for (int i = 0; i < record_count; i++) {
        fprintf(rfp, "%s|%s|%d|%d|%.2f\n",
                records[i].admin_name,
                records[i].category,
                records[i].total_sum,
                records[i].rating_count,
                records[i].avg_rating);
    }
    fclose(rfp);
}
void rate_resolved_complaint(const char *logged_in_user) {
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints available.\n");
        return;
    }

    int eligible = 0;
    printf("\n========== RATE RESOLVED SERVICES ==========\n");
    for (int i = 0; i < count; i++) {
       // Simplify the rating eligibility check to only target Closed complaints:
if (strcmp(complaints[i].user, logged_in_user) == 0 &&
    strcasecmp(complaints[i].status, "Closed") == 0)
    // Allow user rating
 {
            printf("ID: %d | Category: %s | Handled By Admin: %s | Current Rating: %d/5\n",
                   complaints[i].id, complaints[i].category, complaints[i].assignedAdmin, complaints[i].rating);
            eligible++;
        }
    }

    if (eligible == 0) {
        printf("No resolved or closed complaints available for rating.\n");
        return;
    }

    int target_id;
    printf("\nEnter Complaint ID to rate: ");
    if (scanf("%d", &target_id) != 1) {
        while (getchar() != '\n');
        return;
    }

    for (int i = 0; i < count; i++) {
        if (complaints[i].id == target_id && strcmp(complaints[i].user, logged_in_user) == 0) {
            int rating_val;
            printf("Enter Rating for Admin '%s' (1 to 5): ", complaints[i].assignedAdmin);
            scanf("%d", &rating_val);

            if (rating_val < 1 || rating_val > 5) {
                printf("[!] Invalid rating. Must be between 1 and 5.\n");
                return;
            }

            // Save rating into the complaint ticket (complaints.csv)
            complaints[i].rating = rating_val;
            save_complaints(complaints, count);

            // Update cumulative totals and averages in admin_ratings.txt
            update_admin_rating_record(complaints[i].assignedAdmin, rating_val);

            printf("[SUCCESS] Rating %d/5 saved. Thank you for your feedback!\n", rating_val);
            return;
        }
    }

    printf("[!] Complaint ID not found or access denied.\n");
}

#ifdef _WIN32
#include <windows.h>
#endif

// Helper function to set terminal text color via Windows API
void set_win_color(unsigned short color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
#endif
}

// 1. viewComplaints: Displays complaints filed BY the user in standard formatted console output
void viewComplaints(const char *logged_in_user) {
    Complaint complaints[MAX];
    int count = 0;

    // Reset color
    set_win_color(7);

    if (!load_all_complaints(complaints, &count) || count == 0) {
        set_win_color(12); // Red text
        printf("\n[!] No complaints found in the system.\n");
        set_win_color(7);
        return;
    }

    int found = 0;

    set_win_color(11); // Light Cyan
    printf("\n========================================================");
    printf("\n             MY FILED COMPLAINTS LIST                   ");
    printf("\n========================================================\n");
    set_win_color(7);

    for (int i = 0; i < count; i++) {
        if (strcmp(complaints[i].user, logged_in_user) == 0) {
            found++;

            set_win_color(14); // Yellow header
            printf("\n--- [ Complaint ID: #%d ] ---", complaints[i].id);
            set_win_color(7);

            printf("\n Category      : %s", complaints[i].category);
            printf("\n Priority      : %s", complaints[i].priority);

            // Highlight status based on resolution state
            printf("\n Status        : ");
            if (strcmp(complaints[i].status, "Resolved") == 0) set_win_color(10); // Green
            else set_win_color(12); // Red/Pending
            printf("%s", complaints[i].status);
            set_win_color(7);

            printf("\n Assigned Admin: %s", complaints[i].assignedAdmin);
            printf("\n Rating        : %d/5", complaints[i].rating);
            printf("\n Date Filed    : %s", complaints[i].date);
            printf("\n Description   : %s\n", complaints[i].description);
        }
    }

    if (found == 0) {
        set_win_color(12);
        printf("\n[!] No complaints filed yet by account: %s\n", logged_in_user);
        set_win_color(7);
    } else {
        set_win_color(11);
        printf("========================================================\n");
        set_win_color(7);
    }
}

// 2. viewAccusedComplaints: Displays complaints/notices filed AGAINST the user in formatted console output
void viewAccusedComplaints(const char *logged_in_user) {
    char file_name[100];
    sprintf(file_name, "notice_%s.txt", logged_in_user);
    FILE *fp = fopen(file_name, "r");

    if (fp == NULL) {
        set_win_color(10); // Green
        printf("\n========================================================");
        printf("\n [INFO] No official notices or targeted complaints against ID: %s", logged_in_user);
        printf("\n========================================================\n");
        set_win_color(7);
        return;
    }

    set_win_color(12); // Red border for warnings/accused complaints
    printf("\n========================================================");
    printf("\n           CONFIDENTIAL NOTICES / ACCUSATIONS          ");
    printf("\n========================================================\n");
    set_win_color(7);

    char line[256];
    int line_num = 1;
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf(" %d. %s", line_num++, line);
    }
    fclose(fp);

    set_win_color(12);
    printf("========================================================\n");
    set_win_color(7);
}

// Updated User Dashboard without color codes
void user_dashboard(const char *username) {
    // 1. Checks and displays unread alerts sent to this specific user ID
    check_user_notifications(username);

    // 2. Checks if file notices (notice_<username>.txt) exist for the user
    check_and_alert_accused(username);
    int choice;
    do {
        printf("\n===== USER DASHBOARD (%s) =====", username);
        printf("\n1. File a New Complaint");
        printf("\n2. View My Complaint Statuses");
        printf("\n3. Rate Resolved Complaints");
        printf("\n4. View Accused Complaints");
        printf("\n5. View My Punishments");
        printf("\n6. Apply For Grievance");
        printf("\n7. Logout");
        printf("\nEnter Choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
               FileComplaint(username);
                break;
            case 2:
                viewComplaints(username);
                break;

            case 3:
                rate_resolved_complaint(username);
                break;
            case 4:
               viewAccusedComplaints(username);
                break;
            case 5:
                 view_user_punishments(username);
                break;
            case 6:
                apply_for_grievance(username);
                break;
            case 7:
                printf("Logging out...\n");
                break;
            default:
                printf("\nInvalid choice!\n");
        }
    } while (choice != 7);
}
void user_login() {
    char input_id[ID_LEN], input_pass[PASS_LEN];
    char file_id[ID_LEN], file_hash[HASH_LEN];
    char input_hash[HASH_LEN];
    int first_login_flag = 0;

    FILE *fp = fopen(DB_FILE, "r");
    if (!fp) {
        printf("\nNo registered users found. Please register first.\n");
        return;
    }

    printf("\nEnter Registration ID: ");
    scanf("%49s", input_id);
    printf("Enter Password: ");
    scanf("%49s", input_pass);

    get_user_md5(input_pass, input_hash);

    int login_success = 0;
    while (fscanf(fp, "%s %s %d", file_id, file_hash, &first_login_flag) == 3) {
        if (strcmp(input_id, file_id) == 0 && strcmp(input_hash, file_hash) == 0) {
            login_success = 1;
            break;
        }
    }
    fclose(fp);

    if (!login_success) {
        printf("\nInvalid Registration ID or password. Returning to menu...\n");
        return;
    }

    printf("\nCredentials verified. Welcome, %s!\n", input_id);

    /* FIX: Force password reset if flag is 1 */
    if (first_login_flag == 1) {
        force_password_reset(input_id);
    }

    /* FIX: Launch dashboard after login */
    user_dashboard(input_id);
}

void User()
{
    int choice;

    do {
        printf("\n================ USER PORTAL ================\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear input buffer on non-numeric input
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice)
        {
            case 1:
                user_register(); // Goes to register, then comes back here
                break;
            case 2:
                user_login();
                break;
            case 3:
                printf("Returning to Main Menu...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 3);
}


//Admin
void adminregister()
{
    char key[20];
    printf("Enter the setup key to register as admin: ");
    scanf("%s", key);
    if (strcmp(key, setup_key) != 0)
    {
        printf("Invalid setup key.\n");
        return;
    }
    else
    {
        printf("\n Create a Username: ");
        char file_username[50];
        scanf ("%s" , file_username);

        printf("\n Create a password: ");
        char file_password[50];
        scanf("%s", file_password);

        printf("Confirm your password: ");
        char confirm_pass[50];
        scanf("%s", confirm_pass);

        if (strcmp(file_password, confirm_pass) != 0)
        {
            printf ("\n Password didn't match. Please try again.\n");
            return;
        }
         else
         {

            FILE *file = fopen("admin.txt", "a");

            fprintf(file, "%s\n%s\n", file_username, file_password);
            fclose(file);
            printf("Admin registered successfully.\n");
         }



    }
}



void printComplaint(Complaint c)
{
    printf("\n==============================\n");
    printf("Complaint ID : %d\n", c.id);
    printf("Category     : %s\n", c.category);
    printf("Description  : %s\n", c.description);
    printf("Priority     : %s\n", c.priority);
    printf("Status       : %s\n", c.status);
    printf("Date         : %s\n", c.date);
    printf("Assigned Team: %s\n", c.assignedTeam);
    printf("==============================\n");
}




void viewAllComplaints() {
    Complaint complaints[MAX];
    int count = 0;

    int success = load_all_complaints(complaints, &count);

    if (!success || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printComplaint(complaints[i]);
    }

    printf("\nTotal Complaints: %d\n", count);
}

void viewComplaintsByAssignedTeam(const char *team_name) {
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < count; i++) {
#ifdef _WIN32
        if (_stricmp(complaints[i].assignedTeam, team_name) == 0) {
#else
        if (strcasecmp(complaints[i].assignedTeam, team_name) == 0) {
#endif
            printComplaint(complaints[i]);
            found++;
        }
    }

    if (!found) {
        printf("\nNo complaints assigned to %s.\n", team_name);
    } else {
        printf("\nTotal complaints assigned to %s: %d\n", team_name, found);
    }
}
void viewComplaintByID() {
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    int id, found = 0;
    printf("Enter Complaint ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (complaints[i].id == id) {
            printComplaint(complaints[i]);
            found = 1;
            break;
        }
    }

    if (!found) printf("\nComplaint not found.\n");
}

void viewComplaintsByStatus() {
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    char status[20];
    printf("Enter Status (Open/In Progress/Resolved/Closed): ");
    getchar();
    fgets(status, sizeof(status), stdin);
    status[strcspn(status, "\n")] = '\0';

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcasecmp(complaints[i].status, status) == 0) {
            printComplaint(complaints[i]);
            found = 1;
        }
    }

    if (!found) printf("\nNo complaints found with status '%s'.\n", status);
}

void viewComplaintsByDate() {
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    Complaint temp;
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(complaints[j].date, complaints[j + 1].date) > 0) {
                temp = complaints[j];
                complaints[j] = complaints[j + 1];
                complaints[j + 1] = temp;
            }
        }
    }

    printf("\n===== Complaints Sorted by Date =====\n");
    for (int i = 0; i < count; i++) {
        printComplaint(complaints[i]);
    }
}
#include <ctype.h>

// Cross-platform case-insensitive priority lookup
int getPriorityValue(const char *priority) {
    if (!priority) return 0;

    // Use case-insensitive comparison (handles "High", "high", "HIGH")
#ifdef _WIN32
    if (_stricmp(priority, "High") == 0) return 3;
    if (_stricmp(priority, "Medium") == 0) return 2;
    if (_stricmp(priority, "Low") == 0) return 1;
#else
    if (strcasecmp(priority, "High") == 0) return 3;
    if (strcasecmp(priority, "Medium") == 0) return 2;
    if (strcasecmp(priority, "Low") == 0) return 1;
#endif

    return 0; // Default fallback for unassigned/unknown priorities
}

void viewComplaintsByPriority() {
    Complaint complaints[MAX];
    int count = 0;

    // Fix: Pass &count to populate total complaints, do NOT overwrite count with return status
    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    // Bubble sort complaints in descending order of priority value (3 -> 2 -> 1 -> 0)
    Complaint temp;
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (getPriorityValue(complaints[j].priority) < getPriorityValue(complaints[j + 1].priority)) {
                temp = complaints[j];
                complaints[j] = complaints[j + 1];
                complaints[j + 1] = temp;
            }
        }
    }

    printf("\n===== Complaints Sorted by Priority =====\n");
    for (int i = 0; i < count; i++) {
        printComplaint(complaints[i]);
    }
    printf("Total Complaints: %d\n", count);
}

void viewDepartmentComplaints(const char *categoryFilter) {
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(categoryFilter, "ALL") == 0 || strcasecmp(complaints[i].category, categoryFilter) == 0) {
            printComplaint(complaints[i]);
            found++;
        }
    }

    if (found == 0) {
        printf("\nNo complaints found for category: %s\n", categoryFilter);
    } else {
        printf("\nTotal Complaints Found: %d\n", found);
    }
}




void viewComplaintsMenu()
{
    int choice;

    do
    {
        printf("\n========== View Complaints ==========\n");
        printf("1. View All Complaints\n");
        printf("2. Search Complaint by ID\n");
        printf("3. View Complaints by Status\n");
        printf("4. Sort by Date\n");
        printf("5. Sort by Priority\n");
        printf("0. Back\n");

        printf("\nEnter your choice: ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                viewAllComplaints();
                break;

            case 2:
                viewComplaintByID();
                break;

            case 3:
                viewComplaintsByStatus();
                break;

            case 4:
                viewComplaintsByDate();
                break;

            case 5:
                viewComplaintsByPriority();
                break;

            case 0:
                printf("Returning...\n");
                break;

            default:
                printf("Invalid Choice!\n");
        }

    }while(choice != 0);
}


void notify_team(const char *teamName, int complaint_id, const char *status) {
#ifdef _WIN32
    char message[200];
    sprintf(message, "Complaint #%d assigned to %s Team.\nStatus: %s",
            complaint_id, teamName, status);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBox(NULL, message, "Team Notification", MB_OK | MB_ICONINFORMATION);
#else
    printf("\n[NOTIFICATION] Complaint #%d assigned to %s Team. Status: %s\n",
           complaint_id, teamName, status);
#endif
}
void assign_Team(Complaint complaints[], int count, Complaint *c) {
    printf("\n========== REASSIGN CATEGORY & TEAM ==========\n");
    printf("Complaint ID     : %d\n", c->id);
    printf("Current Category : %s\n", c->category);
    printf("Current Team     : %s\n", c->assignedTeam);
    printf("----------------------------------------------\n");

    int choice;
    printf("Select New Category & Responsible Team:\n");
    printf("1. Technical           -> Assign to: IT Support\n");
    printf("2. Accounts            -> Assign to: Accounts Admin\n");
    printf("3. Maintenance         -> Assign to: Maintenance Team\n");
    printf("4. Library             -> Assign to: Library Admin\n");
    printf("5. Exam                -> Assign to: Exam Head\n");
    printf("6. Security / Proctor  -> Assign to: Proctor Office\n");
    printf("Enter choice: ");

    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        printf("\n[!] Invalid input choice.\n");
        return;
    }

    switch (choice) {
        case 1:
            strcpy(c->category, "Technical");
            strcpy(c->assignedTeam, "IT Support");
            break;
        case 2:
            strcpy(c->category, "Accounts");
            strcpy(c->assignedTeam, "Accounts Admin");
            break;
        case 3:
            strcpy(c->category, "Maintenance");
            strcpy(c->assignedTeam, "Maintenance Team");
            break;
        case 4:
            strcpy(c->category, "Library");
            strcpy(c->assignedTeam, "Library Admin");
            break;
        case 5:
            strcpy(c->category, "Exam");
            strcpy(c->assignedTeam, "Exam Head");
            break;
        case 6:
            strcpy(c->category, "Security");
            strcpy(c->assignedTeam, "Proctor");
            break;
        default:
            printf("\n[!] Invalid selection.\n");
            return;
    }

    // Automatically transition status from "Open" to "In Progress" upon assignment
    if (strcasecmp(c->status, "Open") == 0) {
        strcpy(c->status, "In Progress");
    }

    // Save updated struct array back to file
    save_complaints(complaints, count);

    printf("\n[SUCCESS] Complaint #%d reclassified and reassigned!\n", c->id);
    printf("New Category: %s | New Team: %s | Status: %s\n",
           c->category, c->assignedTeam, c->status);

   notify_assigned_team(c->assignedTeam, c->id, c->status);
}

void updateStatus(const char *team_name)
{
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0)
    {
        printf("\n[!] No complaints found.\n");
        return;
    }

    printf("\n================ %s - UPDATE STATUS ================\n",
           team_name);

    int team_count = 0;

    /* Show complaints assigned to this team */
    for (int i = 0; i < count; i++)
    {
        if (strcmp(complaints[i].assignedTeam, team_name) == 0)
        {
            printf("\nComplaint ID : %d", complaints[i].id);
            printf("\nCategory     : %s", complaints[i].category);
            printf("\nFiled By     : %s", complaints[i].user);
            printf("\nStatus       : %s", complaints[i].status);
            printf("\n---------------------------------------------\n");

            team_count++;
        }
    }

    if (team_count == 0)
    {
        printf("\n[!] No complaints are assigned to %s.\n", team_name);
        return;
    }

    int id;

    printf("\nEnter Complaint ID to update: ");

    if (scanf("%d", &id) != 1)
    {
        while (getchar() != '\n');
        printf("[!] Invalid Complaint ID.\n");
        return;
    }

    /* Find the complaint belonging to this team */
    int found = -1;

    for (int i = 0; i < count; i++)
    {
        if (complaints[i].id == id &&
            strcmp(complaints[i].assignedTeam, team_name) == 0)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("\n[!] Complaint not found or this complaint is not assigned to %s.\n",
               team_name);
        return;
    }

    printf("\nCurrent Status: %s\n", complaints[found].status);

    printf("\n========== UPDATE STATUS ==========\n");
    printf("1. In Progress\n");
    printf("2. Resolved\n");
    printf("3. Closed\n");
    printf("Enter choice: ");

    int choice;

    if (scanf("%d", &choice) != 1)
    {
        while (getchar() != '\n');
        printf("[!] Invalid choice.\n");
        return;
    }

    switch (choice)
    {
        case 1:
            strcpy(complaints[found].status, "In Progress");
            break;

        case 2:
            strcpy(complaints[found].status, "Resolved");
            break;

        case 3:
            strcpy(complaints[found].status, "Closed");
            break;

        default:
            printf("\n[!] Invalid choice. Status not changed.\n");
            return;
    }

    /* Save updated status */
    if (!save_complaints(complaints, count))
    {
        printf("\n[!] Failed to save complaint status.\n");
        return;
    }

    printf("\n[SUCCESS] Complaint #%d status updated to '%s'.\n",
           complaints[found].id,
           complaints[found].status);

    /* Notify the user */
    char msg[256];

    snprintf(msg,
             sizeof(msg),
             "Your complaint #%d status has been updated to: %s",
             complaints[found].id,
             complaints[found].status);

    send_notification(
        complaints[found].user,
        complaints[found].id,
        msg
    );
}
void deleteComplaint() {
    Complaint complaints[MAX];
    int count = 0;

    // Load existing complaints from file
    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found to delete.\n");
        return;
    }

    int id;
    printf("\nEnter Complaint ID to delete: ");
    if (scanf("%d", &id) != 1) {
        while (getchar() != '\n'); // Clear invalid input buffer
        printf("\n[!] Invalid input.\n");
        return;
    }

    int found_index = -1;
    for (int i = 0; i < count; i++) {
        if (complaints[i].id == id) {
            found_index = i;
            break;
        }
    }

    if (found_index == -1) {
        printf("\n[!] Complaint ID #%d not found.\n", id);
        return;
    }

    // Display summary and ask for confirmation
    printf("\n========== COMPLAINT TO DELETE ==========\n");
    printf("ID          : %d\n", complaints[found_index].id);
    printf("User        : %s\n", complaints[found_index].user);
    printf("Category    : %s\n", complaints[found_index].category);
    printf("Description : %s\n", complaints[found_index].description);
    printf("Status      : %s\n", complaints[found_index].status);
    printf("=========================================\n");

    char confirm;
    printf("Are you sure you want to permanently delete Complaint #%d? (y/n): ", id);
    scanf(" %c", &confirm);

    if (confirm != 'y' && confirm != 'Y') {
        printf("\n[!] Deletion cancelled.\n");
        return;
    }

    // Shift array elements left to overwrite deleted item
    for (int i = found_index; i < count - 1; i++) {
        complaints[i] = complaints[i + 1];
    }
    count--;

    // Overwrite complaints.csv with updated list
    save_complaints(complaints, count);

    printf("\n[SUCCESS] Complaint #%d deleted successfully.\n", id);
}


#define MAX 100

void claim_complaint(const char *admin_name) {
    Complaint complaints[MAX];
    int count = 0;

    // Load all complaints
    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    int id, found = 0;
    printf("\nEnter Complaint ID to claim: ");
    if (scanf("%d", &id) != 1) {
        while (getchar() != '\n'); // clear invalid input
        printf("[!] Invalid ID input.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (complaints[i].id == id) {
            found = 1;

            // Trim trailing newline
            complaints[i].assignedAdmin[strcspn(complaints[i].assignedAdmin, "\r\n")] = '\0';

            // Check if already claimed
#ifdef _WIN32
            int already_claimed = (strlen(complaints[i].assignedAdmin) > 0 &&
                                   _stricmp(complaints[i].assignedAdmin, "unassigned") != 0);
#else
            int already_claimed = (strlen(complaints[i].assignedAdmin) > 0 &&
                                   strcasecmp(complaints[i].assignedAdmin, "unassigned") != 0);
#endif

            if (already_claimed) {
                printf("\n[!] Complaint #%d is already claimed by %s.\n",
                       id, complaints[i].assignedAdmin);
                return;
            }

            // Assign to current admin
            strncpy(complaints[i].assignedAdmin, admin_name,
                    sizeof(complaints[i].assignedAdmin) - 1);
            complaints[i].assignedAdmin[sizeof(complaints[i].assignedAdmin) - 1] = '\0';

            // Auto-update status
            strcpy(complaints[i].status, "In Progress");

            // Save changes
            save_complaints(complaints, count);

            printf("\n[SUCCESS] Complaint #%d successfully claimed by %s.\n",
                   id, admin_name);
            printf("[INFO] Status automatically updated to: In Progress\n");

            // Notify user
            char msg[256];
            snprintf(msg, sizeof(msg),
                     "Your complaint #%d has been claimed by admin %s. Status: In Progress",
                     id, admin_name);
            send_notification(complaints[i].user, id, msg);

            break;
        }
    }

    if (!found) {
        printf("\nComplaint ID #%d not found.\n", id);
    }
}


void update_complaint_status_by_admin(const char *admin_name) {
    Complaint complaints[MAX];
    int count = 0;

    if (!load_all_complaints(complaints, &count) || count == 0) {
        printf("\nNo complaints found.\n");
        return;
    }

    int id, found = 0;
    printf("\nEnter Complaint ID to update: ");
    if (scanf("%d", &id) != 1) {
        while (getchar() != '\n');
        printf("[!] Invalid ID input.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (complaints[i].id == id) {
            found = 1;

            if (strlen(complaints[i].assignedAdmin) == 0) {
                printf("\n[!] Complaint #%d has not been claimed yet.\n", id);
                return;
            }

#ifdef _WIN32
            int is_authorized = (_stricmp(admin_name, "ALL") == 0) ||
                                (_stricmp(complaints[i].assignedAdmin, admin_name) == 0);
#else
            int is_authorized = (strcasecmp(admin_name, "ALL") == 0) ||
                                (strcasecmp(complaints[i].assignedAdmin, admin_name) == 0);
#endif

            if (!is_authorized) {
                printf("\n[!] Access Denied: Complaint #%d is assigned to %s, not you.\n",
                       id, complaints[i].assignedAdmin);
                return;
            }

            int status_choice;
            printf("\nSelect New Status for Complaint #%d:\n", id);
            printf("1. In Progress\n");
            printf("2. Resolved\n");
            printf("3. Closed\n");
            printf("Enter choice: ");
            if (scanf("%d", &status_choice) != 1) {
                while (getchar() != '\n');
                printf("[!] Invalid choice format.\n");
                return;
            }

            if (status_choice == 1) strcpy(complaints[i].status, "In Progress");
            else if (status_choice == 2) strcpy(complaints[i].status, "Resolved");
            else if (status_choice == 3) strcpy(complaints[i].status, "Closed");
            else {
                printf("\n[!] Invalid selection. Status not changed.\n");
                return;
            }

            save_complaints(complaints, count);
            printf("\n[SUCCESS] Complaint #%d updated to '%s'.\n",
                   id, complaints[i].status);
            break;
        }
    }

    if (!found) {
        printf("\nComplaint ID #%d not found.\n", id);
    }
}

// Dashboard for Super / Assigning Team Admin
void assigning_team_admin_dashboard(const char *admin_name) {

    int choice;
    do {
        printf("\n===== ASSIGNING TEAM ADMIN DASHBOARD (%s) =====", admin_name);
        printf("\n1. View Complaints");
        printf("\n2. Assign Complaint to Team");

        printf("\n3. Delete Complaint");
        printf("\n4. Logout");
        printf("\nEnter Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewComplaintsMenu();
                break;

            case 2: {
                Complaint complaints[MAX];
                int count = 0;

                if (!load_all_complaints(complaints, &count) || count == 0) {
                    printf("\nNo complaints found to assign.\n");
                    break;
                }

                int id, found = 0;
                printf("Enter Complaint ID to assign: ");
                scanf("%d", &id);

                for (int i = 0; i < count; i++) {
                    if (complaints[i].id == id) {
                        assign_Team(complaints, count, &complaints[i]);
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    printf("\nComplaint ID %d not found.\n", id);
                }
                break;
            }



            case 3:
                deleteComplaint();
                break;

            case 4:
                printf("Logging out of Assigning Team Admin dashboard...\n");
                break;

            default:
                printf("\nInvalid Choice!");
        }
    } while (choice != 4);
}

void departmental_admin_dashboard(const char *admin_name, const char *dept_name, const char *category_filter) {


    int choice;
    do {
        printf("\n===== %s PORTAL (%s) =====", dept_name, admin_name);
        printf("\n1. View Complaints");
        printf("\n2. Claim / Take Responsibility");
        printf("\n3. Update Complaint Status");
        printf("\n4. Logout");
        printf("\nEnter Choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: viewDepartmentComplaints(category_filter); break;
            case 2: claim_complaint(admin_name); break;
            case 3: update_complaint_status_by_admin(admin_name); break;
            case 4: printf("Exiting portal...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 4);
}
// Sub-Admin Login
int sub_admin_login(int role_choice, char *logged_user) {
    char username[50], password[50];
    char file_username[50], file_hash[HASH_LEN];
    char input_hash[HASH_LEN];
    int file_role;

    FILE *fp = fopen(ADMIN_FILE, "r");
    if (fp == NULL) {
        printf("\nNo admins registered yet.\n");
        return 0;
    }

    printf("\nEnter Admin Username: ");
    scanf("%49s", username);
    printf("Enter Admin Password: ");
    scanf("%49s", password);

    // Convert input password to MD5 Hash for comparison
    get_user_md5(password, input_hash);

    int logged_in = 0;
    while (fscanf(fp, "%s %s %d", file_username, file_hash, &file_role) == 3) {
        if (strcmp(username, file_username) == 0 &&
            strcmp(input_hash, file_hash) == 0 &&
            file_role == role_choice) {
            logged_in = 1;
            strcpy(logged_user, username);
            break;
        }
    }
    fclose(fp);

    if (logged_in) {
        printf("\n[SUCCESS] Login successful! Welcome, %s.\n", username);
        return 1;
    } else {
        printf("\n[!] Invalid username, password, or unauthorized role.\n");
        return 0;
    }
}

void sub_admin_register(int role_choice) {
    char username[50], password[50], confirm_pass[50];
    char hashed_password[HASH_LEN];

    printf("Create Username: ");
    scanf("%49s", username);

    printf("Create Password: ");
    scanf("%49s", password);

    printf("Confirm Password: ");
    scanf("%49s", confirm_pass);

    if (strcmp(password, confirm_pass) != 0) {
        printf("\nPassword mismatch. Registration failed.\n");
        return;
    }

    get_user_md5(password, hashed_password);

    FILE *file = fopen(ADMIN_FILE, "a");
    if (!file) {
        printf("Error opening admin database file!\n");
        return;
    }

    fprintf(file, "%s %s %d\n", username, hashed_password, role_choice);
    fclose(file);

    printf("[SUCCESS] Admin registered successfully with MD5 encryption!\n");

    // Now call login
    char logged_user[50];
    printf("Please log in now:\n");
    if (sub_admin_login(role_choice, logged_user)) {
        printf("Logged in as: %s\n", logged_user);

        if (role_choice == 1) {
            assigning_team_admin_dashboard(logged_user);
        } else {
            const char *role_title;
            const char *category_filter;

            switch (role_choice) {
                case 2:
                    role_title = "Exam Office Admin";
                    category_filter = "Exam";
                    break;
                case 3:
                    role_title = "Library Admin";
                    category_filter = "Library";
                    break;
                case 4:
                    role_title = "Accounts Office Admin";
                    category_filter = "Accounts";
                    break;
                default:
                    role_title = "Admin";
                    category_filter = "ALL";
                    break;
            }

            departmental_admin_dashboard(logged_user, role_title, category_filter);
        }
    }
}


// Role Menu Handler
void handle_sub_admin_role(int role_choice, const char *role_title, const char *category_filter) {
    int choice;
    char logged_user[50];

    do {
        printf("\n--- %s Portal ---\n", role_title);
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Back\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                sub_admin_register(role_choice);
                break;
            case 2:
                if (sub_admin_login(role_choice, logged_user)) {
                    if (role_choice == 1) {
                        assigning_team_admin_dashboard(logged_user);
                    } else {
                        departmental_admin_dashboard(logged_user, role_title, category_filter);
                    }
                }
                break;
            case 3:
                printf("Returning to Admin menu...\n");
                break;
            default:
                printf("Invalid Choice!\n");
        }
    } while (choice != 3);
}



void AdminPortalMenu() {
    char key[50];

    printf("\n================ ADMIN PORTAL ================\n");
    printf("Enter Admin Setup Key: ");
    scanf("%49s", key);

    if (strcmp(key, General_Key) == 0) {
        handle_sub_admin_role(1, "Assigning Team Admin", "ALL");
    }
    else if (strcmp(key, ExamAdmin_Key) == 0) {
        handle_sub_admin_role(2, "Exam Office Admin", "Exam");
    }
    else if (strcmp(key, LibraryAdmin_key) == 0) {
        handle_sub_admin_role(3, "Library Admin", "Library");
    }
    else if (strcmp(key, Accounts_Key) == 0) {
        handle_sub_admin_role(4, "Accounts Office Admin", "Accounts");
    }
    else if (strcmp(key, setup_key) == 0) {
        /* Master key: let the admin pick which role portal to enter */
        int choice;
        do {
            printf("\n---- Master Key Accepted: Choose Admin Role ----\n");
            printf("1. General Admin\n");
            printf("2. Exam Office Administrator\n");
            printf("3. Library Administrator\n");
            printf("4. Accounts Office Administrator\n");
            printf("5. Back to Main Menu\n");
            printf("Enter choice: ");

            if (scanf("%d", &choice) != 1) {
                while (getchar() != '\n');
                continue;
            }

            switch (choice) {
                case 1: handle_sub_admin_role(1, "Assigning Team Admin", "ALL"); break;
                case 2: handle_sub_admin_role(2, "Exam Office Admin", "Exam"); break;
                case 3: handle_sub_admin_role(3, "Library Admin", "Library"); break;
                case 4: handle_sub_admin_role(4, "Accounts Office Admin", "Accounts"); break;
                case 5: printf("Returning to Main Menu...\n"); break;
                default: printf("Invalid choice. Please try again.\n");
            }
        } while (choice != 5);
    }
    else {
        printf("\n[!] Invalid setup key. Access denied.\n");
    }
}


/* =========================================================
                  RESOLUTION TEAM SYSTEM
   ========================================================= */

/* ---------- Register Resolution Team ---------- */

void teamRegister()
{
    char team[50];
    char username[50];
    char password[50];
    char confirm[50];
    char hash[HASH_LEN];

    int choice;

    printf("\n========== RESOLUTION TEAM REGISTRATION ==========\n");

    printf("1. IT Team\n");
    printf("2. Maintenance Team\n");
    printf("3. Proctor Office\n");
    printf("4. Grievance Committee\n");
    printf("5. Student Discipline Committee\n");
    printf("6. Special Committee (Sexual Harassment)\n");

    printf("Enter team choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            strcpy(team, "IT");
            break;

        case 2:
            strcpy(team, "Maintenance");
            break;

        case 3:
            strcpy(team, "Proctor");
            break;

        case 4:
            strcpy(team, "Grievance");
            break;

        case 5:
            strcpy(team, "StudentDiscipline");
            break;

        case 6:
            strcpy(team, "SpecialCommittee");
            break;

        default:
            printf("Invalid choice!\n");
            return;
    }

    printf("Create Username: ");
    scanf("%49s", username);

    printf("Create Password: ");
    scanf("%49s", password);

    printf("Confirm Password: ");
    scanf("%49s", confirm);

    if(strcmp(password, confirm) != 0)
    {
        printf("\nPassword did not match!\n");
        return;
    }

    /* Use the MD5 function you already have */
    get_user_md5(password, hash);

    FILE *fp = fopen("resolution_team.txt", "a");

    if(fp == NULL)
    {
        printf("\nError opening resolution_team.txt\n");
        return;
    }

    /*
       Stored format:

       Team Username MD5_Hash
    */

    fprintf(fp, "%s %s %s\n", team, username, hash);

    fclose(fp);

    printf("\nResolution team registered successfully!\n");
    printf("Password has been stored using MD5.\n");
}


/* ---------- Resolution Team Login ---------- */

void resolutionLogin()
{
    char username[50];
    char password[50];

    char fileTeam[50];
    char fileUsername[50];
    char fileHash[HASH_LEN];

    char inputHash[HASH_LEN];

    int found = 0;

    printf("\n========== RESOLUTION TEAM LOGIN ==========\n");

    printf("Username: ");
    scanf("%49s", username);

    printf("Password: ");
    scanf("%49s", password);

    /* Convert entered password into MD5 */
    get_user_md5(password, inputHash);

    FILE *fp = fopen("resolution_team.txt", "r");

    if(fp == NULL)
    {
        printf("\nNo resolution team account found.\n");
        printf("Please register a team first.\n");
        return;
    }

    char line[256];

    while (fgets(line, sizeof(line), fp))
    {
        char team[50];
        char savedUsername[50];
        char savedHash[HASH_LEN];

        /* Normal format: Team Username Hash */
        if (sscanf(line, "%49s %49s %64s",
                   team, savedUsername, savedHash) == 3)
        {
            if (strcmp(username, savedUsername) == 0 &&
                strcmp(inputHash, savedHash) == 0)
            {
                strcpy(fileTeam, team);
                strcpy(fileUsername, savedUsername);
                strcpy(fileHash, savedHash);
                found = 1;
                break;
            }
        }

        /*
           Backward compatibility for an older account stored as:
           Special Committee Username Hash
        */
        char oldUser[50];
        char oldHash[HASH_LEN];

        if (sscanf(line, "Special Committee %49s %64s",
                   oldUser, oldHash) == 2)
        {
            if (strcmp(username, oldUser) == 0 &&
                strcmp(inputHash, oldHash) == 0)
            {
                strcpy(fileTeam, "SpecialCommittee");
                strcpy(fileUsername, oldUser);
                strcpy(fileHash, oldHash);
                found = 1;
                break;
            }
        }
    }

    fclose(fp);

    if(!found)
    {
        printf("\nInvalid username or password.\n");
        return;
    }

    printf("\nLogin successful!\n");
    printf("Welcome to %s.\n", fileTeam);

    if(strcmp(fileTeam, "IT") == 0)
    {
        ITMenu(fileUsername);
    }
    else if(strcmp(fileTeam, "Maintenance") == 0)
    {
        MaintenanceMenu(fileUsername);
    }
    else if(strcmp(fileTeam, "Proctor") == 0)
    {
        ProctorMenu();
    }
    else if(strcmp(fileTeam, "Grievance") == 0)
    {
        GrievanceMenu();
    }
    else if(strcmp(fileTeam, "StudentDiscipline") == 0)
    {
        StudentDisciplineMenu();
    }
    else if(strcmp(fileTeam, "SpecialCommittee") == 0)
    {
        special_committee_dashboard();
    }
}


/* =========================================================
                         IT TEAM
   ========================================================= */

void ITMenu(const char *admin_name)
{
    int choice;

    do
    {
        printf("\n========== IT SUPPORT TEAM ==========\n");
        printf("1. View Complaints\n");
        printf("2. Update Status\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                viewComplaintsByAssignedTeam("IT Support");
                break;

            case 2:
                updateStatus("IT Support");
                break;

            case 3:
                printf("Logging out from IT Team...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 3);
}


/* =========================================================
                     MAINTENANCE TEAM
   ========================================================= */

void MaintenanceMenu(const char *admin_name)
{
    int choice;

    do
    {
        printf("\n========== MAINTENANCE TEAM ==========\n");
        printf("1. View Complaints\n");
        printf("2. Update Status\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                viewComplaintsByAssignedTeam("Maintenance Team");
                break;

            case 2:
                updateStatus("Maintenance Team");
                break;

            case 3:
                printf("Logging out from Maintenance Team...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 3);
}


/* =========================================================
                       PROCTOR OFFICE
   ========================================================= */

void ProctorMenu()
{
    int choice;

    do
    {
        printf("\n========== PROCTOR OFFICE ==========\n");
        printf("1. View Complaints\n");
        printf("2. Give Suggestion\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                viewComplaintsByAssignedTeam("Proctor");

                break;

            case 2:
                giveProctorSuggestion();
                break;

            case 3:
                printf("Logging out from Proctor Office...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 3);
}


/* ---------- Proctor Gives Suggestion ---------- */

void giveProctorSuggestion()
{
    int complaintID;
    int choice;

    const char *suggestion;

    printf("\n========== GIVE PROCTOR SUGGESTION ==========\n");

    printf("Enter Complaint ID: ");
    scanf("%d", &complaintID);

    printf("\nSelect Suggestion:\n");
    printf("1. Fine\n");
    printf("2. Cancel Waiver\n");
    printf("3. Rusticate\n");
    printf("4. Call Parents\n");
    printf("5. Investigation\n");
    printf("6. Send Matter to Head Teacher\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            suggestion = "Fine";
            break;

        case 2:
            suggestion = "Cancel Waiver";
            break;

        case 3:
            suggestion = "Rusticate";
            break;

        case 4:
            suggestion = "Call Parents";
            break;

        case 5:
            suggestion = "Investigation";
            break;

        case 6:
            suggestion = "Send Matter to Head Teacher";
            break;

        default:
            printf("Invalid choice!\n");
            return;
    }

    FILE *fp = fopen("suggestion.txt", "a");

    if(fp == NULL)
    {
        printf("\nError opening suggestion.txt\n");
        return;
    }

    fprintf(fp,
            "Complaint ID: %d | Proctor Suggestion: %s\n",
            complaintID,
            suggestion);

    fclose(fp);

    printf("\nSuggestion saved successfully!\n");
    printf("The suggestion will be reviewed by the Student Discipline Committee.\n");
}


/* =========================================================
                   GRIEVANCE COMMITTEE
   ========================================================= */

void GrievanceMenu()
{
    int choice;

    do
    {
        printf("\n========== GRIEVANCE COMMITTEE ==========\n");
        printf("1. View Grievances\n");
        printf("2. Recall\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                viewGrievances();
                break;

            case 2:
                grievanceRecall();
                break;

            case 3:
                printf("Logging out from Grievance Committee...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 3);
}


/* ---------- View Grievances Directly From complaints.csv ---------- */

void viewGrievances()
{
    Complaint complaints[MAX];
    int count = 0;
    int found = 0;

    if(!load_all_complaints(complaints, &count) || count == 0)
    {
        printf("\nNo complaints found.\n");
        return;
    }

    printf("\n========== GRIEVANCES ==========\n");

    for(int i = 0; i < count; i++)
    {
        if(strcmp(complaints[i].category, "Grievance") == 0)
        {
            printComplaint(complaints[i]);
            found = 1;
        }
    }

    if(!found)
    {
        printf("\nNo grievances found.\n");
    }
}


/* ---------- Grievance Recall ---------- */

void grievanceRecall()
{
    int complaintID;
    int choice;

    const char *action;

    printf("\n========== GRIEVANCE RECALL ==========\n");

    printf("Enter Complaint ID: ");
    scanf("%d", &complaintID);

    printf("\nSelect Action:\n");
    printf("1. Fine\n");
    printf("2. Remove Waiver\n");
    printf("3. Rusticate\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            action = "Fine";
            break;

        case 2:
            action = "Remove Waiver";
            break;

        case 3:
            action = "Rusticate";
            break;

        default:
            printf("Invalid choice!\n");
            return;
    }

    FILE *fp = fopen("suggestion.txt", "a");

    if(fp == NULL)
    {
        printf("\nError opening suggestion.txt\n");
        return;
    }

    fprintf(fp,
            "Grievance Complaint ID: %d | Committee Action: %s\n",
            complaintID,
            action);

    fclose(fp);

    printf("\nAction recorded successfully!\n");
}


/* =========================================================
               STUDENT DISCIPLINE COMMITTEE
   ========================================================= */

void StudentDisciplineMenu()
{
    int choice;

    do
    {
        printf("\n========== STUDENT DISCIPLINE COMMITTEE ==========\n");
        printf("1. View Proctor Suggestions\n");
        printf("2. Implement Punishment\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                viewSuggestions();
                break;

            case 2:
                implementPunishment();
                break;

            case 3:
                printf("Logging out from Student Discipline Committee...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 3);
}


/* ---------- View Proctor Suggestions ---------- */

void viewSuggestions()
{
    FILE *fp = fopen("suggestion.txt", "r");

    if(fp == NULL)
    {
        printf("\nNo suggestions found.\n");
        return;
    }

    Complaint complaints[MAX];
    int count = 0;

    if(!load_all_complaints(complaints, &count))
    {
        printf("\nUnable to load complaints.\n");
        fclose(fp);
        return;
    }

    char line[300];
    int complaintID;
    int found;

    printf("\n========== PROCTOR SUGGESTIONS ==========\n");

    while(fgets(line, sizeof(line), fp))
    {
        /*
           Check whether this is a Proctor suggestion
           or a grievance committee action.
        */

        if(sscanf(line,
                  "Complaint ID: %d",
                  &complaintID) == 1)
        {
            found = 0;

            for(int i = 0; i < count; i++)
            {
                if(complaints[i].id == complaintID)
                {
                    printComplaint(complaints[i]);
                    found = 1;
                    break;
                }
            }

            if(found)
            {
                printf("Suggestion/Action: %s", line);
                printf("--------------------------------\n");
            }
            else
            {
                printf("\nComplaint #%d not found.\n", complaintID);
                printf("Suggestion/Action: %s", line);
            }
        }
        else if(sscanf(line,
                       "Grievance Complaint ID: %d",
                       &complaintID) == 1)
        {
            found = 0;

            for(int i = 0; i < count; i++)
            {
                if(complaints[i].id == complaintID)
                {
                    printComplaint(complaints[i]);
                    found = 1;
                    break;
                }
            }

            if(found)
            {
                printf("Committee Action: %s", line);
                printf("--------------------------------\n");
            }
            else
            {
                printf("\nComplaint #%d not found.\n", complaintID);
                printf("Committee Action: %s", line);
            }
        }
        else
        {
            printf("%s", line);
        }
    }

    fclose(fp);
}


/* ---------- Student Discipline Implements Final Punishment ---------- */

void implementPunishment()
{
    int complaintID;
    int choice;

    const char *punishment;

    printf("\n========== IMPLEMENT FINAL PUNISHMENT ==========\n");

    printf("Enter Complaint ID: ");
    scanf("%d", &complaintID);

    printf("\nSelect Final Punishment:\n");
    printf("1. Fine\n");
    printf("2. Cancel Waiver\n");
    printf("3. Rusticate\n");
    printf("4. Call Parents\n");
    printf("5. Investigation\n");
    printf("6. Send Matter to Head Teacher\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            punishment = "Fine";
            break;

        case 2:
            punishment = "Cancel Waiver";
            break;

        case 3:
            punishment = "Rusticate";
            break;

        case 4:
            punishment = "Call Parents";
            break;

        case 5:
            punishment = "Investigation";
            break;

        case 6:
            punishment = "Send Matter to Head Teacher";
            break;

        default:
            printf("Invalid choice!\n");
            return;
    }

    FILE *fp = fopen("suggestion.txt", "a");

    if(fp == NULL)
    {
        printf("\nError opening suggestion.txt\n");
        return;
    }

    fprintf(fp,
            "Complaint ID: %d | FINAL PUNISHMENT: %s\n",
            complaintID,
            punishment);

    fclose(fp);

    printf("\nFinal punishment recorded successfully.\n");
}

/* =========================================================
          SPECIAL COMMITTEE - SEXUAL HARASSMENT
   ========================================================= */

void special_committee_view_complaints()
{
    Complaint complaints[MAX];
    int count = 0;
    int found = 0;

    if (!load_all_complaints(complaints, &count) || count == 0)
    {
        printf("\n[!] No complaints found.\n");
        return;
    }

    printf("\n========================================================\n");
    printf("       SEXUAL HARASSMENT COMPLAINTS\n");
    printf("========================================================\n");

    for (int i = 0; i < count; i++)
    {
        if (strcmp(complaints[i].category, "Sexual Harassment") == 0 &&
            strcmp(complaints[i].assignedTeam, "SpecialCommittee") == 0 &&
            strcmp(complaints[i].status, "Closed") != 0)
        {
            found = 1;

            printf("\n--------------------------------------------------------\n");
            printf("Complaint ID  : #%d\n", complaints[i].id);
            printf("Filed By      : %s\n", complaints[i].user);
            printf("Against User  : %s\n", complaints[i].targetUser);
            printf("Category      : %s\n", complaints[i].category);
            printf("Description   : %s\n", complaints[i].description);
            printf("Priority      : %s\n", complaints[i].priority);
            printf("Status        : %s\n", complaints[i].status);
            printf("Date          : %s\n", complaints[i].date);
            printf("Assigned Team : Special Committee\n");
            printf("--------------------------------------------------------\n");
        }
    }

    if (!found)
    {
        printf("\n[!] No pending Sexual Harassment complaints found.\n");
    }
}


void special_committee_decision()
{
    Complaint complaints[MAX];
    int count = 0;
    int complaintID;
    int found = -1;

    if (!load_all_complaints(complaints, &count) || count == 0)
    {
        printf("\n[!] No complaints found.\n");
        return;
    }

    printf("\n========================================================\n");
    printf("       REVIEW SEXUAL HARASSMENT COMPLAINT\n");
    printf("========================================================\n");

    printf("Enter Complaint ID: ");

    if (scanf("%d", &complaintID) != 1)
    {
        while (getchar() != '\n');
        printf("\n[!] Invalid Complaint ID.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        if (complaints[i].id == complaintID &&
            strcmp(complaints[i].category, "Sexual Harassment") == 0 &&
            strcmp(complaints[i].assignedTeam, "SpecialCommittee") == 0 &&
            strcmp(complaints[i].status, "Closed") != 0)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("\n[!] Pending Sexual Harassment Complaint #%d not found.\n",
               complaintID);
        return;
    }

    printf("\n================ COMPLAINT DETAILS ================\n");
    printf("Complaint ID : #%d\n", complaints[found].id);
    printf("Filed By     : %s\n", complaints[found].user);
    printf("Against User : %s\n", complaints[found].targetUser);
    printf("Category     : %s\n", complaints[found].category);
    printf("Description  : %s\n", complaints[found].description);
    printf("Priority     : %s\n", complaints[found].priority);
    printf("Status       : %s\n", complaints[found].status);
    printf("Date         : %s\n", complaints[found].date);
    printf("====================================================\n");

    char comment[256];

    while (getchar() != '\n');

    printf("\nEnter Special Committee Comment:\n");
    fgets(comment, sizeof(comment), stdin);
    comment[strcspn(comment, "\n")] = '\0';

    if (strlen(comment) == 0)
    {
        strcpy(comment, "Complaint reviewed by Special Committee.");
    }

    int punishmentChoice;

    printf("\n====================================================\n");
    printf("                 COMMITTEE DECISION\n");
    printf("====================================================\n");
    printf("1. Official Warning\n");
    printf("2. Fine\n");
    printf("3. Account Suspension\n");
    printf("4. Proctorial Inquiry\n");
    printf("5. Disciplinary Action\n");
    printf("6. No Punishment\n");
    printf("====================================================\n");
    printf("Enter Choice: ");

    if (scanf("%d", &punishmentChoice) != 1)
    {
        while (getchar() != '\n');
        printf("\n[!] Invalid choice.\n");
        return;
    }

    char punishment[100];

    switch (punishmentChoice)
    {
        case 1:
            strcpy(punishment, "Official Warning");
            break;
        case 2:
            strcpy(punishment, "Fine");
            break;
        case 3:
            strcpy(punishment, "Account Suspension");
            break;
        case 4:
            strcpy(punishment, "Proctorial Inquiry");
            break;
        case 5:
            strcpy(punishment, "Disciplinary Action");
            break;
        case 6:
            strcpy(punishment, "No Punishment");
            break;
        default:
            printf("\n[!] Invalid punishment choice.\n");
            return;
    }

    FILE *fp = fopen(PUNISHMENT_FILE, "a");

    if (fp == NULL)
    {
        printf("\n[!] Error opening punishments.txt\n");
        return;
    }

    time_t currentTime = time(NULL);
    struct tm *currentDate = localtime(&currentTime);

    char date[20];

    if (currentDate != NULL)
    {
        sprintf(date, "%04d-%02d-%02d",
                currentDate->tm_year + 1900,
                currentDate->tm_mon + 1,
                currentDate->tm_mday);
    }
    else
    {
        strcpy(date, "Unknown");
    }

    fprintf(fp, "%d|%s|%s|Committee Comment: %s|%s\n",
            complaints[found].id,
            complaints[found].targetUser,
            punishment,
            comment,
            date);

    fclose(fp);

    /*
       Final step of the workflow:
       After the Special Committee makes its decision,
       the complaint becomes Closed.
    */
    strcpy(complaints[found].status, "Closed");

    if (!save_complaints(complaints, count))
    {
        printf("\n[!] Punishment saved, but complaint status could not be updated.\n");
        return;
    }

    char notification[256];

    snprintf(notification,
             sizeof(notification),
             "Special Committee decision for Sexual Harassment Complaint #%d: %s",
             complaints[found].id,
             punishment);

    send_notification(complaints[found].targetUser,
                      complaints[found].id,
                      notification);

    printf("\n====================================================\n");
    printf("       SPECIAL COMMITTEE DECISION RECORDED\n");
    printf("====================================================\n");
    printf("Complaint ID : #%d\n", complaints[found].id);
    printf("Punishment   : %s\n", punishment);
    printf("Comment      : %s\n", comment);
    printf("Status       : Closed\n");
    printf("Saved To     : punishments.txt\n");
    printf("====================================================\n");
}


void special_committee_dashboard()
{
    int choice;

    do
    {
        printf("\n====================================================\n");
        printf("          SPECIAL COMMITTEE PORTAL\n");
        printf("====================================================\n");
        printf("1. View Sexual Harassment Complaints\n");
        printf("2. Review Complaint & Make Decision\n");
        printf("3. Logout\n");
        printf("====================================================\n");
        printf("Enter Choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("\n[!] Invalid input.\n");
            continue;
        }

        switch (choice)
        {
            case 1:
                special_committee_view_complaints();
                break;

            case 2:
                special_committee_decision();
                break;

            case 3:
                printf("\nLogging out from Special Committee...\n");
                break;

            default:
                printf("\n[!] Invalid choice.\n");
        }

    } while (choice != 3);
}

/* =========================================================
                  RESOLUTION TEAM MENU
   ========================================================= */

void ResolutionTeam()
{
    int choice;

    do
    {
        printf("\n========== RESOLUTION TEAM ==========\n");
        printf("1. Register Resolution Team\n");
        printf("2. Login\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                teamRegister();
                break;

            case 2:
                resolutionLogin();
                break;

            case 3:
                printf("Returning to Main Menu...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 3);
}


 int main() {
    int choice;
    do {
        printf("\n------ Complaint Management System ------\n");
        printf("1. User Portal\n");
        printf("2. Admin Portal\n");
        printf("3. Resolution Team\n");
        printf("4. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                User(); // User dashboard (File complaint, view status, rate resolution)
                break;
            case 2:
                AdminPortalMenu();
                break;
            case 3:
                ResolutionTeam();
                break;
            case 4:
                printf("Exiting application.\n");
                break;
            default:
                printf("Invalid selection.\n");
        }
    } while (choice != 4);

    return 0;
}
