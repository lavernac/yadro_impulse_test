#pragma once

#include "csv_read.h"

typedef struct {
    char **formulas;
    int *row_indexes;
    int *col_indexes;
    int formulas_count;
    int capacity;
    int *ready;
}Formulas_t;

void count_lines(char *filename, CSVFile_t *csv_file);
int get_header(char *str_file, CSVFile_t *csv_file);
Errors file_to_str(char **buffer, char *filename, char **buffer_cpy);
Errors init_csv(CSVFile_t *csv_file);
Errors get_data(char *str_file, CSVFile_t *csv_file);
Errors calculate_value(Formulas_t *formulas, CSVFile_t *csv_file, int i);
char **convert_file_to_rows(char *str_file, int rows_num);


Errors check_index_cell(char *cell);