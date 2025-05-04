#pragma once

#include "csv_read.h"

typedef struct {
    char **formulas;
    int *row_indexes;
    int *col_indexes;
    int formulas_count;
    int capacity;
}Formulas_t;

void count_lines(char *filename, CSVFile_t *csv_file);
void file_to_str(char **buffer, char *filename, char **buffer_cpy);
int get_header(char *str_file, CSVFile_t *csv_file);
Errors init_csv(CSVFile_t *csv_file);
Errors get_data(char *str_file, CSVFile_t *csv_file);