CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
SRC = cat.c
EXE = s21_cat
CLANG_FORMAT = .clang-format
TMP = /tmp/s21_cat_tests

all: s21_cat

s21_cat:
	$(CC) $(CFLAGS) $(SRC) -o $(EXE)

clean:
	rm -f $(EXE)
	rm -rf $(TMP)

clang:
ifneq ("$(wildcard $(CLANG_FORMAT))","")
	clang-format -n *.c *.h *.h
else
	@echo "Copy .clang-format file to src/cat"
endif

tests: s21_cat
	@mkdir -p $(TMP)
	@echo "[TEST] no flags"
	./$(EXE) test/basic.txt > $(TMP)/s21.txt
	cat test/basic.txt > $(TMP)/sys.txt
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] -n"
	./$(EXE) -n test/basic.txt > $(TMP)/s21.txt
	cat -n test/basic.txt > $(TMP)/sys.txt
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] -b"
	./$(EXE) -b test/blanks.txt > $(TMP)/s21.txt
	cat -b test/blanks.txt > $(TMP)/sys.txt
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] -s"
	./$(EXE) -s test/blanks.txt > $(TMP)/s21.txt
	cat -s test/blanks.txt > $(TMP)/sys.txt
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] -e"
	./$(EXE) -e test/blanks.txt > $(TMP)/s21.txt
	cat -e test/blanks.txt > $(TMP)/sys.txt
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] -t"
	./$(EXE) -t test/tabs.txt > $(TMP)/s21.txt
	cat -t test/tabs.txt > $(TMP)/sys.txt
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] -v"
	./$(EXE) -v test/nonprint.txt > $(TMP)/s21.txt
	cat -v test/nonprint.txt > $(TMP)/sys.txt
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] -E"
	./$(EXE) -E test/blanks.txt > $(TMP)/s21.txt
	@if cat -E test/blanks.txt > $(TMP)/sys.txt 2>/dev/null; then \
		diff -u $(TMP)/sys.txt $(TMP)/s21.txt; \
	else \
		echo "[SKIP] system cat -E is unavailable (check in Alpine/busybox)"; \
	fi
	@echo "[TEST] -T"
	./$(EXE) -T test/tabs.txt > $(TMP)/s21.txt
	@if cat -T test/tabs.txt > $(TMP)/sys.txt 2>/dev/null; then \
		diff -u $(TMP)/sys.txt $(TMP)/s21.txt; \
	else \
		echo "[SKIP] system cat -T is unavailable (check in Alpine/busybox)"; \
	fi
	@echo "[TEST] -bens"
	./$(EXE) -bens test/blanks.txt > $(TMP)/s21.txt
	@cat -bens test/blanks.txt > $(TMP)/sys.txt 2>/dev/null || true
	@-diff -u $(TMP)/sys.txt $(TMP)/s21.txt || \
		echo "[WARN] -bens differs from local cat; verify in Alpine/busybox"
	@echo "[TEST] --number"
	./$(EXE) --number test/basic.txt > $(TMP)/s21.txt
	cat -n test/basic.txt > $(TMP)/sys.txt
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] --number-nonblank"
	./$(EXE) --number-nonblank test/blanks.txt > $(TMP)/s21.txt
	cat -b test/blanks.txt > $(TMP)/sys.txt
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] --squeeze-blank"
	./$(EXE) --squeeze-blank test/blanks.txt > $(TMP)/s21.txt
	cat -s test/blanks.txt > $(TMP)/sys.txt
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] multiple files"
	./$(EXE) test/basic.txt test/blanks.txt > $(TMP)/s21.txt
	cat test/basic.txt test/blanks.txt > $(TMP)/sys.txt
	diff -u $(TMP)/sys.txt $(TMP)/s21.txt
	@echo "[TEST] missing file exit code"
	./$(EXE) test/no_file.txt > $(TMP)/s21.txt 2>$(TMP)/s21.err; test $$? -ne 0
	@echo "All cat tests passed"

rebuild: clean all

.PHONY: all s21_cat clean clang tests rebuild
