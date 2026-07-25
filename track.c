// track.c

#include <stdio.h>
#include "complaint.h"

void TrackComplaint() {
    FILE *file;
    int id, searchId;
    char name[50];
    char description[200];
    char status[20];
    int found = 0;

    file = fopen("complaints.txt", "r");

    if (file == NULL) {
        printf("No complaint file found!\n");
        return;
    }

    printf("Enter Complaint ID to track: ");
    scanf("%d", &searchId);

    while (fscanf(file, "%d %s %s %s", &id, name, description, status) != EOF) {
        if (id == searchId) {
            printf("\nComplaint found!\n");
            printf("ID: %d\n", id);
            printf("Name: %s\n", name);
            printf("Description: %s\n", description);
            printf("Status: %s\n", status);
            found = 1;
            break;
        }
    }

    if (found == 0) {
        printf("\nNo complaint found with this ID.\n");
    }

    fclose(file);
}
