#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (!dst) return S21_CONVERSION_ERROR;
  if (src < 0) {
    dst->bits[0] = ~src + 1;
  } else {
    dst->bits[0] = src;
  }
  dst->bits[1] = 0;
  dst->bits[2] = 0;
  dst->bits[3] = 0;
  if (src < 0) SET_SIGN(*dst, 1);
  return S21_OK;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  if (!dst) return S21_CONVERSION_ERROR;
  int flag = S21_OK;
  if (GET_SCALE(src) > 0) s21_truncate(src, &src);
  if (src.bits[1] == 0 && src.bits[2] == 0) {
    if (s21_get_bit(src, 31) == 0 && s21_get_bit(src, 127) == 0)
      *dst = src.bits[0];
    else if (s21_get_bit(src, 31) == 0 && s21_get_bit(src, 127) == 1)
      *dst = ~src.bits[0] + 1;
    else if (s21_get_bit(src, 127) == 1 && src.bits[0] == INT_MIN)
      *dst = INT_MIN;
  } else
    flag = S21_CONVERSION_ERROR;
  return flag;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (!dst) return S21_CONVERSION_ERROR;
  double result = 0;
  for (int i = 95; i >= 0; i--) {
    double tmp_powers = pow(2.0, i);
    result += tmp_powers * s21_get_bit(src, i);
  }
  int mashtab = GET_SCALE(src);
  double power = pow(10.0, mashtab);
  result = result / power;
  if (s21_get_bit(src, 127))
    *dst = (float)result * -1;
  else
    *dst = (float)result;
  return S21_OK;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (!dst || isinf(src) || isnan(src)) {
    return S21_CONVERSION_ERROR;
  }
  dst->bits[0] = 0;
  dst->bits[1] = 0;
  dst->bits[2] = 0;
  dst->bits[3] = 0;
  int flag = S21_OK;
  if (fabsf(src) > MAX_NUM_FLOAT_CONVERTER ||
      (fabsf(src) < MIN_NUM_FLOAT_CONVERTER && fabsf(src) > 0))
    flag = S21_CONVERSION_ERROR;
  if (src == -0.0) {
    dst->bits[0] = 0;
    dst->bits[1] = 0;
    dst->bits[2] = 0;
    dst->bits[3] = 0x80000000;
    flag = S21_OK;
  }
  if (flag == S21_OK && src != -0.0) {
    int sign = 0;
    char buffer[25];
    sprintf(buffer, "%.6E", src);
    int integer_part = atoi(strtok(buffer, "."));
    if (integer_part < 0) {
      integer_part *= -1;
      sign = 1;
    }
    int fractional_part = atoi(strtok(buffer + 2 + sign, "E"));
    int exponent = atoi(strtok(NULL, "E"));
    integer_part = integer_part * 1000000 + fractional_part;
    if (integer_part != 0) exponent -= 6;
    while (integer_part % 10 == 0 && integer_part != 0) {
      integer_part /= 10;
      exponent++;
    }
    int tmp = 0;
    while (exponent < -28) {
      tmp = integer_part % 10;
      integer_part /= 10;
      exponent++;
    }
    if (tmp >= 5) integer_part++;
    s21_from_int_to_decimal(integer_part, dst);
    s21_decimal _ten = {{10, 0, 0, 0}};
    while (exponent > 0) {
      s21_mul(*dst, _ten, dst);
      exponent--;
    }
    exponent *= -1;
    SET_SCALE(*dst, exponent);
    ;
    if (sign) SET_SIGN(*dst, 1);
  }
  return flag;
}
