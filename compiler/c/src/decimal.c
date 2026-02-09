#define _POSIX_C_SOURCE 200809L
#include "decimal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool negative;
    char* integer_part;
    char* fractional_part;
} DecimalParts;

static DecimalParts decimal_parse_parts(const char* str);
static void decimal_free_parts(DecimalParts parts);
static char* decimal_from_parts(DecimalParts parts);
static int decimal_compare_absolute(const char* a, const char* b);
char* decimal_sub(const char* a, const char* b);

static DecimalParts decimal_parse_parts(const char* str) {
    DecimalParts parts;
    parts.negative = false;
    parts.integer_part = NULL;
    parts.fractional_part = NULL;
    
    if (!str || !*str) {
        parts.integer_part = strdup("0");
        parts.fractional_part = strdup("");
        return parts;
    }
    
    const char* p = str;
    if (*p == '-') {
        parts.negative = true;
        p++;
    }
    
    const char* dot = strchr(p, '.');
    
    if (dot) {
        size_t int_len = dot - p;
        parts.integer_part = strndup(p, int_len);
        parts.fractional_part = strdup(dot + 1);
    } else {
        parts.integer_part = strdup(p);
        parts.fractional_part = strdup("");
    }
    
    if (!parts.integer_part || !*parts.integer_part) {
        free(parts.integer_part);
        parts.integer_part = strdup("0");
    }
    
    return parts;
}

static void decimal_free_parts(DecimalParts parts) {
    free(parts.integer_part);
    free(parts.fractional_part);
}

static char* decimal_from_parts(DecimalParts parts) {
    size_t frac_len = parts.fractional_part ? strlen(parts.fractional_part) : 0;
    
    while (frac_len > 0 && parts.fractional_part[frac_len - 1] == '0') {
        frac_len--;
    }
    
    char* int_part = parts.integer_part;
    while (*int_part == '0' && *(int_part + 1) != '\0') {
        int_part++;
    }
    
    bool is_zero = (strcmp(int_part, "0") == 0 && frac_len == 0);
    
    size_t total = (parts.negative && !is_zero ? 1 : 0) + strlen(int_part) + 
                   (frac_len > 0 ? 1 + frac_len : 0) + 1;
    char* result = malloc(total);
    char* p = result;
    
    if (parts.negative && !is_zero) {
        *p++ = '-';
    }
    
    strcpy(p, int_part);
    p += strlen(int_part);
    
    if (frac_len > 0) {
        *p++ = '.';
        strncpy(p, parts.fractional_part, frac_len);
        p[frac_len] = '\0';
    }
    
    return result;
}

char* decimal_normalize(const char* decimal) {
    DecimalParts parts = decimal_parse_parts(decimal);
    char* result = decimal_from_parts(parts);
    decimal_free_parts(parts);
    return result;
}

int decimal_cmp(const char* a, const char* b) {
    DecimalParts pa = decimal_parse_parts(a);
    DecimalParts pb = decimal_parse_parts(b);
    
    if (pa.negative != pb.negative) {
        decimal_free_parts(pa);
        decimal_free_parts(pb);
        return pa.negative ? -1 : 1;
    }
    
    int abs_cmp = decimal_compare_absolute(a, b);
    int result = pa.negative ? -abs_cmp : abs_cmp;
    
    decimal_free_parts(pa);
    decimal_free_parts(pb);
    return result;
}

static int decimal_compare_absolute(const char* a, const char* b) {
    DecimalParts pa = decimal_parse_parts(a);
    DecimalParts pb = decimal_parse_parts(b);
    
    char* int_a = pa.integer_part;
    char* int_b = pb.integer_part;
    
    while (*int_a == '0' && *(int_a + 1)) int_a++;
    while (*int_b == '0' && *(int_b + 1)) int_b++;
    
    size_t len_a = strlen(int_a);
    size_t len_b = strlen(int_b);
    
    if (len_a != len_b) {
        decimal_free_parts(pa);
        decimal_free_parts(pb);
        return len_a > len_b ? 1 : -1;
    }
    
    int cmp = strcmp(int_a, int_b);
    if (cmp != 0) {
        decimal_free_parts(pa);
        decimal_free_parts(pb);
        return cmp;
    }
    
    size_t frac_len_a = pa.fractional_part ? strlen(pa.fractional_part) : 0;
    size_t frac_len_b = pb.fractional_part ? strlen(pb.fractional_part) : 0;
    size_t max_frac = frac_len_a > frac_len_b ? frac_len_a : frac_len_b;
    
    for (size_t i = 0; i < max_frac; i++) {
        char ca = i < frac_len_a ? pa.fractional_part[i] : '0';
        char cb = i < frac_len_b ? pb.fractional_part[i] : '0';
        if (ca != cb) {
            decimal_free_parts(pa);
            decimal_free_parts(pb);
            return ca > cb ? 1 : -1;
        }
    }
    
    decimal_free_parts(pa);
    decimal_free_parts(pb);
    return 0;
}

char* decimal_neg(const char* decimal) {
    DecimalParts parts = decimal_parse_parts(decimal);
    
    bool is_zero = strcmp(parts.integer_part, "0") == 0 && 
                   (!parts.fractional_part || !*parts.fractional_part || 
                    strcmp(parts.fractional_part, "0") == 0);
    
    if (!is_zero) {
        parts.negative = !parts.negative;
    }
    
    char* result = decimal_from_parts(parts);
    decimal_free_parts(parts);
    return result;
}

char* decimal_add(const char* a, const char* b) {
    DecimalParts pa = decimal_parse_parts(a);
    DecimalParts pb = decimal_parse_parts(b);
    
    if (pa.negative == pb.negative) {
        size_t frac_len_a = pa.fractional_part ? strlen(pa.fractional_part) : 0;
        size_t frac_len_b = pb.fractional_part ? strlen(pb.fractional_part) : 0;
        size_t max_frac = frac_len_a > frac_len_b ? frac_len_a : frac_len_b;
        
        char* frac_a = calloc(max_frac + 1, 1);
        char* frac_b = calloc(max_frac + 1, 1);
        
        if (pa.fractional_part) strcpy(frac_a, pa.fractional_part);
        if (pb.fractional_part) strcpy(frac_b, pb.fractional_part);
        
        for (size_t i = frac_len_a; i < max_frac; i++) frac_a[i] = '0';
        for (size_t i = frac_len_b; i < max_frac; i++) frac_b[i] = '0';
        
        char* result_frac = malloc(max_frac + 2);
        int carry = 0;
        
        for (int i = max_frac - 1; i >= 0; i--) {
            int digit_a = frac_a[i] - '0';
            int digit_b = frac_b[i] - '0';
            int sum = digit_a + digit_b + carry;
            carry = sum / 10;
            result_frac[i] = (sum % 10) + '0';
        }
        result_frac[max_frac] = '\0';
        
        size_t len_a = strlen(pa.integer_part);
        size_t len_b = strlen(pb.integer_part);
        size_t max_len = (len_a > len_b ? len_a : len_b) + 2;
        
        char* result_int = calloc(max_len, 1);
        int pos = 0;
        
        int i = len_a - 1;
        int j = len_b - 1;
        
        while (i >= 0 || j >= 0 || carry) {
            int digit_a = (i >= 0) ? (pa.integer_part[i] - '0') : 0;
            int digit_b = (j >= 0) ? (pb.integer_part[j] - '0') : 0;
            int sum = digit_a + digit_b + carry;
            carry = sum / 10;
            result_int[pos++] = (sum % 10) + '0';
            i--;
            j--;
        }
        result_int[pos] = '\0';
        
        for (int k = 0; k < pos / 2; k++) {
            char tmp = result_int[k];
            result_int[k] = result_int[pos - 1 - k];
            result_int[pos - 1 - k] = tmp;
        }
        
        DecimalParts result_parts;
        result_parts.negative = pa.negative;
        result_parts.integer_part = result_int;
        result_parts.fractional_part = result_frac;
        
        char* result = decimal_from_parts(result_parts);
        
        free(frac_a);
        free(frac_b);
        free(result_int);
        free(result_frac);
        decimal_free_parts(pa);
        decimal_free_parts(pb);
        
        return result;
    } else {
        // Different signs: convert to subtraction
        // -5 + 10 becomes 10 - 5 = 5
        // 10 + (-5) becomes 10 - 5 = 5
        // We subtract the absolute values and determine the sign
        
        int abs_cmp = decimal_compare_absolute(a, b);
        
        if (abs_cmp == 0) {
            // Equal absolute values, opposite signs: result is 0
            decimal_free_parts(pa);
            decimal_free_parts(pb);
            return strdup("0");
        }
        
        // Determine which has larger absolute value
        const char* larger = abs_cmp > 0 ? a : b;
        const char* smaller = abs_cmp > 0 ? b : a;
        
        // Result takes sign of the number with larger absolute value
        bool result_negative = (abs_cmp > 0) ? pa.negative : pb.negative;
        
        DecimalParts pl = decimal_parse_parts(larger);
        DecimalParts ps = decimal_parse_parts(smaller);
        
        // Now subtract absolute values
        size_t frac_len_l = pl.fractional_part ? strlen(pl.fractional_part) : 0;
        size_t frac_len_s = ps.fractional_part ? strlen(ps.fractional_part) : 0;
        size_t max_frac = frac_len_l > frac_len_s ? frac_len_l : frac_len_s;
        
        char* frac_l = calloc(max_frac + 1, 1);
        char* frac_s = calloc(max_frac + 1, 1);
        
        if (pl.fractional_part) strcpy(frac_l, pl.fractional_part);
        if (ps.fractional_part) strcpy(frac_s, ps.fractional_part);
        
        for (size_t i = frac_len_l; i < max_frac; i++) frac_l[i] = '0';
        for (size_t i = frac_len_s; i < max_frac; i++) frac_s[i] = '0';
        
        char* result_frac = malloc(max_frac + 2);
        int borrow = 0;
        
        for (int i = max_frac - 1; i >= 0; i--) {
            int digit_l = frac_l[i] - '0';
            int digit_s = frac_s[i] - '0';
            int diff = digit_l - digit_s - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result_frac[i] = diff + '0';
        }
        result_frac[max_frac] = '\0';
        
        size_t len_l = strlen(pl.integer_part);
        size_t len_s = strlen(ps.integer_part);
        size_t max_len = (len_l > len_s ? len_l : len_s) + 2;
        
        char* result_int = calloc(max_len, 1);
        int pos = 0;
        
        int i = len_l - 1;
        int j = len_s - 1;
        
        while (i >= 0 || j >= 0) {
            int digit_l = (i >= 0) ? (pl.integer_part[i] - '0') : 0;
            int digit_s = (j >= 0) ? (ps.integer_part[j] - '0') : 0;
            int diff = digit_l - digit_s - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result_int[pos++] = diff + '0';
            i--;
            j--;
        }
        result_int[pos] = '\0';
        
        // Reverse result_int
        for (int k = 0; k < pos / 2; k++) {
            char tmp = result_int[k];
            result_int[k] = result_int[pos - 1 - k];
            result_int[pos - 1 - k] = tmp;
        }
        
        DecimalParts result_parts;
        result_parts.negative = result_negative;
        result_parts.integer_part = result_int;
        result_parts.fractional_part = result_frac;
        
        char* result = decimal_from_parts(result_parts);
        
        free(frac_l);
        free(frac_s);
        free(result_int);
        free(result_frac);
        decimal_free_parts(pa);
        decimal_free_parts(pb);
        decimal_free_parts(pl);
        decimal_free_parts(ps);
        
        return result;
    }
}

char* decimal_sub(const char* a, const char* b) {
    DecimalParts pa = decimal_parse_parts(a);
    DecimalParts pb = decimal_parse_parts(b);
    
    if (pa.negative != pb.negative) {
        char* b_neg = decimal_neg(b);
        char* result = decimal_add(a, b_neg);
        free(b_neg);
        decimal_free_parts(pa);
        decimal_free_parts(pb);
        return result;
    }
    
    int abs_cmp = decimal_compare_absolute(a, b);
    
    if (abs_cmp == 0) {
        decimal_free_parts(pa);
        decimal_free_parts(pb);
        return strdup("0");
    }
    
    const char* larger = abs_cmp > 0 ? a : b;
    const char* smaller = abs_cmp > 0 ? b : a;
    bool result_negative = (abs_cmp > 0) ? pa.negative : pb.negative;
    
    DecimalParts pl = decimal_parse_parts(larger);
    DecimalParts ps = decimal_parse_parts(smaller);
    
    size_t frac_len_l = pl.fractional_part ? strlen(pl.fractional_part) : 0;
    size_t frac_len_s = ps.fractional_part ? strlen(ps.fractional_part) : 0;
    size_t max_frac = frac_len_l > frac_len_s ? frac_len_l : frac_len_s;
    
    char* frac_l = calloc(max_frac + 1, 1);
    char* frac_s = calloc(max_frac + 1, 1);
    
    if (pl.fractional_part) strcpy(frac_l, pl.fractional_part);
    if (ps.fractional_part) strcpy(frac_s, ps.fractional_part);
    
    for (size_t i = frac_len_l; i < max_frac; i++) frac_l[i] = '0';
    for (size_t i = frac_len_s; i < max_frac; i++) frac_s[i] = '0';
    
    char* result_frac = malloc(max_frac + 1);
    int borrow = 0;
    
    for (int i = max_frac - 1; i >= 0; i--) {
        int digit_l = frac_l[i] - '0';
        int digit_s = frac_s[i] - '0';
        int diff = digit_l - digit_s - borrow;
        
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result_frac[i] = diff + '0';
    }
    result_frac[max_frac] = '\0';
    
    size_t len_l = strlen(pl.integer_part);
    size_t len_s = strlen(ps.integer_part);
    size_t max_len = len_l > len_s ? len_l : len_s;
    
    char* result_int = calloc(max_len + 2, 1);
    int pos = 0;
    
    int i = len_l - 1;
    int j = len_s - 1;
    
    while (i >= 0 || j >= 0) {
        int digit_l = (i >= 0) ? (pl.integer_part[i] - '0') : 0;
        int digit_s = (j >= 0) ? (ps.integer_part[j] - '0') : 0;
        int diff = digit_l - digit_s - borrow;
        
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result_int[pos++] = diff + '0';
        i--;
        j--;
    }
    
    if (pos == 0) {
        result_int[pos++] = '0';
    }
    result_int[pos] = '\0';
    
    for (int k = 0; k < pos / 2; k++) {
        char tmp = result_int[k];
        result_int[k] = result_int[pos - 1 - k];
        result_int[pos - 1 - k] = tmp;
    }
    
    DecimalParts result_parts;
    result_parts.negative = result_negative;
    result_parts.integer_part = result_int;
    result_parts.fractional_part = result_frac;
    
    char* result = decimal_from_parts(result_parts);
    
    free(frac_l);
    free(frac_s);
    free(result_int);
    free(result_frac);
    decimal_free_parts(pa);
    decimal_free_parts(pb);
    decimal_free_parts(pl);
    decimal_free_parts(ps);
    
    return result;
}

char* decimal_mul(const char* a, const char* b) {
    DecimalParts pa = decimal_parse_parts(a);
    DecimalParts pb = decimal_parse_parts(b);
    
    bool is_zero_a = strcmp(pa.integer_part, "0") == 0 && 
                     (!pa.fractional_part || !*pa.fractional_part);
    bool is_zero_b = strcmp(pb.integer_part, "0") == 0 && 
                     (!pb.fractional_part || !*pb.fractional_part);
    
    if (is_zero_a || is_zero_b) {
        decimal_free_parts(pa);
        decimal_free_parts(pb);
        return strdup("0");
    }
    
    size_t frac_len_a = pa.fractional_part ? strlen(pa.fractional_part) : 0;
    size_t frac_len_b = pb.fractional_part ? strlen(pb.fractional_part) : 0;
    size_t total_decimal_places = frac_len_a + frac_len_b;
    
    char* combined_a = malloc(strlen(pa.integer_part) + frac_len_a + 1);
    strcpy(combined_a, pa.integer_part);
    if (pa.fractional_part) strcat(combined_a, pa.fractional_part);
    
    char* combined_b = malloc(strlen(pb.integer_part) + frac_len_b + 1);
    strcpy(combined_b, pb.integer_part);
    if (pb.fractional_part) strcat(combined_b, pb.fractional_part);
    
    size_t len_a = strlen(combined_a);
    size_t len_b = strlen(combined_b);
    size_t result_len = len_a + len_b;
    
    int* result_digits = calloc(result_len, sizeof(int));
    
    for (int i = len_a - 1; i >= 0; i--) {
        for (int j = len_b - 1; j >= 0; j--) {
            int digit_a = combined_a[i] - '0';
            int digit_b = combined_b[j] - '0';
            int product = digit_a * digit_b;
            int pos = (len_a - 1 - i) + (len_b - 1 - j);
            
            result_digits[pos] += product;
            
            int carry = result_digits[pos] / 10;
            result_digits[pos] %= 10;
            int carry_pos = pos + 1;
            while (carry > 0) {
                result_digits[carry_pos] += carry;
                carry = result_digits[carry_pos] / 10;
                result_digits[carry_pos] %= 10;
                carry_pos++;
            }
        }
    }
    
    char* result_str = malloc(result_len + 2);
    size_t result_pos = 0;
    bool started = false;
    
    for (int i = result_len - 1; i >= 0; i--) {
        if (result_digits[i] != 0 || started || i == 0) {
            result_str[result_pos++] = result_digits[i] + '0';
            started = true;
        }
    }
    result_str[result_pos] = '\0';
    
    if (total_decimal_places > 0 && total_decimal_places < result_pos) {
        size_t int_part_len = result_pos - total_decimal_places;
        char* final_result = malloc(result_pos + 2);
        
        strncpy(final_result, result_str, int_part_len);
        final_result[int_part_len] = '.';
        strcpy(final_result + int_part_len + 1, result_str + int_part_len);
        
        free(result_str);
        result_str = final_result;
    } else if (total_decimal_places >= result_pos) {
        size_t zeros_needed = total_decimal_places - result_pos;
        char* final_result = malloc(total_decimal_places + 4);
        strcpy(final_result, "0.");
        for (size_t i = 0; i < zeros_needed; i++) {
            strcat(final_result, "0");
        }
        strcat(final_result, result_str);
        free(result_str);
        result_str = final_result;
    }
    
    DecimalParts result_parts = decimal_parse_parts(result_str);
    result_parts.negative = (pa.negative != pb.negative);
    char* final = decimal_from_parts(result_parts);
    
    free(combined_a);
    free(combined_b);
    free(result_digits);
    free(result_str);
    decimal_free_parts(pa);
    decimal_free_parts(pb);
    decimal_free_parts(result_parts);
    
    return final;
}

char* decimal_div(const char* a, const char* b, int precision) {
    DecimalParts pb = decimal_parse_parts(b);
    bool is_zero_b = strcmp(pb.integer_part, "0") == 0 && 
                     (!pb.fractional_part || !*pb.fractional_part);
    decimal_free_parts(pb);
    
    if (is_zero_b) {
        return strdup("0");
    }
    
    if (precision <= 0) precision = 15;
    
    double val_a = atof(a);
    double val_b = atof(b);
    double quotient = val_a / val_b;
    
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%.*f", precision, quotient);
    
    return decimal_normalize(buffer);
}

char* decimal_from_int(int64_t value) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%ld", (long)value);
    return strdup(buffer);
}

char* decimal_from_float(double value) {
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%.15g", value);
    return decimal_normalize(buffer);
}

int64_t decimal_to_int(const char* decimal) {
    DecimalParts parts = decimal_parse_parts(decimal);
    int64_t value = atoll(parts.integer_part);
    if (parts.negative) value = -value;
    decimal_free_parts(parts);
    return value;
}

double decimal_to_float(const char* decimal) {
    return atof(decimal);
}
