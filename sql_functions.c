#include "main.h"


/**
 * This function is a callback function used in the SQLite library.
 * It takes in the result of a query and prints the column names and corresponding values.
 *
 * @param NotUsed A pointer to user-defined data that is not used in this function.
 * @param argc The number of columns in the query result.
 * @param argv An array of strings representing the values of each column in the query result.
 * @param azColName An array of strings representing the names of each column in the query result.
 * @return int Returns 0.
 */
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}


/**
 * Creates a connection to an SQLite database.
 *
 * Returns:
 *     A pointer to the SQLite database connection.
 */
sqlite3 *create_connection(){
    sqlite3 *db;
    int rc;
    char *sql;

    rc = sqlite3_open("TMS.db", &db);
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(NULL);
    }

    return(db);
}


/**
 * Executes an SQL statement using the SQLite library.
 *
 * @param sql - The SQL statement to be executed.
 * @param db - The SQLite database connection.
 * @param flag - An integer flag indicating the type of operation being performed.
 * @return The result code of the SQL execution.
 */
int exec_sql(char *sql, sqlite3 *db, int flag){
    int rc;
    char *zErrMsg = 0;
    char *success_strings[4] = {"Table created successfully", "Record inserted successfully", "Record updated successfully", "Record deleted successfully"};

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "%s\n", success_strings[flag]);
    }

    // Diffrentiate from cJSON error
    if (rc == -1) rc = -2;

    sqlite3_close(db);
    return(rc);
}


/**
 * Creates a table named 'CUSTOMERS' in an SQLite database.
 *
 * @return The result of executing the SQL statement, indicating whether the table was successfully created or not.
 */
int create_sql_table(){
    sqlite3 *db = create_connection();

    char sql[256] = "CREATE TABLE IF NOT EXISTS CUSTOMERS("  \
        "ACCOUNT_NUMBER CHAR(10)     PRIMARY KEY     NOT NULL," \
        "FIRST_NAME     TEXT," \
        "LAST_NAME      TEXT," \
        "BVN            CHAR(11)                NOT NULL," \
        "PIN            CHAR(4)                 NOT NULL," \
        "BALANCE        REAL);";

    return exec_sql(sql, db, 0);
}


/**
 * Inserts a new entry into a SQLite database table named 'CUSTOMERS'.
 *
 * @param acc_num   The account number of the customer.
 * @param f_name    The first name of the customer.
 * @param l_name    The last name of the customer.
 * @param bvn       The BVN (Bank Verification Number) of the customer.
 * @param pin       The PIN of the customer.
 * @return          The result of the 'exec_sql' function, indicating the success or failure of the SQL query execution.
 */
int create_new_sql_entry(char *acc_num, char *f_name, char *l_name, char *bvn, char *pin){
    char sql[256];
    sqlite3 *db = create_connection();

    sprintf(
        sql,
        "INSERT INTO CUSTOMERS " \
        "(ACCOUNT_NUMBER, FIRST_NAME, LAST_NAME, BVN, PIN) " \
        "VALUES ('%s', '%s', '%s', '%s', '%s');",
        acc_num, f_name, l_name, bvn, pin
    );

    return exec_sql(sql, db, 1);
}

/**
 * Updates the balance of a customer in a SQLite database.
 *
 * @param acc_num The account number of the customer.
 * @param balance The new balance of the customer.
 * @return The result of executing the SQL statement, indicating the success or failure of the update operation.
 */
int update_sqlite_database(char* acc_num, double balance){
    char sql[128];
    sqlite3 *db = create_connection();

    sprintf(
        sql,
        "UPDATE CUSTOMERS "  \
        "SET BALANCE = %f " \
        "WHERE ACCOUNT_NUMBER = '%s';", balance, acc_num
    );

    return exec_sql(sql, db, 2);
}


/**
 * Deletes a row from the "CUSTOMERS" table in a SQLite database based on the provided account number.
 *
 * @param acc_num A string representing the account number of the customer to be deleted from the table.
 * @return An integer value representing the result of executing the SQL query.
 */
int delete_from_table(char *acc_num){
    char sql[64];
    sqlite3 *db = create_connection();

    sprintf(
        sql,
        "DELETE FROM CUSTOMERS "  \
        "WHERE ACCOUNT_NUMBER = '%s';", acc_num
    );

    return exec_sql(sql, db, 3);
}
