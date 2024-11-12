CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11
CHECK_FLAGS = -lcheck -lm -lpthread
#-lsubunit -lrt
LDFLAGS=-L/usr/local/lib
GCOV_FLAGS=-fprofile-arcs -ftest-coverage

LIBRARY_SOURCES = bcd.c \
				  compare.c \
				  converters.c \
				  round.c \
				  arithmetic_bcd.c

LIBRARY_OBJECTS = bcd.o \
				  compare.o \
				  converters.o \
				  round.o \
				  arithmetic_bcd.o

TEST_SOURCES = ../tests/s21_arithmetic_test/s21_add_test.c \
               ../tests/s21_arithmetic_test/s21_sub_test.c \
               ../tests/s21_arithmetic_test/s21_mul_test.c \
               ../tests/s21_arithmetic_test/s21_div_test.c \
               ../tests/s21_comparsion_test/s21_is_equal_test.c \
               ../tests/s21_comparsion_test/s21_is_greater_test.c \
               ../tests/s21_comparsion_test/s21_is_greater_or_equal_test.c \
               ../tests/s21_comparsion_test/s21_is_less_test.c \
               ../tests/s21_comparsion_test/s21_is_less_or_equal_test.c \
               ../tests/s21_comparsion_test/s21_is_not_equal_test.c \
               ../tests/s21_convertor_test/s21_test_from_decimal_to_float.c \
               ../tests/s21_convertor_test/s21_test_from_decimal_to_int.c \
               ../tests/s21_convertor_test/s21_test_from_float_to_decimal.c \
               ../tests/s21_convertor_test/s21_test_from_int_to_decimal.c \
               ../tests/s21_other_test/s21_round_test.c \
               ../tests/s21_other_test/s21_floor_test.c\
               ../tests/s21_other_test/s21_truncate_test.c\
               ../tests/s21_other_test/s21_negate_test.c\
			   ../tests/test.c

all: test gcov_report

compile_objects:
	$(CC) $(CFLAGS) -c ${LIBRARY_SOURCES}

s21_decimal.a: compile_objects
	ar -rc s21_decimal.a ${LIBRARY_OBJECTS}
	#ranlib s21_decimal.a
	rm -f *.o

build: s21_decimal.a

rebuild: clean build

test: rebuild
	$(CC) $(CFLAGS) $(TEST_SOURCES) s21_decimal.a $(CHECK_FLAGS) $(GCOV_FLAGS) -o tests.out
	./tests.out
test_valgrind: test
	valgrind --log-file=valgrind.log --leak-check=full --track-origins=yes ./tests.out

gcov_report:
	$(CC) $(CFLAGS) --coverage $(LIBRARY_SOURCES) $(TEST_SOURCES) -o gcov_test -lcheck -lm -pthread
	chmod +x *
	./gcov_test
	lcov -t "gcov_test" -o gcov_test.info --no-external -c -d .
	genhtml -o report/ gcov_test.info
	open ./report/index.html

clean:
	@rm -rf ../src/*.o ../*/*.out ./*/*.a ../src/*.a
	@rm -rf *.gcda *.gcno *.info gcov_report
style:
	cp ../materials/linters/.clang-format .
	find .. -name "*.c" -exec clang-format -i {} \;
	find .. -name "*.h" -exec clang-format -i {} \;
	rm -rf .clang-format

style_test:
	cp ../materials/linters/.clang-format .
	find .. -name "*.c" -exec clang-format -n {} \;
	find .. -name "*.h" -exec clang-format -n {} \;
	rm -rf .clang-format



