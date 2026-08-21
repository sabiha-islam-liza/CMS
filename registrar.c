
#include "registrar.h"

// Define the setup key
const char *Registrar_Key = "REG123";

/* ---------- Helper Functions ---------- */
void read_input(char *buffer, int size, const char *prompt) {
    printf("%s", prompt);
    if (scanf("%49s", buffer) != 1) {
        while (getchar() != '\n'); // clear buffer
        buffer[0] = '\0';
    }
}

int get_menu_choice() {
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n'); // clear buffer
        return -1;
    }
    return choice;
}

FILE* safe_open(const char *filename, const char *mode) {
    FILE *fp = fopen(filename, mode);
    if (!fp) {
        printf("[!] Error opening file: %s\n", filename);
    }
    return fp;
}

int confirm_password(const char *pass, const char *confirm) {
    return strcmp(pass, confirm) == 0;
}

void hash_password(const char *password, char *output) {
    get_user_md5(password, output);
}

/* ---------- Registrar Registration ---------- */
void registrar_register() {
    char key[50], username[50], password[50], confirm_pass[50];
    char hashed_password[HASH_LEN];

    printf("\n========== REGISTRAR REGISTRATION ==========\n");
    read_input(key, sizeof(key), "Enter Registrar Setup Key: ");

    if (strcmp(key, Registrar_Key) != 0) {
        printf("\n[!] Invalid Registrar setup key.\n");
        return;
    }

    read_input(username, sizeof(username), "Create Username: ");
    read_input(password, sizeof(password), "Create Password: ");
    read_input(confirm_pass, sizeof(confirm_pass), "Confirm Password: ");

    if (!confirm_password(password, confirm_pass)) {
        printf("\n[!] Passwords do not match. Registration failed.\n");
        return;
    }

    hash_password(password, hashed_password);

    FILE *fp = safe_open("registrar.txt", "a");
    if (!fp) return;

    fprintf(fp, "%s %s\n", username, hashed_password);
    fclose(fp);

    printf("\n[SUCCESS] Registrar registered successfully!\n");
    printf("Password has been stored using MD5.\n");
}

/* ---------- Registrar Login ---------- */
int registrar_login(char *logged_user) {
    char username[50], password[50];
    char file_username[50], file_hash[HASH_LEN], input_hash[HASH_LEN];
    int logged_in = 0;

    FILE *fp = safe_open("registrar.txt", "r");
    if (!fp) {
        printf("\n[!] No Registrar account found.\n");
        printf("Please register a Registrar first.\n");
        return 0;
    }

    printf("\n========== REGISTRAR LOGIN ==========\n");
    read_input(username, sizeof(username), "Username: ");
    read_input(password, sizeof(password), "Password: ");

    hash_password(password, input_hash);

    while (fscanf(fp, "%49s %64s", file_username, file_hash) == 2) {
        if (strcmp(username, file_username) == 0 &&
            strcmp(input_hash, file_hash) == 0) {
            logged_in = 1;
            strcpy(logged_user, username);
            break;
        }
    }
    fclose(fp);

    if (logged_in) {
        printf("\n[SUCCESS] Registrar login successful!\n");
        printf("Welcome, %s.\n", username);
        return 1;
    }

    printf("\n[!] Invalid username or password.\n");
    return 0;
}

/* ---------- View Admin Ratings ---------- */
void registrar_view_ratings() {
    FILE *fp = safe_open(RATINGS_FILE, "r");
    if (!fp) return;

    char admin_name[50], category[50];
    int total_sum, count, found = 0;
    float avg_rating;

    printf("\n=====================================================================\n");
    printf("                     ADMIN RATINGS (REGISTRAR ONLY)\n");
    printf("=====================================================================\n");
    printf("%-20s | %-20s | %-15s | %-10s\n",
           "Admin Name", "Category/Role", "Total Rating", "Average");
    printf("---------------------------------------------------------------------\n");

    while (fscanf(fp, "%49[^|]|%49[^|]|%d|%d|%f\n",
                  admin_name, category, &total_sum, &count, &avg_rating) == 5) {
        printf("%-20s | %-20s | %d (%d votes) | %.2f / 5.0\n",
               admin_name, category, total_sum, count, avg_rating);
        found++;
    }

    if (!found) {
        printf("No admin ratings registered in the database.\n");
    }
    printf("=====================================================================\n");
    fclose(fp);
}

/* ---------- Registrar Dashboard ---------- */
void registrar_dashboard(const char *username) {
    int choice;
    do {
        printf("\n===== REGISTRAR DASHBOARD (%s) =====\n", username);
        printf("1. View All Admin Ratings\n");
        printf("2. Logout\n");
        printf("Enter Choice: ");

        choice = get_menu_choice();
        if (choice == -1) {
            printf("[!] Invalid input.\n");
            continue;
        }

        switch (choice) {
            case 1: registrar_view_ratings(); break;
            case 2: printf("\nLogging out of Registrar Portal...\n"); break;
            default: printf("\nInvalid choice.\n");
        }
    } while (choice != 2);
}

/* ---------- Registrar Portal ---------- */
void Registrar() {
    int choice;
    char logged_user[50];

    do {
        printf("\n========== REGISTRAR PORTAL ==========\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Back\n");
        printf("Enter Choice: ");

        choice = get_menu_choice();
        if (choice == -1) {
            printf("[!] Invalid input.\n");
            continue;
        }

        switch (choice) {
            case 1: registrar_register(); break;
            case 2: if (registrar_login(logged_user)) registrar_dashboard(logged_user); break;
            case 3: printf("\nReturning to Main Menu...\n"); break;
            default: printf("\nInvalid choice.\n");
        }
    } while (choice != 3);
}
