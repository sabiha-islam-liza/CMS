#include <stdio.h>
#include "complaint.h"
 int main ()
 {
    int choice;

    printf("------This is a Comlaint Management System------\n");
    
    printf("1.File a complaint.\n");
    printf ("2. Track a complaint.\n");
    printf ("3.Admin authentication.\n");
    printf ("4.Exit\n");
 
    switch (choice)
 {

    case 1:
     FileComplaint();
     break;

    case 2:
     TrackComplaint();
     break;

    case 3:
      AdminAuthentication();
      break;

    case 4:
      printf("Exiting the program.\n");
      break;

    case 5:
      printf("Invalid choice. Please try again.\n");
      
 }
    return 0;
 }