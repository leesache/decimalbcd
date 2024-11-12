#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper macros to extract parts of the decimal
#define GET_SIGN(dec) ((dec).bits[3] >> 31 & 1)
#define GET_SCALE(dec) ((dec).bits[3] >> 16 & 0xFF)
#define GET_SCALE_BIG_DECIMAL(big_dec) ((big_dec).bits[7] >> 16 & 0xFF)
#define SET_SIGN(dec, sign) \
  ((dec).bits[3] = ((dec).bits[3] & ~(1 << 31)) | (sign << 31))
#define SET_SIGN_BIG(big_dec, sign) \
  ((big_dec).bits[7] = (big_dec).bits[7] & ~(1 << 31) | (sign << 31))
#define SET_SCALE(dec, exp) \
  ((dec).bits[3] = ((dec).bits[3] & ~(0xFF << 16)) | ((exp & 0xFF) << 16))
#define SET_BIG_DECIMAL_SCALE(big_dec, exp) \
  ((big_dec).bits[7] = (big_dec).bits[7] & ~(0xFF << 16) | (exp << 16))

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define MAX_LENGTH 29
#define MAX_NUM_FLOAT_CONVERTER 79228162514264337593543950335.0f
#define MIN_NUM_FLOAT_CONVERTER 1e-28f
#define MAX_ARRAY_LEN 29

typedef struct {
  int bits[4];
} s21_decimal;

// Error codes
#define S21_OK 0
#define S21_PLUS_INFINITY 1
#define S21_NEGATIVE_INFINITY 2
#define S21_DIVISION_BY_ZERO 3
#define S21_CONVERSION_ERROR 1
#define S21_CALCULATION_ERROR 1

#define MAX_SCALE 28

// Arithmetic operations
int s21_add(s21_decimal dec1, s21_decimal dec2, s21_decimal *result);
int s21_sub(s21_decimal dec1, s21_decimal dec2, s21_decimal *result);
int s21_mul(s21_decimal dec1, s21_decimal dec2, s21_decimal *result);
int s21_div(s21_decimal dec1, s21_decimal dec2, s21_decimal *result);

// Comparison operations
int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

// Converters
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Other functions
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// Utility functions
void s21_init_decimal(s21_decimal *dec);
int s21_get_bit(s21_decimal value, int bit);
void s21_set_bit(s21_decimal *num, int bit, int value);

// Long arithmetic
void s21_long_sub(int *a, int a_len, int *b, int b_len, int *result,
                  int *result_len);
void s21_long_add(int *a, int a_len, int *b, int b_len, int *result,
                  int *result_len);
void s21_long_mul(int *a, int a_len, int *b, int b_len, int *result,
                  int *result_len);
void s21_long_div(int *a, int a_len, int *b, int b_len, int *resutlt,
                  int *quotient_len);

// Long Utility
void s21_shift_array_left(int arr[], int size);
void s21_shift_left(int *array, int size, int shift_count);
int s21_count_significant_digits(int arr[], int n);
void s21_initialize_str(char *str);
int s21_long_compare(int *a, int a_len, int *b, int b_len);
void s21_banking_round(int int_array[], int *len, int scale);
void s21_swap(int **a, int **b);
void s21_swap_int(int *a, int *b);

// BCD functions
void s21_decimal_to_binary_string(s21_decimal decimal, char *str);
void s21_binary_string_to_decimal(char *str, s21_decimal *decimal);
void s21_binary_string_shift_left(char *str);
void s21_binary_string_shift_right(char *str);
int s21_bits_to_int(char *bits);
void s21_int_to_bits(int value, char *bits);
void s21_add_or_sub_3_to_bcd_bits(char *str, int index, char is_add);
int s21_bcd_to_digit(char *bcd);
int s21_is_bigger_five(char *str, int index);
void s21_binary_string_to_bcd_string(char *src, char *res);
void s21_bcd_string_to_binary_string(char *src, char *res);
void s21_bcd_string_to_int_array(const char *bcd_string, int *result,
                                 int *result_len);
void s21_bcd_normalization(int int_array1[], int *result_len1, int scale1,
                           int int_array2[], int *result_len2, int scale2);
void s21_int_array_to_bcd_string(int int_array[], int array_len,
                                 char bcd_str[]);

#endif  // S21_DECIMAL_H