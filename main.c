#include <stdio.h>
#include "complaint.h"
 int main ()
 {
    int choice;
do {
    printf("------This is a Comlaint Management System------\n");
    
    printf("1.File a complaint.\n");
    printf ("2. Track a complaint.\n");
    printf ("3.Admin authentication.\n");
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
     FileComplaint();
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