#include "csv_read_test.h"

START_TEST(count_lines_test) {
  CSVFile_t csv_file = {0};
  char str[] = ",a\n1,1\n2,2\n3,3\n";
  count_lines(str, &csv_file);

  ck_assert_int_eq(csv_file.rows_num, 4);
}
END_TEST

START_TEST(file_to_str_test) {
  char filename[] = "test_files/correct/just_nums.csv", *str_file = NULL,
       *str_file_cpy = NULL;

  ck_assert_int_eq(file_to_str(&str_file, filename, &str_file_cpy), OK);

  free(str_file);
  free(str_file_cpy);
}
END_TEST

START_TEST(file_to_str_error_test) {
  char filename[] = "wrong_file", *str_file = NULL, *str_file_cpy = NULL;

  ck_assert_int_eq(file_to_str(&str_file, filename, &str_file_cpy),
                   CANT_OPEN_FILE_ERROR);

  free(str_file);
  free(str_file_cpy);
}
END_TEST

START_TEST(get_columns_name_test) {
  CSVFile_t csv_file = {0};
  char *str_file = NULL, *str_file_cpy = NULL;
  char check[3][5] = {"A", "B", "Cell"};

  file_to_str(&str_file, "test_files/correct/formula_end.csv", &str_file_cpy);
  int headers_num = get_header(str_file, &csv_file);

  ck_assert_int_eq(headers_num, 3);

  for (int i = 0; i < headers_num; i++)
    ck_assert_str_eq(csv_file.headers[i], check[i]);

  free(str_file);
  free(str_file_cpy);
  free_csv(&csv_file);
}
END_TEST

START_TEST(file_to_rows_test) {
  char str_file[13] = "1,1\n2,2\n3,3\n";
  char check[3][4] = {"1,1", "2,2", "3,3"};
  int rows_num = 3;
  char **res = convert_file_to_rows(str_file, rows_num);

  for (int i = 0; i < rows_num; i++) {
    ck_assert_str_eq(res[i], check[i]);
    free(res[i]);
  }

  free(res);
}
END_TEST

START_TEST(init_memory_test) {
  CSVFile_t csv_file = {0};
  char *str_file = NULL, *str_file_cpy = NULL;

  file_to_str(&str_file, "test_files/correct/formula_end.csv", &str_file_cpy);
  count_lines(str_file, &csv_file);
  get_header(str_file, &csv_file);

  ck_assert_int_eq(init_csv(&csv_file), OK);

  free(str_file);
  free(str_file_cpy);
  free_csv(&csv_file);
}
END_TEST

Suite *test_conversion(void) {
  Suite *s1 = suite_create("\033[45m CONVERSION \033[0m");
  TCase *tc1_1 = tcase_create("arithmetics_tc");

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, count_lines_test);
  tcase_add_test(tc1_1, file_to_str_test);
  tcase_add_test(tc1_1, file_to_str_error_test);
  tcase_add_test(tc1_1, get_columns_name_test);
  tcase_add_test(tc1_1, init_memory_test);
  tcase_add_test(tc1_1, file_to_rows_test);

  suite_add_tcase(s1, tc1_1);
  return s1;
}
