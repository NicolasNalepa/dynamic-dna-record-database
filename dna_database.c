#include "dna_database.h"

#define MAX_DNA_LENGTH 5096



/*
Function: Load_database 
Purpose: Reads a csv DNA database file and loads all record into an allocated array of DNARecord struct 
Parameters: filename - name of the file to open 
            record_count - pointer used to return the number of valid record 
Returns: a pointer allocated to an array of DNARecord structs or NULL if the file cannot open or contains no valid data
Side eefects:  - allocated memory that must late be freed with free_database()
               - updates *record_count based on the number of records read 
*/

struct DNARecord *load_database(const char *filename, int *record_count) {
    *record_count = 0;

    FILE *fp = fopen(filename, "r");
    if (!fp) return NULL; //file cannot be opened

    char line[4096];

    // count lines through first pass 
    while (fgets(line, sizeof(line), fp)) {
        if (strlen(line)>1)
        (*record_count)++;

    }

    if (*record_count ==0) {
        fclose(fp);
        return NULL; //means there is no valid data 
    }

        rewind(fp); //go back to the start of the file 

    // make array of DNArecord structs 

struct DNARecord *db = malloc(sizeof(struct DNARecord) * (*record_count));
    
if (!db) {
        fclose(fp);
        return NULL;
    }

int index = 0;

// second pass through 

while (fgets(line, sizeof(line), fp) && index < *record_count) {
    char *id = strtok(line, ",");
    char *classif = strtok(NULL, ",");
    char *seq = strtok(NULL, "\n");


    if (!id || !classif || !seq)
    continue; // this skips the malformed lines 

    // now copt the fixes-size fields

    strcpy(db[index].id, id);
    strcpy(db[index].classification, classif);

    //allocates the dynamic memor for the DNA sequence 

    db[index].sequence = malloc(strlen(seq) + 1);
        strcpy(db[index].sequence, seq);

        index++;
}

fclose(fp);
return db;


}


/*
Function: find_longest_sequence
Prupose: Finds the DNA record with the longest sequence 
Parameters: 
database        - array of DNARecord structs 
record_count    - number of record in the array 
longest_length  - pointer used to return the length of the longest sequence
Returns: A pointer or the ID string of the record with the longest sequence. As well as returns NULL if there are zero records.
side effects: Writes the longest sequence length into *longest_length.
*/

char *find_longest_sequence(struct DNARecord *database, int record_count, int *longest_length) {

    if (record_count == 0) {
        *longest_length = 0;
        return NULL;
    }


        int maxIndex = 0;
        int maxLen = strlen(database[0].sequence);

        //loops through all the records

        for (int i = 1; i < record_count; i++) {
            int len = strlen(database[i].sequence);
       
            if (len > maxLen) {
                maxLen = len;
                maxIndex = i;
            }
        }

        *longest_length = maxLen;
        return database[maxIndex].id;



}


/*
Function: count_by_classification
Purpose: counts how many DNA records matha. specific classification.
Parameters: 
databse         - array of DNARecord structs 
record_count    - number of records 
classification  classification value to math 
Returns: the number of matching records 
side effects: None.
*/

int count_by_classification(struct DNARecord *database, int record_count, const char *classification) {

    int count = 0;

    //check each record 
    for (int i =0; i < record_count; i++){

        if(strcmp(database[i].classification, classification) == 0 ){
            count++;
        }
    }

    return count;



}



/*
Function: calculate_average_sequence_length
Purpose: computes that average length of all DNA sequences in the database
Parameters: 
database        - array of DNARecord structs
record_count    - number of records 
Returns: the average sequence length as a double, returns 0.0 if the database is empty 
side effects: None.
*/

double calculate_average_sequence_length(struct DNARecord *database, int record_count) {

    if (record_count == 0){
    return 0.0;

}

int total = 0;

for (int i = 0; i < record_count; i ++) {
    total += strlen(database[i].sequence);

    }


    return (double)total / record_count;


}


/*
Function: add_record 
Purpose: dynamically increases the DNA databse by 1 record and adds new data 
Parameters: 
    db_ptr        - pointer to the database pointer (allows realloc to modify it)
    record_count  - pointer to the number of records
    id            - ID string of the new record
    classification- classification string of the new record
    sequence      - DNA sequence to store (memory is dynamically allocated)
Returns: 1 on success, 0 if memory allocation fails
side effects: 
    - Reallocates the database array.
    - Updates *record_count.
    - Allocates memory for the DNA sequence.

*/

int add_record(struct DNARecord **db_ptr, int *record_count, const char *id, const char *classification, const char *sequence) {

    // attemp to expand that database by 1 

    struct DNARecord *temp = realloc (*db_ptr, sizeof(struct DNARecord) * (*record_count + 1));

        if (temp == NULL) {
            return 0; // failed 


        }


        // updates the pointer if realloc succeeds 

        *db_ptr = temp;

        // write into a new spot 

    strcpy((*db_ptr)[*record_count].id, id);
    strcpy((*db_ptr)[*record_count].classification, classification);

     (*db_ptr)[*record_count].sequence = malloc(strlen(sequence) + 1);

    if ((*db_ptr)[*record_count].sequence == NULL) {
        return 0;
    }

    strcpy((*db_ptr)[*record_count].sequence, sequence);

    // Increase record count 

    
    (*record_count)++;

    return 1;
}


/*
Function: delete_function
Purpose: removes a record with a matching ID from the database
Parameters: 
    db_ptr        - pointer to the database pointer
    record_count  - pointer to the number of records
    id_to_delete  - the ID of the record to remove

Returns:
    1 if the record was successfully deleted.
    0 if no matching record was found.

Side Effects:
    - Frees memory for the deleted record's sequence.
    - Shifts remaining records left.
    - Uses realloc() to shrink the database.
    - Updates *record_count.
*/

int delete_record(struct DNARecord **db_ptr, int *record_count, const char *id_to_delete) {

struct DNARecord *db = *db_ptr;

int index = -1;

// find the record with a matching id 

for (int i = 0; i < *record_count; i++) {
    if (strcmp(db[i].id, id_to_delete) == 0 ){
        index =i;
        break;
    }
}

// if match isnt found, return failure 

if (index == -1) {
    return 0;
}

// free the memory for the sequence 

free (db[index].sequence);

//shift everything after it to the left by 1 

for (int i = index; i < *record_count -1; i++){
    db[i] = db[i + 1];
}


// reduce the count 

(*record_count)--;

// shrink the database array using realloc 

    struct DNARecord *new_db = realloc(db, sizeof(struct DNARecord) * (*record_count));

    if (new_db != NULL) {
        *db_ptr = new_db; // only update if the realloc succeeded

    }

return 1; // for success


}



/*
Function: free_database
Purpose:
    Frees all memory allocated for the DNA database.

Parameters:
    database     - pointer to array of DNARecord structs
    record_count - number of records in the array

Returns:
    Nothing.

Side Effects:
    - Frees each record's sequence (malloc'd memory).
    - Frees the database array itself.
*/

void free_database(struct DNARecord * database, int record_count) {

// free each sequence 
for (int i = 0; i < record_count; i++) {
    free(database[i].sequence);
}

// free the array of structs 

free(database);

}
