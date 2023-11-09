#include "main.h"


/**
 * This function reads a JSON object from a file and returns it as a cJSON object.
 *
 * @return A cJSON object representing the JSON data read from the file. If an error occurs, `NULL` is returned.
 */
cJSON *load_object_from_file(){
    FILE *fp = fopen("cjson_store.json", "r");
    if (fp == NULL) {
       fprintf(stderr, "Error: Unable to open the file.\n");
       return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = (char *)malloc(fileSize + 1);
    if (buffer == NULL) {
       fprintf(stderr, "Error: Unable to allocate memory.\n");
       fclose(fp);
       return NULL;
    }

    size_t bytesRead = fread(buffer, 1, fileSize, fp);
    if (bytesRead < fileSize) {
       fprintf(stderr, "Error: Unable to read the entire file.\n");
       fclose(fp);
       free(buffer);
       return NULL;
    }
    fclose(fp);

    buffer[fileSize] = '\0';

    cJSON *parsedObject = cJSON_Parse(buffer);
    if (parsedObject == NULL) {
       fprintf(stderr, "Error: Unable to parse the JSON.\n");
       free(buffer);
       return NULL;
    }

    free(buffer);
    return parsedObject;
}

/**
 * Adds a cJSON object to a JSON file.
 *
 * @param object A pointer to a cJSON object representing the JSON data to be added to the file.
 *
 * @return None. The function writes the JSON data to the file "cjson_store.json" if successful.
 * If any error occurs, an error message is printed and the function returns without modifying the file.
 */
void add_object_to_json_file(cJSON *object){
    if (object == NULL) {
       fprintf(stderr, "Error: Invalid object.\n");
       return;
    }

    FILE *fp = fopen("cjson_store.json", "w");
    if (fp == NULL) {
       fprintf(stderr, "Error: Unable to open the file.\n");
       return;
    }

    char *jsonString = cJSON_Print(object);
    if (jsonString == NULL) {
       fprintf(stderr, "Error: Unable to print the JSON.\n");
       fclose(fp);
       return;
    }

    fputs(jsonString, fp);
    fclose(fp);
    free(jsonString);
}
