#include<stdio.h>
#include <string.h>

#define setup_key "syntexerror"


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
int admin_login()
        {
            char username [50], password[50];
            char file_username[50], file_password[50];
            FILE *fp = fopen("admin.txt", "r");
            fscanf(fp, "%s\n%s", file_username, file_password);
            
            if (fp == NULL)
            {
                printf("\nNo admin registered yet.\n");
                return 0;
            }

            else 
            {
                printf ("\nEnter your username: ");
                scanf ("%s", username);
                printf ("\n Enter your password: ");
                scanf ("%s" ,  password);
                if (strcmp(username, file_username) == 0 && strcmp(password, file_password) == 0)
                {
                    printf("\nLogin successful.\n");
                    return 1;
                }
                else
                {
                    printf("\nInvalid username or password.\n");
                    return 0;
                }
            }

        }

int main ()
  {
int choice;
do {
    printf("------This is a Comlaint Management System------\n");
    
    printf("1.Admin register.\n");
    printf ("2. Admin login.\n");
    printf ("3.Exit\n");
    printf ("Enter your choice: ");
    scanf("%d", &choice);
    switch (choice)
    {
        case 1:
            adminregister();
            break;
        case 2:
            admin_login();
            break;
        case 3:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
    }


  } 
while(choice != 3); 
    return 0;
}
