#include "csv_read_test.h"

START_TEST(check_index_cell_test) {
  char cell[] = "30";

  ck_assert_int_eq(check_index_cell(cell), OK);
}
END_TEST

START_TEST(check_index_cell_error_test) {
  char cell[] = "30.2";

  ck_assert_int_eq(check_index_cell(cell), INCORRECT_INDEX_OF_ROW);
}
END_TEST

START_TEST(get_data_test) {
  CSVFile_t csv_file = {0};
  char *str_file = NULL, *str_file_cpy = NULL;

  file_to_str(&str_file, "test_files/correct/formula_end.csv", &str_file_cpy);
  count_lines(str_file, &csv_file);
  get_header(str_file, &csv_file);
  init_csv(&csv_file);
  ck_assert_int_eq(get_data(str_file_cpy, &csv_file), OK);

  int indexes_check[] = {1, 2, 30, 0};
  for (int i = 0; i < csv_file.rows_num - 1; i++)
    ck_assert_int_eq(csv_file.rows_nums[i], indexes_check[i]);

  double data_check[3][3] = {{1, 3, 5}, {2, 4, 6}, {7, 8, 2}};
  for (int i = 0; i < csv_file.rows_num - 1; i++)
    for (int j = 0; j < csv_file.cols_num; j++)
      ck_assert_int_eq(fabs(csv_file.data[i][j] - data_check[i][j]) < 1e-6, 1);

  free(str_file);
  free(str_file_cpy);
  free_csv(&csv_file);
}
END_TEST

START_TEST(get_data_deep_recursive_test) {
  CSVFile_t csv_file = {0};
  char *str_file = NULL, *str_file_cpy = NULL;

  file_to_str(&str_file, "test_files/correct/recursive.csv", &str_file_cpy);
  count_lines(str_file, &csv_file);
  get_header(str_file, &csv_file);
  init_csv(&csv_file);
  ck_assert_int_eq(get_data(str_file_cpy, &csv_file), OK);

  int indexes_check[] = {1, 2, 30, 0};
  for (int i = 0; i < csv_file.rows_num - 1; i++)
    ck_assert_int_eq(csv_file.rows_nums[i], indexes_check[i]);

  double data_check[3][3] = {{-3, 3, 2}, {2, 4, 6}, {7, 8, -5}};
  for (int i = 0; i < csv_file.rows_num - 1; i++)
    for (int j = 0; j < csv_file.cols_num; j++)
      ck_assert_int_eq(fabs(csv_file.data[i][j] - data_check[i][j]) < 1e-6, 1);

  free(str_file);
  free(str_file_cpy);
  free_csv(&csv_file);
}
END_TEST

START_TEST(read_csv_test) {
  CSVFile_t csv_file = {0};
  char *str_file = NULL, *str_file_cpy = NULL;

  ck_assert_int_eq(read_csv("test_files/correct/recursive.csv", &csv_file), OK);

  int indexes_check[] = {1, 2, 30, 0};
  for (int i = 0; i < csv_file.rows_num - 1; i++)
    ck_assert_int_eq(csv_file.rows_nums[i], indexes_check[i]);

  double data_check[3][3] = {{-3, 3, 2}, {2, 4, 6}, {7, 8, -5}};
  for (int i = 0; i < csv_file.rows_num - 1; i++)
    for (int j = 0; j < csv_file.cols_num; j++)
      ck_assert_int_eq(fabs(csv_file.data[i][j] - data_check[i][j]) < 1e-6, 1);

  free(str_file);
  free(str_file_cpy);
  free_csv(&csv_file);
}
END_TEST

START_TEST(div_zero_test) {
  CSVFile_t csv_file = {0};
  char *str_file = NULL, *str_file_cpy = NULL;

  ck_assert_int_eq(read_csv("test_files/bad/div.csv", &csv_file),
                   DIV_ZERO_ERROR);

  free(str_file);
  free(str_file_cpy);
  free_csv(&csv_file);
}
END_TEST

START_TEST(unknown_operation_test) {
  CSVFile_t csv_file = {0};
  char *str_file = NULL, *str_file_cpy = NULL;

  ck_assert_int_eq(read_csv("test_files/bad/operation.csv", &csv_file),
                   UNKNOWN_OPERATION_ERROR);

  free(str_file);
  free(str_file_cpy);
  free_csv(&csv_file);
}
END_TEST

START_TEST(bad_cell_test) {
  CSVFile_t csv_file = {0};
  char *str_file = NULL, *str_file_cpy = NULL;

  ck_assert_int_eq(read_csv("test_files/bad/cell.csv", &csv_file),
                   INCORRECT_CELL_ERROR);

  free(str_file);
  free(str_file_cpy);
  free_csv(&csv_file);
}
END_TEST

START_TEST(bad_arg_test) {
  CSVFile_t csv_file = {0};
  char *str_file = NULL, *str_file_cpy = NULL;

  ck_assert_int_eq(read_csv("test_files/bad/arg.csv", &csv_file),
                   BAD_ARG_ERROR);

  free(str_file);
  free(str_file_cpy);
  free_csv(&csv_file);
}
END_TEST

Suite *test_calculating(void) {
  Suite *s1 = suite_create("\033[45m CALCULATING \033[0m");
  TCase *tc1_1 = tcase_create("operations_tc");

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, check_index_cell_test);
  tcase_add_test(tc1_1, check_index_cell_error_test);
  tcase_add_test(tc1_1, get_data_test);
  tcase_add_test(tc1_1, get_data_deep_recursive_test);
  tcase_add_test(tc1_1, read_csv_test);
  tcase_add_test(tc1_1, div_zero_test);
  tcase_add_test(tc1_1, unknown_operation_test);
  tcase_add_test(tc1_1, bad_arg_test);
  tcase_add_test(tc1_1, bad_cell_test);

  suite_add_tcase(s1, tc1_1);
  return s1;
}
