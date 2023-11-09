#include "main.h"

unsigned int new(char *f_name, char *l_name, char *bvn, char *pin){
    int errnumber;
    unsigned int acc_num;
    char acc_num_str[11];
    cJSON *account, *obj_store = load_object_from_file();
    if (obj_store == NULL){
        obj_store = cJSON_CreateObject();
        if (obj_store == NULL) goto error;
    }

    do {
        acc_num = random();
        sprintf(acc_num_str, "%010d", acc_num);
    } while (cJSON_GetObjectItem(obj_store, acc_num_str));

    account = cJSON_CreateObject();
    if (account == NULL) goto error;

    if (cJSON_AddStringToObject(account, "f_name", f_name) == NULL) goto error;
    if (cJSON_AddStringToObject(account, "l_name", l_name) == NULL) goto error;
    if (cJSON_AddStringToObject(account, "bvn", bvn) == NULL) goto error;
    if (cJSON_AddStringToObject(account, "acc_num", acc_num_str) == NULL) goto error;
    if (cJSON_AddStringToObject(account, "pin", pin) == NULL) goto error;
    if (cJSON_AddNumberToObject(account, "balance", 0) == NULL) goto error;

    errnumber = create_sql_table();
    if (errnumber) goto errsql;
    errnumber = create_new_sql_entry(acc_num_str, f_name, l_name, bvn, pin);
    if (errnumber) goto errsql;

    cJSON_AddItemToObject(obj_store, acc_num_str, account);
    add_object_to_json_file(obj_store);
    errnumber = acc_num;
    goto clear;

error:
    fprintf(stderr, "Failed to Create cJSON object.\n");
errsql:
    errnumber = 0;
    cJSON_Delete(account);
clear:
    cJSON_Delete(obj_store);
    return(acc_num);
}


/**
 * Perform a deposit operation.
 *
 * @param acc_num The account number to perform the deposit on.
 * @param amount The amount to be deposited.
 * @return 0 if the deposit operation is successful, -1 if there is an error.
 */
int deposit(char *acc_num, char *amount){
    int errnumber;
    cJSON *balance, *account, *obj_store = load_object_from_file();

    if (obj_store == NULL) goto error;

    account = cJSON_GetObjectItem(obj_store, acc_num);
    if (account == NULL) goto error;

    balance = cJSON_GetObjectItem(account, "balance");
    if (balance == NULL) goto error;

    balance->valuedouble += atof(amount);

    errnumber = update_sqlite_database(acc_num, balance->valuedouble);
    if (errnumber) goto errsql;

    cJSON_ReplaceItemInObject(account, "balance", balance);
    cJSON_ReplaceItemInObject(obj_store, acc_num, account);
    add_object_to_json_file(obj_store);
    errnumber = 0;
    goto clear;

error:
    fprintf(stderr, "Failed to Create cJSON object.\n");
    errnumber = -1;
errsql:
    cJSON_Delete(account);
    cJSON_Delete(balance);
clear:
    cJSON_Delete(obj_store);
    return(errnumber);
}


/**
 * Withdraws a specified amount from a bank account.
 *
 * @param acc_num The account number as a string.
 * @param pin The PIN number as a string.
 * @param amount The withdrawal amount as a string.
 * @return An integer representing the error number. A value of 0 indicates success, -1 indicates a failure to create the cJSON object, 2 indicates an incorrect PIN, and 3 indicates insufficient funds.
 */
int withdraw(char *acc_num, char *pin, char* amount){
    double new_bal;
    int errnumber;
    cJSON *balance, *account, *corr_pin, *obj_store = load_object_from_file();

    if (obj_store == NULL) goto error;

    account = cJSON_GetObjectItem(obj_store, acc_num);
    if (account == NULL) goto error;

    balance = cJSON_GetObjectItem(account, "balance");
    if (balance == NULL) goto error;

    corr_pin = cJSON_GetObjectItem(account, "pin");
    if (corr_pin == NULL) goto error;

    if (strcmp(pin, corr_pin->valuestring)){
        fprintf(stdout, "Incorrect pin.\n");
        errnumber = 2;
        goto clear;
    }

    new_bal = balance->valuedouble - atof(amount);
    if (new_bal < 0) {
        fprintf(stdout, "Insufficient funds.\n");
        errnumber = 3;
        goto clear;
    }
    balance->valuedouble = new_bal;

    errnumber = update_sqlite_database(acc_num, balance->valuedouble);
    if (errnumber) goto errsql;

    cJSON_ReplaceItemInObject(account, "balance", balance);
    cJSON_ReplaceItemInObject(obj_store, acc_num, account);
    add_object_to_json_file(obj_store);

    errnumber = 0;
    goto clear;

error:
    errnumber = -1;
    fprintf(stderr, "Failed to Create cJSON object.\n");
errsql:
    cJSON_Delete(account);
    cJSON_Delete(balance);
    cJSON_Delete(corr_pin);
clear:
    cJSON_Delete(obj_store);
    return(errnumber);
}



/**
 * Checks the account balance and PIN for a given account number.
 *
 * @param acc_num The account number to check.
 * @param pin The PIN to verify.
 * @return An error number based on the outcome of the checks:
 *         -1 if failed to create cJSON object.
 *          0 if PIN is correct and balance is printed.
 *          2 if PIN is incorrect.
 */
int check(char *acc_num, char *pin){
    int errnumber;
    cJSON *balance, *corr_pin, *account, *obj_store = load_object_from_file();

    if (obj_store == NULL) goto error;

    account = cJSON_GetObjectItem(obj_store, acc_num);
    if (account == NULL) goto error;

    balance = cJSON_GetObjectItem(account, "balance");
    if (balance == NULL) goto error;

    corr_pin = cJSON_GetObjectItem(account, "pin");
    if (corr_pin == NULL) goto error;

    if (strcmp(pin, corr_pin->valuestring)){
        fprintf(stdout, "Incorrect pin.\n");
        errnumber = 2;
        goto clear;
    }

    printf("Your balance is %f\n", balance->valuedouble);
    errnumber = 0;
    goto clear;

error:
    fprintf(stderr, "Failed to Create cJSON object.\n");
    errnumber = -1;
    cJSON_Delete(account);
    cJSON_Delete(balance);
    cJSON_Delete(corr_pin);
clear:
    cJSON_Delete(obj_store);
    return(errnumber);
}


/**
 * liquidate - Function to liquidate an account
 * @acc_num: The account number to be liquidated
 * @pin: The PIN associated with the account
 * @bvn: The BVN (Bank Verification Number) associated with the account
 *
 * Return: An error number indicating the result of the liquidation process.
 *         A value of 0 indicates success, while negative values indicate errors.
 */
int liquidate(char *acc_num, char *pin, char *bvn)
{
    int errnumber;
    cJSON *corr_bvn, *corr_pin, *account, *obj_store = load_object_from_file();

    if (obj_store == NULL)
        goto error;

    account = cJSON_GetObjectItem(obj_store, acc_num);
    if (account == NULL)
        goto error;

    corr_pin = cJSON_GetObjectItem(account, "pin");
    if (corr_pin == NULL)
        goto error;
    if (strcmp(pin, corr_pin->valuestring))
    {
        fprintf(stdout, "Incorrect pin.\n");
        errnumber = 2;
        goto clear;
    }

    corr_bvn = cJSON_GetObjectItem(account, "bvn");
    if (corr_bvn == NULL)
        goto error;
    if (strcmp(pin, corr_pin->valuestring))
    {
        fprintf(stdout, "Incorrect BVN.\n");
        errnumber = 3;
        goto clear;
    }

    errnumber = delete_from_table(acc_num);
    if (errnumber)
        goto errsql;

    cJSON_DeleteItemFromObject(obj_store, acc_num);
    add_object_to_json_file(obj_store);
    errnumber = 0;
    goto clear;

error:
    fprintf(stderr, "Failed to Create cJSON object.\n");
    errnumber = -1;
errsql:
    cJSON_Delete(account);
    cJSON_Delete(corr_bvn);
    cJSON_Delete(corr_pin);
clear:
    cJSON_Delete(obj_store);
    return (errnumber);
}
