// AISL Decimal Arithmetic Header
// String-based arbitrary precision decimal arithmetic

#ifndef DECIMAL_H
#define DECIMAL_H

#include <stdint.h>

// Normalize a decimal string (remove unnecessary zeros)
char* decimal_normalize(const char* str);

// Compare two decimal strings
// Returns: -1 if a < b, 0 if a == b, 1 if a > b
int decimal_cmp(const char* a, const char* b);

// Negate a decimal (flip sign)
char* decimal_neg(const char* a);

// Arithmetic operations
char* decimal_add(const char* a, const char* b);
char* decimal_sub(const char* a, const char* b);
char* decimal_mul(const char* a, const char* b);
char* decimal_div(const char* a, const char* b, int precision);

// Conversion functions
char* decimal_from_int(int64_t val);
char* decimal_from_float(double val);
int64_t decimal_to_int(const char* decimal);
double decimal_to_float(const char* decimal);

#endif // DECIMAL_H
