#include <stdio.h>
#include "complaint.h"

void user_register()
{
    char file_username[50], file_password[50];
    printf("\nCreate a Username: ");
    scanf("%s", file_username);
    printf("Create a Password: ");
    scanf("%s", file_password);
    if(fscanf(fopen("user.txt", "r"), "%s", file_username) == 1)
    {
        printf("Username already exists. Please choose a different username.\n");
        return;
    }
printf("Confirm your password: ");
    char confirm_pass[50];
    scanf("%s", confirm_pass);
    if (strcmp(file_password, confirm_pass) != 0)
    {
        printf("\nPassword didn't match. Please try again.\n");
        return;
    }
    else
    {
        FILE *file = fopen("user.txt", "a");
        fprintf(file, "%s\n%s\n", file_username, file_password);
        fclose(file);
        printf("User registered successfully.\n");
    }
    printf("\nRegistration successful. You can now log in with your credentials.\n");
}
void user_login()
{
    char username[50], password[50];
    char file_username[50], file_password[50];
    FILE *fp = fopen("user.txt", "r");
    
    printf("\nEnter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);
    while (fscanf(fp, "%s\n%s", file_username, file_password) != EOF)
    {
        if (strcmp(username, file_username) == 0 && strcmp(password, file_password) == 0)
        {
            printf("\nLogin successful. Welcome, %s!\n", username);
            fclose(fp);
            return;
        }
    }
    printf("\nInvalid username or password. Please try again.\n");
    fclose(fp);
}
void User()
{
    int choice;
    printf ("\n-------Welcome to the User Portal!-------\n");
    printf("\nPlease select an option:\n");
    printf ("If you are a new user, please select option 1 to register.\n");
    printf ("If you are an existing user, please select option 2 to login.\n");

    printf("\n1. Register\n2. Login\nEnter your choice: ");
    scanf("%d", &choice);
    do 
  {  
    switch (choice)
    {
        case 1:
            user_register();
            break;
        case 2:
            user_login();
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }
   } while (choice!= 1 && choice !=2);
}
