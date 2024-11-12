#include "s21_decimal.h"

int s21_is_equal(s21_decimal dec1, s21_decimal dec2) {
  s21_decimal dec_res;
  int res = 1;
  s21_sub(dec1, dec2, &dec_res);
  for (int i = 0; i < 3; i++) {
    if (dec_res.bits[i] != 0) res = 0;
  }
  return res;
}

int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_equal(dec1, dec2);
}

int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
  s21_decimal res_dec;
  int res = 0;

  s21_sub(dec1, dec2, &res_dec);

  if (GET_SIGN(res_dec)) res = 1;
  return res;
}

int s21_is_greater(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_less(dec1, dec2) && !s21_is_equal(dec1, dec2);
}

int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2) {
  return s21_is_less(dec1, dec2) || s21_is_equal(dec1, dec2);
}

int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2) {
  return s21_is_greater(dec1, dec2) || s21_is_equal(dec1, dec2);
}
