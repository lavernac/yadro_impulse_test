#include "csv_read.h"
#include <stdio.h>

int count_lines(char *filename)
{
  FILE *in = fopen(filename,"r"); 
  int ch=0;
  int lines=0;

  if(in != NULL) {
    while((ch = fgetc(in)) != EOF){
        
        if(ch == '\n'){
            lines++;
        }
    }
    fclose(in);
  }
  return lines;
}

void read_csv(char *file_to_read, CSV_file *csv_file) {
    // FILE *file = fopen(file_to_read, "r");
    printf("%d\n", count_lines(file_to_read));
}

void print_csv(CSV_file *csv_file) {
    
}