#include "csv_read_test.h"

int main() {
  int failed = 0;
  Suite *csv_read_test[] = {test_conversion(), test_calculating(), NULL};

  for (int i = 0; csv_read_test[i] != NULL; i++) {
    SRunner *sr = srunner_create(csv_read_test[i]);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);

    failed += srunner_ntests_failed(sr);
    srunner_free(sr);
  }
  printf("========= FAILED: %d =========\n", failed);

  return failed == 0 ? 0 : 1;
}
