#include "helpers.h"

#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "csv_read.h"

#define ISVALUE 1
#define ISFORMULA 2

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

Errors check_index_cell(char *cell) {
  int ret_status = OK;
  for (int i = 0, len = strlen(cell); i < len; i++)
    if (!isdigit(cell[i])) {
      printf("Incorrect index of row!\n");
      ret_status = INCORRECT_INDEX_OF_ROW;
    }
  return ret_status;
}

Errors get_index_cell(Errors ret_status, CSVFile_t *csv_file, char *cell,
                      int i) {
  if (ret_status == OK && i != 0)
    csv_file->rows_nums[i - 1] = atoi(cell);
  else if (i != 0) {
    ret_status = INCORRECT_INDEX_OF_ROW;
  }
  return ret_status;
}

int check_data_cell(char *cell) {
  int cell_status = ISVALUE;
  if (cell[0] == '=')
    cell_status = ISFORMULA;
  else
    for (int i = 0, len = strlen(cell), dot_counter = 0, minuses_counter = 0;
         i < len; i++) {
      if ((!isdigit(cell[i]) && cell[i] != '.' && cell[0] != '-') ||
          dot_counter > 1 || minuses_counter > 1)
        cell_status = INCORRECT_CELL_ERROR;
      else if (cell[i] == '.')
        dot_counter++;
      else if (cell[i] == '-')
        minuses_counter++;
    }
  return cell_status;
}

void *my_realloc(void *ptr, int old_size, int new_size, int szof) {
  if (new_size == 0) {
    free(ptr);
    return NULL;
  }
  void *new_pointer = calloc(new_size, szof);
  if (new_pointer == NULL) return NULL;

  memcpy(new_pointer, ptr, old_size < new_size ? old_size : new_size);
  if (ptr) free(ptr);
  return new_pointer;
}

void add_formula_to_struct(char **cell, Formulas_t *formulas, int i, int *j) {
  if (formulas->formulas_count + 1 > formulas->capacity) {
    formulas->col_indexes =
        my_realloc(formulas->col_indexes, formulas->capacity,
                   formulas->capacity + 10, sizeof(int));
    formulas->row_indexes =
        my_realloc(formulas->row_indexes, formulas->capacity,
                   formulas->capacity + 10, sizeof(int));
    formulas->formulas = my_realloc(formulas->formulas, formulas->capacity,
                                    formulas->capacity + 10, sizeof(char *));
    formulas->ready = my_realloc(formulas->ready, formulas->capacity,
                                 formulas->capacity + 10, sizeof(int));
    formulas->capacity += 10;
  }

  formulas->formulas[formulas->formulas_count] =
      malloc((strlen(*cell) + 1) * sizeof(char));
  strncpy(formulas->formulas[formulas->formulas_count], *cell,
          strlen(*cell) + 1);
  formulas->col_indexes[formulas->formulas_count] = *j;
  formulas->row_indexes[formulas->formulas_count] = i;
}

Errors get_data_cell(char **cell, Formulas_t *formulas, CSVFile_t *csv_file,
                     int i, int *j) {
  Errors ret_status = OK;
  *cell = strtok(NULL, ",");
  if (*cell) {
    int cell_status = check_data_cell(*cell);
    switch (cell_status) {
      case ISVALUE:
        csv_file->data[i][(*j)++] = atof(*cell);
        break;
      case ISFORMULA:
        add_formula_to_struct(cell, formulas, i, j);
        csv_file->data[i][(*j)++] = NAN;
        (formulas->formulas_count)++;
        break;
      default:
        printf("Incorrect cell: %d - row index, %s - column name\n",
               csv_file->rows_nums[i], csv_file->headers[*j]);
        ret_status = INCORRECT_CELL_ERROR;
        csv_file->data[i][(*j)++] = NAN;
    }
  }
  return ret_status;
}

void free_tmp_data(char **rows, int rows_num, Formulas_t *formulas) {
  free(formulas->col_indexes);
  free(formulas->row_indexes);
  free(formulas->ready);
  for (int i = 0; i < formulas->formulas_count; i++)
    free(formulas->formulas[i]);
  free(formulas->formulas);
  for (int i = 0; i < rows_num; i++) free(rows[i]);
  free(rows);
}

int get_formula_index(Formulas_t *formulas, int i, int j) {
  int i_tmp = 0, flag = 1, index = 0;
  for (; i_tmp < (*formulas).formulas_count && flag; i_tmp++) {
    for (int j_tmp = 0; j_tmp < (*formulas).formulas_count && flag; j_tmp++) {
      if ((*formulas).row_indexes[index] == i_tmp &&
          (*formulas).col_indexes[index] == j_tmp)
        index++;
      if (i_tmp == i && j_tmp == j) flag = 0;
    }
  }

  return index - 1;
}

double decode_cell(char *formula, CSVFile_t *csv_file, char **end_of_opernad,
                   Formulas_t *formulas, int *is_good_arg) {
  int col_name_len = 0, row_len = 0;
  for (; !isdigit(formula[col_name_len]); col_name_len++);
  for (row_len = col_name_len; isdigit(formula[row_len]); row_len++);
  row_len -= col_name_len;
  *end_of_opernad = formula + col_name_len + row_len;

  char *col_name = calloc(col_name_len + 1, sizeof(char)),
       *row = calloc((row_len + 1), sizeof(char));
  strncpy(col_name, formula, col_name_len);
  strncpy(row, formula + col_name_len, row_len);

  int i = 0, j = 0, row_index = atof(row);
  for (; j < csv_file->cols_num && strcmp(col_name, csv_file->headers[j]); j++);
  for (; i < csv_file->rows_num && row_index != csv_file->rows_nums[i]; i++);

  free(col_name);
  free(row);
  double ret_data = NAN;
  if (j == csv_file->cols_num || i == csv_file->rows_num) {
    printf("Bad arg!\n");
    *is_good_arg = 0;
  }
  if (*is_good_arg && isnan(csv_file->data[i][j]))
    calculate_value(formulas, csv_file, get_formula_index(formulas, i, j));

  if (*is_good_arg) ret_data = csv_file->data[i][j];

  return ret_data;
}

Errors do_operation(CSVFile_t *csv_file, Formulas_t *formulas, char operation,
                    double arg1, double arg2, int is_good_arg, int i) {
  Errors ret_status = OK;
  if (is_good_arg) {
    switch (operation) {
      case '+':
        csv_file->data[(*formulas).row_indexes[i]][(*formulas).col_indexes[i]] =
            arg1 + arg2;
        break;
      case '-':
        csv_file->data[(*formulas).row_indexes[i]][(*formulas).col_indexes[i]] =
            arg1 - arg2;
        break;
      case '*':
        csv_file->data[(*formulas).row_indexes[i]][(*formulas).col_indexes[i]] =
            arg1 * arg2;
        break;
      case '/':
        if (arg2 == 0) {
          ret_status = DIV_ZERO_ERROR;
          printf("Div zero error: %d - row index, %s - col name\n",
                 csv_file->rows_nums[(*formulas).row_indexes[i]],
                 csv_file->headers[(*formulas).col_indexes[i]]);
        } else
          csv_file
              ->data[(*formulas).row_indexes[i]][(*formulas).col_indexes[i]] =
              arg1 / arg2;
        break;
      default:
        printf("Unknown operation: %d - row index, %s - col name\n",
               csv_file->rows_nums[(*formulas).row_indexes[i]],
               csv_file->headers[(*formulas).col_indexes[i]]);
        ret_status = UNKNOWN_OPERATION_ERROR;
    }
  } else {
    ret_status = BAD_ARG_ERROR;
  }

  return ret_status;
}

Errors calculate_value(Formulas_t *formulas, CSVFile_t *csv_file, int i) {
  Errors ret_status = OK;
  if ((*formulas).ready[i] != 1) {
    char *end_of_arg = NULL, operation = '\0';
    int is_good_arg = 1;
    double arg1 = decode_cell((*formulas).formulas[i] + 1, csv_file,
                              &end_of_arg, formulas, &is_good_arg);
    operation = *end_of_arg;
    double arg2 = decode_cell(
        (*formulas).formulas[i] + (end_of_arg - (*formulas).formulas[i]) + 1,
        csv_file, &end_of_arg, formulas, &is_good_arg);
    ret_status =
        do_operation(csv_file, formulas, operation, arg1, arg2, is_good_arg, i);
  }
  (*formulas).ready[i] = 1;

  return ret_status;
}

Errors get_data(char *str_file, CSVFile_t *csv_file) {
  Errors ret_status = OK;
  Formulas_t formulas = {0};
  char **rows = convert_file_to_rows(str_file, csv_file->rows_num);
  for (int i = 0, j = 0; i < csv_file->rows_num && ret_status == OK;
       i++, j = 0) {
    char *cell = strtok(rows[i], ",");
    if (cell && i != 0) {
      ret_status = check_index_cell(cell);
      if (ret_status == OK)
        ret_status = get_index_cell(ret_status, csv_file, cell, i);

      while (cell && ret_status == OK)
        ret_status = get_data_cell(&cell, &formulas, csv_file, i - 1, &j);
    }
  }
  // в переменной formulas хранятся все ячейки, которые надо будет посчитать, с
  // помощью этого цикла можно посмотреть содержимое той переменной

  // for (int i = 0; i <
  // formulas.formulas_count; i++) {
  //   printf("%d: %d %d %s\n", i, formulas.row_indexes[i],
  //          formulas.col_indexes[i], formulas.formulas[i]);
  // }
  for (int i = 0; i < formulas.formulas_count && ret_status == OK; i++)
    ret_status = calculate_value(&formulas, csv_file, i);

  free_tmp_data(rows, csv_file->rows_num, &formulas);

  return ret_status;
}