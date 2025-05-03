#include <stdlib.h>
#include <string.h>

#include "csv_read.h"
#include "helpers.h"

Errors read_csv(char *file_to_read, CSVFile_t *csv_file) {
  Errors ret_status = OK;
  char *str_file = NULL, *str_file_cpy = NULL;
  if (csv_file) {
    file_to_str(&str_file, file_to_read, &str_file_cpy);
    count_lines(str_file, csv_file);
    get_header(str_file, csv_file);
    ret_status = init_csv(csv_file);
    if (ret_status == OK)
      get_data(str_file_cpy, csv_file);
    else
      printf("Init memmory error!\n");

    free(str_file);
    free(str_file_cpy);
  }
  return ret_status;
}

void print_csv(CSVFile_t *csv_file) {
  if (csv_file) {
    for (int i = 0; i < csv_file->cols_num; i++)
      printf(",%s", csv_file->headers[i]);
    printf("\n");
    for (int i = 0; i < csv_file->rows_num - 1; i++) {
      printf("%d,", csv_file->rows_nums[i]);
      for (int j = 0; j < csv_file->cols_num; j++) {
        if (j != csv_file->cols_num - 1)
          printf("%.1lf,", csv_file->data[i][j]);
        else
          printf("%.1lf", csv_file->data[i][j]);
      }

      printf("\n");
    }
    printf("\n");
  }
}

void free_csv(CSVFile_t *csv) {
  for (int i = 0; i < csv->cols_num; i++)
    if (csv->headers[i]) free(csv->headers[i]);
  if (csv->headers) free(csv->headers);

  for (int i = 0; i < csv->rows_num; i++) {
    if (csv->data[i]) free(csv->data[i]);
  }
  if (csv->data) free(csv->data);
  if (csv->rows_nums) free(csv->rows_nums);
}