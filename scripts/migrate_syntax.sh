#!/bin/bash
# Script to migrate AISL code from old syntax (op_add_i32) to new syntax (add)

set -e

migrate_file() {
    local file="$1"
    echo "Migrating $file..."
    
    # Create backup
    cp "$file" "$file.bak"
    
    # Arithmetic operations
    sed -i 's/op_add_i32/add/g' "$file"
    sed -i 's/op_add_i64/add/g' "$file"
    sed -i 's/op_add_f32/add/g' "$file"
    sed -i 's/op_add_f64/add/g' "$file"
    
    sed -i 's/op_sub_i32/sub/g' "$file"
    sed -i 's/op_sub_i64/sub/g' "$file"
    sed -i 's/op_sub_f32/sub/g' "$file"
    sed -i 's/op_sub_f64/sub/g' "$file"
    
    sed -i 's/op_mul_i32/mul/g' "$file"
    sed -i 's/op_mul_i64/mul/g' "$file"
    sed -i 's/op_mul_f32/mul/g' "$file"
    sed -i 's/op_mul_f64/mul/g' "$file"
    
    sed -i 's/op_div_i32/div/g' "$file"
    sed -i 's/op_div_i64/div/g' "$file"
    sed -i 's/op_div_f32/div/g' "$file"
    sed -i 's/op_div_f64/div/g' "$file"
    
    sed -i 's/op_mod_i32/mod/g' "$file"
    sed -i 's/op_mod_i64/mod/g' "$file"
    
    sed -i 's/op_neg_i32/neg/g' "$file"
    sed -i 's/op_neg_i64/neg/g' "$file"
    sed -i 's/op_neg_f32/neg/g' "$file"
    sed -i 's/op_neg_f64/neg/g' "$file"
    
    # Comparison operations
    sed -i 's/op_eq_i32/eq/g' "$file"
    sed -i 's/op_eq_i64/eq/g' "$file"
    sed -i 's/op_eq_f32/eq/g' "$file"
    sed -i 's/op_eq_f64/eq/g' "$file"
    
    sed -i 's/op_ne_i32/ne/g' "$file"
    sed -i 's/op_ne_i64/ne/g' "$file"
    sed -i 's/op_ne_f32/ne/g' "$file"
    sed -i 's/op_ne_f64/ne/g' "$file"
    
    sed -i 's/op_lt_i32/lt/g' "$file"
    sed -i 's/op_lt_i64/lt/g' "$file"
    sed -i 's/op_lt_f32/lt/g' "$file"
    sed -i 's/op_lt_f64/lt/g' "$file"
    
    sed -i 's/op_gt_i32/gt/g' "$file"
    sed -i 's/op_gt_i64/gt/g' "$file"
    sed -i 's/op_gt_f32/gt/g' "$file"
    sed -i 's/op_gt_f64/gt/g' "$file"
    
    sed -i 's/op_le_i32/le/g' "$file"
    sed -i 's/op_le_i64/le/g' "$file"
    sed -i 's/op_le_f32/le/g' "$file"
    sed -i 's/op_le_f64/le/g' "$file"
    
    sed -i 's/op_ge_i32/ge/g' "$file"
    sed -i 's/op_ge_i64/ge/g' "$file"
    sed -i 's/op_ge_f32/ge/g' "$file"
    sed -i 's/op_ge_f64/ge/g' "$file"
    
    # Math operations
    sed -i 's/math_abs_i32/abs/g' "$file"
    sed -i 's/math_abs_i64/abs/g' "$file"
    sed -i 's/math_abs_f32/abs/g' "$file"
    sed -i 's/math_abs_f64/abs/g' "$file"
    
    sed -i 's/math_min_i32/min/g' "$file"
    sed -i 's/math_min_i64/min/g' "$file"
    sed -i 's/math_min_f32/min/g' "$file"
    sed -i 's/math_min_f64/min/g' "$file"
    
    sed -i 's/math_max_i32/max/g' "$file"
    sed -i 's/math_max_i64/max/g' "$file"
    sed -i 's/math_max_f32/max/g' "$file"
    sed -i 's/math_max_f64/max/g' "$file"
    
    echo "✓ Migrated $file"
}

# Migrate all .aisl files in tests and examples
find tests examples -name "*.aisl" -type f | while read file; do
    migrate_file "$file"
done

echo ""
echo "Migration complete! Backup files created with .bak extension"
echo "To remove backups: find tests examples -name '*.bak' -delete"
