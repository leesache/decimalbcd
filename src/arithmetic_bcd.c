#include "s21_decimal.h"

void s21_long_add(int *num1, int len1, int *num2, int len2, int *result,
                  int *result_len) {
  int carry = 0;
  int max_len = (len1 > len2) ? len1 : len2;
  int i = 0;
  for (i = 0; i < max_len || carry; i++) {
    int sum = carry;
    if (i < len1) sum += num1[len1 - 1 - i];
    if (i < len2) sum += num2[len2 - 1 - i];
    result[max_len - i - 1] = sum % 10;
    carry = sum / 10;
  }
  if (carry) {
    for (int j = max_len; j > 0; j--) {
      result[j] = result[j - 1];
    }
    result[0] = carry;
    *result_len = i + 1;
  } else {
    *result_len = i;
  }
}

void s21_swap(int **a, int **b) {
  int *temp = *a;
  *a = *b;
  *b = temp;
}

void s21_swap_int(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

void s21_long_sub(int *num1, int len1, int *num2, int len2, int *result,
                  int *result_len) {
  if (s21_long_compare(num1, len1, num2, len2) == -1) {
    s21_swap(&num1, &num2);
    s21_swap_int(&len1, &len2);
  }
  int borrow = 0;
  int i = 0;
  for (i = 0; i < len1; i++) {
    int diff = num1[len1 - 1 - i] - borrow;
    if (i < len2) diff -= num2[len2 - 1 - i];
    if (diff < 0) {
      diff += 10;
      borrow = 1;
    } else {
      borrow = 0;
    }
    result[i] = diff;
  }
  *result_len = (i == 0) ? 1 : i;
  for (int j = 0; j < *result_len / 2; j++) {
    int temp = result[j];
    result[j] = result[*result_len - 1 - j];
    result[*result_len - 1 - j] = temp;
  }
}

int s21_long_compare(int *a, int a_len, int *b, int b_len) {
  int count_digits_a = s21_count_significant_digits(a, a_len);
  int count_digits_b = s21_count_significant_digits(b, b_len);
  if (count_digits_a > count_digits_b) return 1;
  if (count_digits_a < count_digits_b) return -1;
  for (int i = 0; i < a_len; i++) {
    if (a[i] > b[i]) return 1;
    if (a[i] < b[i]) return -1;
  }
  return 0;
}

void s21_long_mul(int *a, int a_len, int *b, int b_len, int *result,
                  int *result_len) {
  memset(result, 0, (a_len + b_len) * sizeof(int));
  for (int i = 0; i < a_len; i++) {
    int carry = 0;
    for (int j = 0; j < b_len; j++) {
      int temp = result[i + j] + a[a_len - 1 - i] * b[b_len - 1 - j] + carry;
      carry = temp / 10;
      result[i + j] = temp % 10;
    }
    result[i + b_len] += carry;
  }
  for (int i = 0; i < *result_len / 2; i++) {
    int temp = result[i];
    result[i] = result[*result_len - 1 - i];
    result[*result_len - 1 - i] = temp;
  }
}

void s21_long_div(int *a, int a_len, int *b, int b_len, int *quotient,
                  int *quotient_len) {
  int dividend[a_len];
  memcpy(dividend, a, a_len * sizeof(int));
  int divisor[b_len];
  memcpy(divisor, b, b_len * sizeof(int));
  memset(quotient, 0, a_len * sizeof(int));
  int current_len = b_len;
  while (current_len <= a_len) {
    int temp_result[current_len];
    int temp_len;
    s21_long_sub(dividend, a_len, divisor, b_len, temp_result, &temp_len);
    while (temp_len > 1 && temp_result[temp_len - 1] == 0) {
      temp_len--;
    }
    if (temp_len > b_len || (temp_len == b_len &&
                             temp_result[temp_len - 1] >= divisor[b_len - 1])) {
      memcpy(dividend, temp_result, temp_len * sizeof(int));
      a_len = temp_len;
      quotient[a_len - current_len]++;
    } else {
      break;
    }
    current_len++;
  }
  *quotient_len = a_len;
  while (*quotient_len > 1 && quotient[*quotient_len - 1] == 0) {
    (*quotient_len)--;
  }
}

void s21_remove_unsign_zeroes(int arr[], int *arr_len) {
  while (*arr_len != 29 && arr[0] == 0) {
    s21_shift_left(arr, *arr_len, 1);
    (*arr_len)--;
  }
}

void s21_banking_round(int int_array[], int *len, int scale) {
  int is_bigger = 0;
  int cur_dig = int_array[*len - scale];
  for (int i = *len - scale + 1; i < *len && scale != 0; i++) {
    if (int_array[i] != 0) {
      is_bigger = 1;
    }
  }
  if (scale <= s21_count_significant_digits(int_array, *len)) {
    for (int i = *len - 1; i >= scale; i--) {
      int_array[i] = int_array[i - scale];
    }
    for (int i = 0; i < scale; i++) {
      int_array[i] = 0;
    }
  }

  if (int_array[*len - 1] % 2 == 0) {
    if (cur_dig > 5 || (cur_dig == 5 && is_bigger)) {
      int_array[*len - 1]++;
    }
  } else {
    if (cur_dig >= 5) {
      int_array[*len - 1]++;
    }
  }
  s21_shift_left(int_array, *len, scale);
  (*len) -= scale;
}

int s21_add(s21_decimal dec1, s21_decimal dec2, s21_decimal *result) {
  s21_init_decimal(result);
  char binary_str1[97] = {0}, binary_str2[97] = {0}, bin_str[213] = "";
  char bcd_str1[213] = {0}, bcd_str2[213] = {0};
  int int_array1[70] = {0}, int_array2[70] = {0}, arithmetic_result[70] = {0};
  int result_len1 = 0, result_len2 = 0, arithmetic_result_len = 0;
  int scale1 = GET_SCALE(dec1), scale2 = GET_SCALE(dec2),
      sign1 = GET_SIGN(dec1), sign2 = GET_SIGN(dec2);
  int result_status = S21_OK;
  int MAX_ARRAY[MAX_ARRAY_LEN] = {7, 9, 2, 2, 8, 1, 6, 2, 5, 1, 4, 2, 6, 4, 3,
                                  3, 7, 5, 9, 3, 5, 4, 3, 9, 5, 0, 3, 3, 5};
  s21_decimal_to_binary_string(dec1, binary_str1);
  s21_decimal_to_binary_string(dec2, binary_str2);
  s21_binary_string_to_bcd_string(binary_str1, bcd_str1);
  s21_binary_string_to_bcd_string(binary_str2, bcd_str2);
  s21_bcd_string_to_int_array(bcd_str1, int_array1, &result_len1);
  s21_bcd_string_to_int_array(bcd_str2, int_array2, &result_len2);
  s21_bcd_normalization(int_array1, &result_len1, scale1, int_array2,
                        &result_len2, scale2);
  int result_compare =
      s21_long_compare(int_array1, result_len1, int_array2, result_len2);
  if (sign1 == sign2) {
    s21_long_add(int_array1, result_len1, int_array2, result_len2,
                 arithmetic_result, &arithmetic_result_len);
    SET_SIGN(*result, sign1);
  } else if (sign1 == 0 && sign2 == 1) {
    s21_long_sub(int_array1, result_len1, int_array2, result_len2,
                 arithmetic_result, &arithmetic_result_len);
    if (result_compare == 1 || result_compare == 0)
      SET_SIGN(*result, 0);
    else
      SET_SIGN(*result, 1);
  } else if (sign1 == 1 && sign2 == 0) {
    s21_long_sub(int_array2, result_len2, int_array1, result_len1,
                 arithmetic_result, &arithmetic_result_len);
    if (result_compare == -1 || result_compare == 0)
      SET_SIGN(*result, 0);
    else
      SET_SIGN(*result, 1);
  }
  s21_remove_unsign_zeroes(arithmetic_result, &arithmetic_result_len);
  if (arithmetic_result_len > 29)
    s21_banking_round(arithmetic_result, &arithmetic_result_len,
                      scale1 > scale2 ? scale1 : scale2);
  else
    SET_SCALE(*result, scale1 > scale2 ? scale1 : scale2);
  s21_int_array_to_bcd_string(arithmetic_result, arithmetic_result_len,
                              bcd_str1);
  s21_bcd_string_to_binary_string(bcd_str1, bin_str);
  s21_binary_string_to_decimal(bin_str, result);
  if (s21_long_compare(arithmetic_result, arithmetic_result_len, MAX_ARRAY,
                       MAX_ARRAY_LEN) == 1 &&
      GET_SIGN(*result) == 0) {
    result_status = S21_PLUS_INFINITY;
    s21_init_decimal(result);
  } else if (s21_long_compare(arithmetic_result, arithmetic_result_len,
                              MAX_ARRAY, MAX_ARRAY_LEN) == 1 &&
             GET_SIGN(*result) == 1) {
    result_status = S21_NEGATIVE_INFINITY;
    s21_init_decimal(result);
  }
  return result_status;
}

void s21_shift_left(int *array, int size, int shift_count) {
  if (size <= 1 || shift_count <= 0) {
    return;
  }
  shift_count = shift_count % size;
  for (int i = 0; i < size - shift_count; i++) {
    array[i] = array[i + shift_count];
  }
  for (int i = size - shift_count; i < size; i++) {
    array[i] = 0;
  }
}

void s21_bcd_normalization(int int_array1[], int *result_len1, int scale1,
                           int int_array2[], int *result_len2, int scale2) {
  int index = 29;
  if (scale1 > scale2) {
    if (int_array2[0] != 0) {
      for (int i = 0; i < scale1 - scale2; i++) {
        int_array2[index++] = 0;
        (*result_len2)++;
      }
    } else
      s21_shift_left(int_array2, *result_len2, scale1 - scale2);
  } else if (scale2 > scale1) {
    if (int_array1[0] != 0) {
      for (int i = 0; i < scale2 - scale1; i++) {
        int_array1[index++] = 0;
        (*result_len1)++;
      }
    } else
      s21_shift_left(int_array1, *result_len1, scale2 - scale1);
  }
}

int s21_sub(s21_decimal dec1, s21_decimal dec2, s21_decimal *result) {
  s21_init_decimal(result);
  char binary_str1[97] = {0}, binary_str2[97] = {0}, bin_str[213] = "";
  char bcd_str1[213] = {0}, bcd_str2[213] = {0};
  int int_array1[70] = {0}, int_array2[70] = {0}, arithmetic_result[70] = {0};
  int result_len1 = 0, result_len2 = 0, arithmetic_result_len = 0;
  int scale1 = GET_SCALE(dec1), scale2 = GET_SCALE(dec2),
      sign1 = GET_SIGN(dec1), sign2 = GET_SIGN(dec2);
  int result_status = S21_OK;
  int MAX_ARRAY[MAX_ARRAY_LEN] = {7, 9, 2, 2, 8, 1, 6, 2, 5, 1, 4, 2, 6, 4, 3,
                                  3, 7, 5, 9, 3, 5, 4, 3, 9, 5, 0, 3, 3, 5};
  s21_decimal_to_binary_string(dec1, binary_str1);
  s21_decimal_to_binary_string(dec2, binary_str2);
  s21_binary_string_to_bcd_string(binary_str1, bcd_str1);
  s21_binary_string_to_bcd_string(binary_str2, bcd_str2);
  s21_bcd_string_to_int_array(bcd_str1, int_array1, &result_len1);
  s21_bcd_string_to_int_array(bcd_str2, int_array2, &result_len2);
  s21_bcd_normalization(int_array1, &result_len1, scale1, int_array2,
                        &result_len2, scale2);
  int result_compare =
      s21_long_compare(int_array1, result_len1, int_array2, result_len2);
  if (sign1 == 0 && sign2 == 0) {
    s21_long_sub(int_array1, result_len1, int_array2, result_len2,
                 arithmetic_result, &arithmetic_result_len);
    if (result_compare == 1 || result_compare == 0)
      SET_SIGN(*result, 0);
    else
      SET_SIGN(*result, 1);
  } else if (sign1 == 0 && sign2 == 1) {
    s21_long_add(int_array1, result_len1, int_array2, result_len2,
                 arithmetic_result, &arithmetic_result_len);
    SET_SIGN(*result, 0);
  } else if (sign1 == 1 && sign2 == 0) {
    s21_long_add(int_array2, result_len2, int_array1, result_len1,
                 arithmetic_result, &arithmetic_result_len);
    SET_SIGN(*result, 1);
  } else if (sign1 == 1 && sign2 == 1) {
    s21_long_sub(int_array2, result_len2, int_array1, result_len1,
                 arithmetic_result, &arithmetic_result_len);
    if (result_compare == 1)
      SET_SIGN(*result, 1);
    else
      SET_SIGN(*result, 0);
  }
  s21_remove_unsign_zeroes(arithmetic_result, &arithmetic_result_len);
  if (arithmetic_result_len > 29)
    s21_banking_round(arithmetic_result, &arithmetic_result_len,
                      scale1 > scale2 ? scale1 : scale2);
  else
    SET_SCALE(*result, scale1 > scale2 ? scale1 : scale2);
  s21_int_array_to_bcd_string(arithmetic_result, arithmetic_result_len,
                              bcd_str1);
  s21_bcd_string_to_binary_string(bcd_str1, bin_str);
  s21_binary_string_to_decimal(bin_str, result);
  if (s21_long_compare(arithmetic_result, arithmetic_result_len, MAX_ARRAY,
                       MAX_ARRAY_LEN) == 1 &&
      GET_SIGN(*result) == 0) {
    result_status = S21_PLUS_INFINITY;
    s21_init_decimal(result);
  } else if (s21_long_compare(arithmetic_result, arithmetic_result_len,
                              MAX_ARRAY, MAX_ARRAY_LEN) == 1 &&
             GET_SIGN(*result) == 1) {
    result_status = S21_NEGATIVE_INFINITY;
    s21_init_decimal(result);
  }
  return result_status;
}

void s21_shift_array_left(int arr[], int size) {
  for (int i = size - 1; i > 0; i--) {
    arr[i] = arr[i - 1];
  }
  arr[0] = 0;
}

int s21_mul(s21_decimal dec1, s21_decimal dec2, s21_decimal *result) {
  s21_init_decimal(result);
  char binary_str1[97] = {0}, binary_str2[97] = {0}, bin_str[213] = "";
  char bcd_str1[213] = {0}, bcd_str2[213] = {0};
  int int_array1[70] = {0}, int_array2[70] = {0}, arithmetic_result[70] = {0};
  int result_len1 = 0, result_len2 = 0, arithmetic_result_len = 69;
  int scale1 = GET_SCALE(dec1), scale2 = GET_SCALE(dec2),
      sign1 = GET_SIGN(dec1), sign2 = GET_SIGN(dec2);
  int result_status = S21_OK;
  int MAX_ARRAY[MAX_ARRAY_LEN] = {7, 9, 2, 2, 8, 1, 6, 2, 5, 1, 4, 2, 6, 4, 3,
                                  3, 7, 5, 9, 3, 5, 4, 3, 9, 5, 0, 3, 3, 5};
  int result_scale = scale1 + scale2;
  s21_decimal_to_binary_string(dec1, binary_str1);
  s21_decimal_to_binary_string(dec2, binary_str2);
  s21_binary_string_to_bcd_string(binary_str1, bcd_str1);
  s21_binary_string_to_bcd_string(binary_str2, bcd_str2);
  s21_bcd_string_to_int_array(bcd_str1, int_array1, &result_len1);
  s21_bcd_string_to_int_array(bcd_str2, int_array2, &result_len2);
  s21_long_mul(int_array1, result_len1, int_array2, result_len2,
               arithmetic_result, &arithmetic_result_len);
  for (int i = arithmetic_result_len - 1;
       result_scale != 0 && arithmetic_result[i] == 0; i--) {
    i++;
    result_scale--;
    s21_shift_array_left(arithmetic_result, arithmetic_result_len);
  }
  s21_remove_unsign_zeroes(arithmetic_result, &arithmetic_result_len);
  if (arithmetic_result_len > 29)
    s21_banking_round(arithmetic_result, &arithmetic_result_len,
                      scale1 > scale2 ? scale1 : scale2);
  else
    SET_SCALE(*result, result_scale);
  s21_int_array_to_bcd_string(arithmetic_result, arithmetic_result_len,
                              bcd_str1);
  s21_bcd_string_to_binary_string(bcd_str1, bin_str);
  s21_binary_string_to_decimal(bin_str, result);
  if ((sign1 != sign2)) {
    SET_SIGN(*result, 1);
  } else {
    SET_SIGN(*result, 0);
  }
  if (s21_long_compare(arithmetic_result, arithmetic_result_len, MAX_ARRAY,
                       MAX_ARRAY_LEN) == 1 &&
      GET_SIGN(*result) == 0) {
    result_status = S21_PLUS_INFINITY;
    s21_init_decimal(result);
  } else if (s21_long_compare(arithmetic_result, arithmetic_result_len,
                              MAX_ARRAY, MAX_ARRAY_LEN) == 1 &&
             GET_SIGN(*result) == 1) {
    result_status = S21_NEGATIVE_INFINITY;
    s21_init_decimal(result);
  }
  return result_status;
}

int s21_is_dec_null(s21_decimal dec) {
  int result = 1;
  for (int i = 0; i < 3; i++) {
    if (dec.bits[i] != 0) result = 0;
  }
  return result;
}

int s21_div(s21_decimal dec1, s21_decimal dec2, s21_decimal *result) {
  s21_init_decimal(result);
  if (s21_is_dec_null(dec2)) {
    return S21_DIVISION_BY_ZERO;
  }
  char binary_str1[97] = {0}, binary_str2[97] = {0}, bin_str[213] = "";
  char bcd_str1[213] = {0}, bcd_str2[213] = {0};
  int int_array1[70] = {0}, int_array2[70] = {0}, arithmetic_result[70] = {0};
  int result_len1 = 0, result_len2 = 0, arithmetic_result_len = 0;
  int sign1 = GET_SIGN(dec1), sign2 = GET_SIGN(dec2);
  int result_status = S21_OK;
  int MAX_ARRAY[MAX_ARRAY_LEN] = {7, 9, 2, 2, 8, 1, 6, 2, 5, 1, 4, 2, 6, 4, 3,
                                  3, 7, 5, 9, 3, 5, 4, 3, 9, 5, 0, 3, 3, 5};
  int result_scale = 0;
  s21_decimal_to_binary_string(dec1, binary_str1);
  s21_decimal_to_binary_string(dec2, binary_str2);
  s21_binary_string_to_bcd_string(binary_str1, bcd_str1);
  s21_binary_string_to_bcd_string(binary_str2, bcd_str2);
  s21_bcd_string_to_int_array(bcd_str1, int_array1, &result_len1);
  s21_bcd_string_to_int_array(bcd_str2, int_array2, &result_len2);
  s21_long_div(int_array1, result_len1, int_array2, result_len2,
               arithmetic_result, &arithmetic_result_len);
  s21_int_array_to_bcd_string(arithmetic_result, arithmetic_result_len,
                              bcd_str1);
  s21_bcd_string_to_binary_string(bcd_str1, bin_str);
  s21_binary_string_to_decimal(bin_str, result);
  if ((sign1 != sign2)) {
    SET_SIGN(*result, 1);
  } else {
    SET_SIGN(*result, 0);
  }
  SET_SCALE(*result, result_scale);
  if (s21_long_compare(arithmetic_result, arithmetic_result_len, MAX_ARRAY,
                       MAX_ARRAY_LEN) == 1 &&
      GET_SIGN(*result) == 0) {
    result_status = S21_PLUS_INFINITY;
    s21_init_decimal(result);
  } else if (s21_long_compare(arithmetic_result, arithmetic_result_len,
                              MAX_ARRAY, MAX_ARRAY_LEN) == 1 &&
             GET_SIGN(*result) == 1) {
    result_status = S21_NEGATIVE_INFINITY;
    s21_init_decimal(result);
  }
  return result_status;
}