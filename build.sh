#!/bin/sh
set -e

top="`pwd`"
build="`pwd`/build"

rm -rf "${build}"
mkdir -p "${build}"

exec 9>"${build}/Makefile"

#all
(
  echo "all: sysdep crypto tinyssh"
  echo
  echo "clean:"
  echo "\trm -f *.o ${h}"
  echo
) >&9

#sysdep
cp -pr sysdep/*.out sysdep/*.c "${build}"
(
  cd "${build}"
  all=`ls *yes.out | sed 's/-yes.out//' | tr -s '\n' ' '`

  (
    echo "sysdep: ${all}"
    echo

    for x in `ls -1 *yes.out`; do
      file=`echo ${x} | sed 's/-yes.out//'`
      echo "${file}: \\"
      echo "\t${file}-yes.c ${file}-yes.out"
      echo "\t \$(CC) -c ${file}-yes.c 2>/dev/null \\"
      echo "\t&& cp ${file}-yes.out ${file} \\"
      echo "\t|| echo > ${file}"
      echo "\trm -f ${file}-yes.o"
      echo
    done
  ) >&9
  make sysdep
)

#crypto
cp -pr crypto/* "${build}"
(
  cd "${build}"
  all=`cat CRYPTOLIBS | tr -s '\n' ' '`

  ls -1 crypto_*.h | sed 's/^/#include \"/' | sed 's/$/\"/' > crypto.h

  (
    echo "crypto: ${all}"
    echo

    cat CRYPTOSOURCES | while read file; do
      gcc -MM "${file}.c" 
      echo "\t \$(CC) -c ${file}.c"
      echo
    done
  ) >&9
) 

#tinyssh
cp -pr tinyssh/* "${build}"
(
  cd "${build}"
  all=`cat LIBS | tr -s '\n' ' '`

  (
    echo "tinyssh: ${all}"
    echo

    cat SOURCES | while read file; do
      gcc -MM "${file}.c" 
      echo "\t \$(CC) -c ${file}.c"
      echo
    done
  ) >&9
) 

