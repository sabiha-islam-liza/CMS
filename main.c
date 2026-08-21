#include <stdio.h>
#include "user.h"
#include "admin.h"
#include "resolution.h"
#include "registrar.h"

int main() {
    int choice;
    do {
        printf("\n------ Complaint Management System ------\n");
        printf("1. User Portal\n");
        printf("2. Admin Portal\n");
        printf("3. Resolution Team\n");
        printf("4. Registrar\n");
        printf("5. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // clear invalid input
            continue;
        }

        switch (choice) {
            case 1:
                User();              // User dashboard
                break;
            case 2:
                AdminPortalMenu();   // Admin menu
                break;
            case 3:
                ResolutionTeam();    // Resolution team menu
                break;
            case 4:
                Registrar();         // Registrar menu
                break;
            case 5:
                printf("Exiting application.\n");
                break;
            default:
                printf("Invalid selection.\n");
        }
    } while (choice != 5);

    return 0;
}

