#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  if (!result) {
    return S21_CALCULATION_ERROR;
  }
  SET_SIGN(value, ~GET_SIGN(value));
  for (int i = 0; i < 4; i++) {
    result->bits[i] = value.bits[i];
  }
  return S21_OK;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (!result) {
    return S21_CALCULATION_ERROR;
  }
  s21_init_decimal(result);
  char binary_str[97] = {0}, bin_str[213] = "";
  char bcd_str[213] = {0};
  int int_array[70] = {0};
  int len = 0;
  int scale = GET_SCALE(value), sign = GET_SIGN(value);
  s21_decimal_to_binary_string(value, binary_str);
  s21_binary_string_to_bcd_string(binary_str, bcd_str);
  s21_bcd_string_to_int_array(bcd_str, int_array, &len);
  if (scale <= s21_count_significant_digits(int_array, len)) {
    for (int i = len - 1; i >= scale; i--) {
      int_array[i] = int_array[i - scale];
    }
    for (int i = 0; i < scale; i++) {
      int_array[i] = 0;
    }
  }
  s21_int_array_to_bcd_string(int_array, len, bcd_str);
  s21_bcd_string_to_binary_string(bcd_str, bin_str);
  s21_binary_string_to_decimal(bin_str, result);
  SET_SCALE(*result, 0);
  SET_SIGN(*result, sign);
  return S21_OK;
}

int s21_count_significant_digits(int arr[], int n) {
  int count = 0;
  int significant_start = 0;
  while (significant_start < n && arr[significant_start] == 0)
    significant_start++;
  for (int i = significant_start; i < n; i++) count++;
  return count;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (!result) {
    return S21_CALCULATION_ERROR;
  }
  s21_truncate(value, result);
  s21_decimal one = {{1, 0, 0, 0}};
  if (GET_SIGN(*result) == 1) {
    s21_sub(*result, one, result);
  }
  return S21_OK;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  if (!result) {
    return S21_CALCULATION_ERROR;
  }
  s21_decimal one = {{1, 0, 0, 0}}, ten = {{10, 0, 0, 0}};
  s21_decimal res_trunc = {{0, 0, 0, 0}}, truncated_dec = {{0, 0, 0, 0}},
              res_mul = {{0, 0, 0, 0}}, res_sub = {{0, 0, 0, 0}};
  s21_truncate(value, &truncated_dec);
  s21_sub(value, truncated_dec, &res_sub);
  s21_mul(res_sub, ten, &res_mul);
  s21_truncate(res_mul, &res_trunc);
  if (res_trunc.bits[0] >= 5) {
    if (GET_SIGN(truncated_dec) == 0)
      s21_add(truncated_dec, one, result);
    else
      s21_sub(truncated_dec, one, result);
  } else {
    *result = truncated_dec;
  }
  SET_SCALE(*result, 0);
  return S21_OK;
}