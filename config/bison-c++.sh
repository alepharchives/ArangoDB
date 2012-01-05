#!/bin/sh

BISON="$1"
OUTPUT="$2"
INPUT="$3"

if test "x$BISON" = x -o "x$OUTPUT" = x -o "x$INPUT" = x;  then
  echo "usage: $0 <bison> <output> <input>"
  exit 1
fi

#############################################################################
## bison
#############################################################################

${BISON} -l -d -ra -S lalr1.cc -o ${OUTPUT} ${INPUT}

#############################################################################
## sanity checks
#############################################################################

PREFIX=`echo ${OUTPUT} | sed -e 's:\.cpp$::'`

test -f ${PREFIX}.hpp || exit 1
test -f ${PREFIX}.cpp || exit 1

#############################################################################
## rename file
#############################################################################

mv ${PREFIX}.hpp ${PREFIX}.h || exit 1

#############################################################################
## fix header file name in source, fix defines
#############################################################################

sed -e 's:\.hpp:.h:' < ${OUTPUT} \
  | sed -e 's:# if YYENABLE_NLS:# if defined(YYENABLE_NLS) \&\& YYENABLE_NLS:' \
  > ${OUTPUT}.tmp

# give some information
diff -u ${OUTPUT} ${OUTPUT}.tmp

# and move the files to the final destination
mv ${OUTPUT}.tmp ${OUTPUT} || exit 1
