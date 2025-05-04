#include "csv_read.h"

int main(int argc, char *argv[]) {
  CSVFile_t csv = {0};
  if (argc == 2) {
    if (read_csv(argv[1], &csv) == OK) print_csv(&csv);
    free_csv(&csv);
  } else
    printf("Wrong num of arguments\n");
}