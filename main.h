#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

// #include <cjson/cJSON.h>
#include "cJSON.h"
#include <sqlite3.h>

// Input parsing functions
char *input_prompt(void);
char *format_input(char *command);
int check_if_exit(char *command);
char **arg_split(char *input);
void display_help_generic(void);

// Data management
unsigned int new(char *f_name, char *l_name, char* bvn, char *pin);
int deposit(char *acc_num, char *amount);
int withdraw(char *acc_num, char *pin, char* amount);
int check(char *acc_num, char *pin);
int liquidate(char *acc_num, char *pin, char* bvn);

// Find data in store
cJSON *load_object_from_file();
void add_object_to_json_file(cJSON *account);

// SQLite functions
sqlite3 *create_connection();
int exec_sql(char *sql, sqlite3 *db, int flag);
int create_sql_table();
int create_new_sql_entry(char *acc_num, char *f_name, char *l_name, char *bvn, char *pin);
int update_sqlite_database(char* acc_num, double balance);
int delete_from_table(char *acc_num);

#endif
