#pragma once

#include <stdio.h>

typedef enum  {
    OK,
    MEMORY_ALLOCATE_ERROR,
    INCORRECT_INDEX_OF_ROW,
    INCORRECT_CELL_ERROR
}Errors;

typedef struct {
    char **headers;
    int cols_num;
    int *rows_nums;
    int rows_num; 
    double **data;
}CSVFile_t;

Errors read_csv(char *filename, CSVFile_t *csv_file);
void print_csv(CSVFile_t *csv_file);
void free_csv(CSVFile_t *);