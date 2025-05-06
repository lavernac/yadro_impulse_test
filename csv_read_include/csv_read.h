#pragma once

#include <stdio.h>

typedef enum  {
    OK,
    CANT_OPEN_FILE_ERROR,
    MEMORY_ALLOCATE_ERROR,
    INCORRECT_INDEX_OF_ROW,
    INCORRECT_CELL_ERROR,
    UNKNOWN_OPERATION_ERROR,
    BAD_ARG_ERROR,
    DIV_ZERO_ERROR
}Errors;

typedef struct {
    double **data;
    char **headers;
    int *rows_nums;
    int cols_num;
    int rows_num; 
}CSVFile_t;

Errors read_csv(char *filename, CSVFile_t *csv_file);
void print_csv(CSVFile_t *csv_file);
void free_csv(CSVFile_t *);