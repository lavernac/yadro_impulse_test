#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "csv_read.h"
#include "helpers.h"

void count_lines(char *str, CSVFile_t *csv_file) {
  int lines = 0;
  if (str)
    while (*str) {
      if (*str == '\n') lines++;
      str++;
    }

  csv_file->rows_num = lines;
}

Errors file_to_str(char **buffer, char *filename, char **buffer_cpy) {
  long length = 0;
  Errors ret_status = OK;
  FILE *f = fopen(filename, "rb");

  if (f) {
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    *buffer = calloc(length + 1, sizeof(char));
    *buffer_cpy = calloc(length + 1, sizeof(char));
    if (*buffer) {
      fread(*buffer, 1, length, f);
    }
    strncpy(*buffer_cpy, *buffer, length + 1);
    fclose(f);
  } else {
    ret_status = CANT_OPEN_FILE_ERROR;
    printf("Can`t open file!\n");
  }

  return ret_status;
}

void get_col(char **col_name, char *header_cpy, CSVFile_t *csv_file,
             int *cols_num) {
  if (*cols_num == 0)
    *col_name = strtok(header_cpy, ",");
  else {
    *col_name = strtok(NULL, ",");
  }
  if (*col_name) {
    csv_file->headers[*cols_num] =
        malloc((strlen(*col_name) + 1) * sizeof(char));
    strncpy(csv_file->headers[*cols_num], *col_name, strlen(*col_name) + 1);
    (*cols_num)++;
  }
}

int get_header(char *str_file, CSVFile_t *csv_file) {
  char *header = strtok(str_file, "\n"), header_cpy[strlen(header) + 1],
       *col_name = NULL;
  strncpy(header_cpy, header, strlen(header) + 1);
  int cols_num = 0;

  col_name = strtok(header_cpy, ",");
  while (col_name) {
    col_name = strtok(NULL, ",");
    cols_num++;
  }

  csv_file->headers = malloc((cols_num) * sizeof(char *));
  csv_file->cols_num = cols_num;
  cols_num = 0;
  strncpy(header_cpy, header, strlen(header) + 1);
  get_col(&col_name, header_cpy, csv_file, &cols_num);
  while (col_name) get_col(&col_name, header_cpy, csv_file, &cols_num);

  return cols_num;
}

Errors init_csv(CSVFile_t *csv_file) {
  Errors ret_status = OK;
  csv_file->rows_nums = malloc(csv_file->rows_num * sizeof(int));
  if (!csv_file->rows_nums) ret_status = MEMORY_ALLOCATE_ERROR;
  if (ret_status == OK)
    csv_file->data = malloc(csv_file->rows_num * sizeof(double *));
  if (!csv_file->data) ret_status = MEMORY_ALLOCATE_ERROR;

  for (int i = 0; i < csv_file->rows_num; i++) {
    if (ret_status == OK) {
      csv_file->data[i] = malloc(csv_file->cols_num * sizeof(double));
      for (int j = 0; j < csv_file->cols_num; j++) csv_file->data[i][j] = NAN;
    }
    if (!csv_file->data[i]) ret_status = MEMORY_ALLOCATE_ERROR;
  }
  return ret_status;
}

void get_row(char **row, char **rows, int *row_counter, char *str_file) {
  if (*row_counter == 0)
    *row = strtok(str_file, "\n");
  else
    *row = strtok(NULL, "\n");

  if (*row) {
    rows[*row_counter] = malloc((strlen(*row) + 1) * sizeof(char));
    strncpy(rows[*row_counter], *row, strlen(*row) + 1);
    (*row_counter)++;
  }
}

char **convert_file_to_rows(char *str_file, int rows_num) {
  char **rows = malloc(rows_num * (sizeof(char *))), *row = NULL;
  int row_counter = 0;
  get_row(&row, rows, &row_counter, str_file);
  while (row) get_row(&row, rows, &row_counter, str_file);

  return rows;
}