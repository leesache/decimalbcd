#include "s21_decimal.h"

void s21_set_bit(s21_decimal *num, int bit, int value) {
  if (value) {
    num->bits[bit / 32] |= (1 << (bit % 32));
  } else {
    num->bits[bit / 32] &= ~(1 << (bit % 32));
  }
}

void s21_decimal_to_binary_string(s21_decimal decimal, char *str) {
  int index = 0;
  for (int i = 2; i >= 0; i--) {
    for (int j = 31; j >= 0; --j) {
      int bit = (decimal.bits[i] >> j) & 1;
      str[index++] = bit ? '1' : '0';
    }
  }
  str[index] = '\0';
}

void s21_binary_string_to_decimal(char *str, s21_decimal *decimal) {
  int index = 115;
  for (int i = 96; i >= 0; i--) {
    s21_set_bit(decimal, i, str[index] - 48);
    index++;
  }
}

void s21_binary_string_shift_left(char *str) {
  int len = strlen(str);
  for (int i = 0; i < len - 1; ++i) {
    str[i] = str[i + 1];
  }
  str[len - 1] = '0';
}

void s21_binary_string_shift_right(char *str) {
  int len = strlen(str);
  for (int i = len - 1; i > 0; --i) {
    str[i] = str[i - 1];
  }
  str[0] = '0';
}

int s21_bits_to_int(char *bits) {
  int value = 0;
  for (int i = 0; i < 4; i++) {
    value = (value << 1) | (bits[i] - '0');
  }
  return value;
}

void s21_int_to_bits(int value, char *bits) {
  for (int i = 3; i >= 0; i--) {
    bits[i] = (value & 1) ? '1' : '0';
    value >>= 1;
  }
  bits[4] = '\0';
}

void s21_add_or_sub_3_to_bcd_bits(char *str, int index, char is_add) {
  char bits[5];
  strncpy(bits, str + index, 4);
  bits[4] = '\0';

  int value = s21_bits_to_int(bits);
  if (is_add)
    value += 3;
  else
    value -= 3;
  s21_int_to_bits(value, bits);

  strncpy(str + index, bits, 4);
}

int s21_bcd_to_digit(char *bcd) {
  int digit = 0;
  for (int i = 0; i < 4; i++) {
    digit <<= 1;
    if (bcd[i] == '1') {
      digit |= 1;
    }
  }
  return digit;
}

int s21_is_bigger_five(char *str, int index) {
  for (int i = index; i >= 0; i -= 4) {
    int digit = s21_bcd_to_digit(&str[i]);
    if (digit >= 5) {
      return i;
    }
  }
  return -1;
}

void s21_binary_string_to_bcd_string(char *src, char *res) {
  s21_initialize_str(res);
  strncpy(res + 116, src, 96);
  int index = 116;
  for (int i = 0; i < 96; i++) {
    while ((index = s21_is_bigger_five(res, index - 4)) != -1) {
      s21_add_or_sub_3_to_bcd_bits(res, index, 1);
    }
    s21_binary_string_shift_left(res);
    index = 116;
  }
}

void s21_initialize_str(char *str) {
  int i = 0;
  for (; i < 213; i++) {
    str[i] = '0';
  }
  str[i - 1] = '\0';
}

void s21_bcd_string_to_binary_string(char *src, char *res) {
  strcpy(res, src);
  int index = 116;
  for (int i = 0; i < 96; i++) {
    s21_binary_string_shift_right(res);
    while ((index = s21_is_bigger_five(res, index - 4)) != -1) {
      s21_add_or_sub_3_to_bcd_bits(res, index, 0);
    }
    index = 116;
  }
}

void s21_bcd_string_to_int_array(const char *bcd_string, int *result,
                                 int *result_len) {
  int len = 116;
  *result_len = len / 4;
  for (int i = 0; i < *result_len; i++) {
    result[*result_len - 1 - i] =
        s21_bcd_to_digit((char *)&bcd_string[len - 4 * (i + 1)]);
  }
}

void s21_int_array_to_bcd_string(int int_array[], int array_len,
                                 char bcd_str[]) {
  for (int i = 0; i < array_len; i++) {
    int number = int_array[i];
    for (int j = 0; j < 4; j++) {
      bcd_str[4 * i + 3 - j] = (number % 2) ? '1' : '0';
      number /= 2;
    }
  }
}

void s21_init_decimal(s21_decimal *dec) {
  dec->bits[0] = dec->bits[1] = dec->bits[2] = dec->bits[3] = 0;
}

int s21_get_bit(s21_decimal value, int bit) {
  return (value.bits[bit / 32] >> (bit % 32)) & 1;
}
