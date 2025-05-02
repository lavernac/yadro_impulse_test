#pragma once

void count_lines(char *filename, CSVFile_t *csv_file);
void file_to_str(char **buffer, char *filename, char **buffer_cpy);
int get_header(char *str_file, CSVFile_t *csv_file);
int init_csv(CSVFile_t *csv_file);
int get_data(char *str_file, CSVFile_t *csv_file);