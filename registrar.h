
#ifndef REGISTRAR_H
#define REGISTRAR_H

#include <stdio.h>
#include <string.h>
#include "md5.h"   // for get_user_md5
#include "user.h"
#include "admin.h"


// External setup key
extern const char *Registrar_Key;


//Samiha (252-35-412)
// Helper function prototypes
void read_input(char *buffer, int size, const char *prompt);
int get_menu_choice();
FILE* safe_open(const char *filename, const char *mode);
int confirm_password(const char *pass, const char *confirm);
void hash_password(const char *password, char *output);
// Registrar function prototypes
void registrar_register();
int registrar_login(char *logged_user);
void registrar_view_ratings();
void registrar_dashboard(const char *username);
void Registrar();

#endif
