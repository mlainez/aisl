#!/bin/bash
# Convert all AISL files from (call ...) to direct function call syntax

cd /var/home/marc/Projects/aisl

# Process all .aisl files
find . -name "*.aisl" -type f | while read file; do
    echo "Processing: $file"
    # Use perl for proper global replacement
    # Match: (call <whitespace> <function_name>
    # Replace with: (<function_name>
    perl -i -pe 's/\(call\s+([a-zA-Z_][a-zA-Z0-9_]*)/($1/g' "$file"
done

echo "Conversion complete!"
