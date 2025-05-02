#include <stdio.h>
#include "csv_read.h"

int main(int argc, char *argv[]) {
    CSV_file csv = {0};
    if (argc == 2) {
        read_csv(argv[1], &csv);
        print_csv(&csv);
    }
    else
        printf("Wrong num of arguments\n");
}