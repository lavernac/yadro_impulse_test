#pragma once

typedef struct {
    char **headers;
    double **data;
}CSV_file;

void read_csv(char *filename, CSV_file *csv_file);
void print_csv(CSV_file *csv_file);