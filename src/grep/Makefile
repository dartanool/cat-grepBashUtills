CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
SRC = s21_grep.c
EXE = s21_grep
CLANG_FORMAT = .clang-format
TMP = /tmp/s21_grep_tests
INPUT = test/input_1.txt

GREP_OK = || test $$? -eq 1

all: s21_grep

s21_grep:
	$(CC) $(CFLAGS) $(SRC) -o $(EXE)

clean:
	rm -f $(EXE)
	rm -rf $(TMP)

clang:
ifneq ("$(wildcard $(CLANG_FORMAT))","")
	clang-format -n *.c *.h
else
	@echo "Copy .clang-format file to src/grep"
endif

tests: s21_grep
	@mkdir -p $(TMP)
	@echo "[TEST] -e"
	./$(EXE) -e Hello $(INPUT) > $(TMP)/s21.txt
	grep -e Hello $(INPUT) > $(TMP)/sys.txt $(GREP_OK)
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] -i"
	./$(EXE) -i -e HELLO $(INPUT) > $(TMP)/s21.txt
	grep -i -e HELLO $(INPUT) > $(TMP)/sys.txt $(GREP_OK)
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] -v"
	./$(EXE) -v world $(INPUT) > $(TMP)/s21.txt
	grep -v world $(INPUT) > $(TMP)/sys.txt $(GREP_OK)
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] -c"
	./$(EXE) -c -e world $(INPUT) > $(TMP)/s21.txt
	grep -c -e world $(INPUT) > $(TMP)/sys.txt $(GREP_OK)
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] -l"
	./$(EXE) -l -e world $(INPUT) > $(TMP)/s21.txt
	grep -l -e world $(INPUT) > $(TMP)/sys.txt $(GREP_OK)
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] -n"
	./$(EXE) -n -e Hello $(INPUT) > $(TMP)/s21.txt
	grep -n -e Hello $(INPUT) > $(TMP)/sys.txt $(GREP_OK)
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] multiple -e"
	./$(EXE) -n -e Hello -e world $(INPUT) > $(TMP)/s21.txt
	grep -n -e Hello -e world $(INPUT) > $(TMP)/sys.txt $(GREP_OK)
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] -f"
	./$(EXE) -f test/patterns_1.txt $(INPUT) > $(TMP)/s21.txt
	grep -f test/patterns_1.txt $(INPUT) > $(TMP)/sys.txt $(GREP_OK)
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] -f patterns_eof"
	./$(EXE) -f test/patterns_eof.txt $(INPUT) > $(TMP)/s21.txt
	grep -f test/patterns_eof.txt $(INPUT) > $(TMP)/sys.txt $(GREP_OK)
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] -o"
	./$(EXE) -o -e Hello $(INPUT) > $(TMP)/s21.txt
	grep -o -e Hello $(INPUT) > $(TMP)/sys.txt $(GREP_OK)
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] -iv"
	./$(EXE) -iv HELLO $(INPUT) > $(TMP)/s21.txt
	grep -iv HELLO $(INPUT) > $(TMP)/sys.txt $(GREP_OK)
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] missing file exit code"
	./$(EXE) -e Hello test/no_file.txt > $(TMP)/s21.txt 2>$(TMP)/s21.err; test $$? -ne 0
	@echo "All grep tests passed"

rebuild: clean all

.PHONY: all s21_grep clean clang tests rebuild
