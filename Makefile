TARGET=csv_read.a
CC=gcc
DFLAGS=-g3
CFLAGS=-Wall -Wextra -std=c11 -I csv_read_include # -Werror
OS=$(shell uname)
TEST_TARGET=test
EXECUTABLE=csv_read

TEST_DIR=tests
SRC_DIR=csv_read_lib
BUILD_DIR=obj
BIN_DIR=bin

MODULES=$(shell find "$(SRC_DIR)" -name "*.c")

OBJECTS=$(MODULES:%.c=%.o)
MODULES_TEST=$(wildcard $(TEST_DIR)/*.c)
OBJECTS_TEST=$(MODULES_TEST:%.c=%.o)
MODULES_RUN = $(wildcard $(SRC_DIR)/*.c) main.c
OBJECTS_RUN = $(MODULES_RUN:%.c=%.o)

IS_UBUNTU = $(shell grep -i 'ubuntu' /etc/os-release 2>/dev/null)
IS_DEBIAN = $(shell grep -i 'debian' /etc/os-release 2>/dev/null)

OS = $(shell uname)
ifeq ($(OS), Darwin)
	LDLIBS = -lcheck -lm -lpthread
else
	LDLIBS = -lcheck -lm -lsubunit -lpthread  -lrt 
endif

.PHONY: all run build rebuild clean test gcov_report valgrind_check $(TARGET)

all: rebuild # test gcov_report

debug: create_dir
	@$(CC) $(CFLAGS) $(DFLAGS) $(MODULES_RUN) -lm -o $(BIN_DIR)/$@

debug_test: create_dir
	@$(CC)  $(DFLAGS) $(MODULES) $(MODULES_TEST) $(LDLIBS) -L. -lm -o $(BIN_DIR)/$@

build: $(TARGET)

rebuild: clean $(TARGET)

$(TARGET): create_dir $(OBJECTS) style
	@ar rcs $(BUILD_DIR)/$(TARGET) $(shell find "$(BUILD_DIR)/$(SRC_DIR)" -name "*.o" -not -name "main.o")

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $(BUILD_DIR)/$@ $^

run: create_dir_run $(TARGET)
	@$(CC) $(CFLAGS) main.c $(BUILD_DIR)/$(TARGET)  -o $(BIN_DIR)/$(EXECUTABLE) -lm
	@./$(BIN_DIR)/$(EXECUTABLE) test_files/recursive.csv

test: $(TARGET) $(OBJECTS_TEST)
	@$(CC) $(CFLAGS) $(wildcard $(BUILD_DIR)/$(TEST_DIR)/*.o) $(BUILD_DIR)/$(TARGET) $(LDLIBS) -L. -o $(BIN_DIR)/$(TEST_TARGET)
	@./$(BIN_DIR)/$(TEST_TARGET)
	
gcov_report: $(TARGET)
	@$(CC) --coverage $(CFLAGS) $(wildcard $(TEST_DIR)/*.c) $(LDLIBS) $(MODULES) -o $(BIN_DIR)/$(TEST_TARGET)_gcov
	@./$(BIN_DIR)/$(TEST_TARGET)_gcov
	@lcov -t "test_coverage" -o test_coverage.info -c -d .
	@genhtml -o report test_coverage.info
	@open report/index.html
	@rm -rf */*.gcda */*.gcno report/test_coverage.info *.info

format_style:
	@cp ../materials/linters/.clang-format .
	@clang-format -n */*.c */*.h
	@clang-format -i */*.c */*.h
	@ # rm ./.clang-format

valgrind_check: test
	@CK_FORK=no valgrind --vgdb=no --leak-check=full \
	--show-leak-kinds=all --track-origins=yes --log-file="valgrind.log" -v --verbose -q --quiet -s ./$(BIN_DIR)/$(TEST_TARGET)

run_and_valgrind: run
	valgrind --track-origins=yes --leak-check=full --log-file="valgrind.log" --show-leak-kinds=all ./$(BIN_DIR)/$(EXECUTABLE) test_files/simple_recursive.csv

clean:
	@echo "Deleting unnecessary files..."
	@rm -rf obj bin report *.a *.log 
	@rm -rf **/*.dSYM **/*.dSYM **/*.log **/*.log test_coverage.info

# Установка check.h

install_check_arch:
	@sudo pacman -S check

install_check_debian:
	@sudo apt install check

create_dir:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)
	@mkdir -p $(BUILD_DIR)/$(TEST_DIR)
	@mkdir -p $(BIN_DIR)

create_dir_run:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)
	@mkdir -p $(BIN_DIR)

style:
	@clang-format -i -style=Google $(SRC_DIR)/*.c main.c

cppcheck:
	@cppcheck --enable=all --suppress=missingIncludeSystem */*.c */*.h