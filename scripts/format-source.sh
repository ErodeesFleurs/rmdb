#!/bin/sh

cd "`dirname \"$0\"`/../src"

: ${CLANG_FORMAT:=clang-format}

for file in *; do
  if [ -d "$file" ]; then
    find "$file" \( -name '*.cpp' -o -name '*.hpp' -o -name '*.c' -o -name '*.h' \) -exec $CLANG_FORMAT -fallback-style=none -i {} \;
  fi
done