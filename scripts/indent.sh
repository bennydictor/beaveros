#!/bin/sh
if ! which indent >/dev/null 2>/dev/null; then
    echo "It seems like you don't have GNU indent installed"
    echo "Install it using your package manager"
    echo "or get it from ftp://ftp.gnu.org/gnu/indent"
fi

files=''
if [ "$1" == 'all' ]; then
    files="$(find -L src -type f -regex '.*\.[ch]$')"
elif [ "$1" == 'staged' ]; then
    files="$(git diff --cached --name-only | grep '\.[ch]$')"
elif [ -f "$1" ]; then
    files="$1"
else
    echo "Usage: $0 FILE"
    echo "Usage: $0 all"
    echo "Usage: $0 staged"
    exit 1
fi

args='
--blank-lines-after-procedures
--braces-on-func-def-line
--braces-on-if-line
--braces-on-struct-decl-line
--break-after-boolean-operator
--case-indentation0
--comment-indentation33
--continuation-indentation8
--cuddle-else
--declaration-comment-column33
--declaration-indentation1
--dont-break-procedure-type
--dont-format-comments
--dont-format-first-column-comments
--dont-line-up-parentheses
--dont-space-special-semicolon
--dont-star-comments
--else-endif-column0
--honour-newlines
--indent-label0
--indent-level4
--line-comments-indentation0
--line-length79
--no-blank-lines-after-commas
--no-blank-lines-after-declarations
--no-comment-delimiters-on-blank-lines
--no-space-after-function-call-names
--no-space-after-parentheses
--no-tabs
--parameter-indentation4
--paren-indentation4
--space-after-cast
--space-after-for
--space-after-if
--space-after-while
--swallow-optional-blank-lines
--tab-size1
-T int64_t
-T uint64_t
-T va_list
'

args="$args $(cat $files | tr '\n' ' ' | \
    egrep -o 'typedef (struct|enum|union) {[^}]*}[^;]*;' | \
    sed -E 's/.*\W(\w+);$/-T \1/' | sort -u)"
args="$args $(cat $files | \
    egrep -o 'typedef.*;' | \
    sed -E 's/.*\W(\w+);$/-T \1/' | sort -u)"

indent $args $files
