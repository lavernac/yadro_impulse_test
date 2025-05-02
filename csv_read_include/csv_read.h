#pragma once

#include <stdio.h>

#define OK 0
#define MEMORY_ALLOCATE_ERROR 1

typedef struct {
    char **headers;
    int cols_num;
    int *rows_nums;
    int rows_num; 
    int **data;
}CSVFile_t;

int read_csv(char *filename, CSVFile_t *csv_file);
void print_csv(CSVFile_t *csv_file);
void free_csv(CSVFile_t *);