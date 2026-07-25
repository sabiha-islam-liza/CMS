#include <stdio.h>
#include <string.h>

void forgetPassword()
{
    FILE *file, *temp;

    int id, inputId;
    char username[30], inputName[30];
    char password[30];
    char newPass[30];
    char confirmPass[30];
    char role[20];

    int found = 0;

    file = fopen("users.txt", "r");
    temp = fopen("temp.txt", "w");

    printf("\n===== FORGET PASSWORD =====\n");

    printf("Enter ID: ");
    scanf("%d", &inputId);

    printf("Enter Username: ");
    scanf("%s", inputName);

    while(fscanf(file,"%d %s %s %s",&id,username,password,role)!=EOF)
    {
        if(id == inputId && strcmp(username,inputName)==0)
        {
            found = 1;

            printf("Create New Password: ");
            scanf("%s", newPass);

            printf("Confirm Password: ");
            scanf("%s", confirmPass);

            if(strcmp(newPass,confirmPass)==0)
            {
                strcpy(password,newPass);
                printf("\nPassword Changed Successfully!\n");
            }
            else
            {
                printf("\nPassword Doesn't Match!\n");
            }
        }

        fprintf(temp,"%d %s %s %s\n",id,username,password,role);
    }

    fclose(file);
    fclose(temp);

    remove("users.txt");
    rename("temp.txt","users.txt");

    if(found==0)
    {
        printf("\nInvalid ID or Username!\n");
    }
}
