#include <stdio.h>
#include "complaint.h"


 int main ()
 {
    int choice;
do {
    printf("------This is a Comlaint Management System------\n");

    printf("\n-----If you are a user and want to file a complain, please select option 1 to register or log in.-----\n");
    printf ("\n-----If you want to check the status of your complaint, please select option 2.-----\n");
    printf ("\n-----If you are an admin and want to manage complaints, please select option 3.-----\n");
    printf("1.User.\n");
    printf ("2. Track a complaint.\n");
    printf ("3.Admin.\n");
    printf ("4.Exit\n");
    printf ("Enter your choice: ");
  
  if (scanf("%d", &choice) != 1) 
{
    // make sure to clear the input buffer if the input is invalid
    while (getchar() != '\n');  
    { 
        printf("Invalid input. Please enter a number (1-4).\n");
    }
    choice = -1;
    continue;    // go back to menu
}
    switch (choice)
 {

    case 1:
     User();
     break;

    case 2:
     TrackComplaint();
     break;

    case 3:
     Admin();
     break;

    case 4:
     printf("Exiting the program.\n");
     break;

    default:
     printf("Invalid choice. Please try again.\n");
      
 }
    }
    while(choice != 4);
    return 0;
 }