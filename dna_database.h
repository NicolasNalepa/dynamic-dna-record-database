#ifndef DNA_DATABASE_H
#define DNA_DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * struct DNARecord: 
 * Holds the data for a single DNA record.
 *
 * Field 1: id
 * A fixed-size string for the record ID (e.g., "ID001").
 *
 * Field 2: classification
 * A fixed-size string for the classification (e.g., "Mammal").
 *
 * Field 3: sequence
 * A pointer to a dynamically allocated string holding the DNA sequence.
 */
struct DNARecord {
    char id[10];
    char classification[50];
    char *sequence;
};

struct DNARecord *load_database(const char *filename, int *record_count);
char *find_longest_sequence(struct DNARecord *database, int record_count, int *longest_length);
int count_by_classification(struct DNARecord *database, int record_count, const char *classification);
double calculate_average_sequence_length(struct DNARecord *database, int record_count);
void free_database(struct DNARecord *database, int record_count);
int add_record(struct DNARecord **db_ptr, int *record_count, const char *id, const char *classification, const char *sequence);
int delete_record(struct DNARecord **db_ptr, int *record_count, const char *id_to_delete);

#endif // DNA_DATABASE_H