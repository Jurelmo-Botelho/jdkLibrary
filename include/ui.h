#ifndef UI_H
#define UI_H

#include "auth.h"

// Menus principais
void ui_show_main_menu(void);
void ui_show_admin_menu(void);
void ui_show_student_menu(void);
void ui_show_book_menu(void);
void ui_show_report_menu(void);

// Login
void ui_show_login_header(void);
int ui_get_login_id(void);
char* ui_get_login_password(void);
void ui_show_login_success(const char *name);
void ui_show_login_failed(void);

// Inputs genéricos
int ui_get_int(const char *prompt);
char* ui_get_string(const char *prompt, char *buffer, int size);
void ui_wait_enter(void);
void ui_clear_screen(void);

// Mensagens
void ui_show_message(const char *msg);
void ui_show_error(const char *msg);
void ui_show_success(const char *msg);

#endif