#pragma once

#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "csv_read.h"
#include "helpers.h"

Suite *test_base();
Suite *test_conversion();
Suite *test_calculating();