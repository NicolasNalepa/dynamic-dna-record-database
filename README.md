# Dynamic DNA Record Database

A C library for loading, analyzing, modifying, and safely releasing a dynamically allocated DNA-record database stored in CSV format.

## Features

- Parses DNA records from a CSV file into structures
- Allocates sequence storage dynamically
- Finds the longest DNA sequence
- Counts records by classification
- Calculates average sequence length
- Adds and removes records with `realloc`
- Releases all allocated memory through a dedicated cleanup function

## Compile

```bash
gcc -Wall -Wextra -std=c99 -c dna_database.c
```

`dna_db.csv` provides sample input data for integration with a driver program.
