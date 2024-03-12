#!/usr/bin/env bash

#  MANPATH=$(manpath)

extract_header() {
  echo $1 | sed -rn 's/.*include <(.*)>.*/\1/p' | head -1
}


find_include() {
  IFS=':'
  for dir in $MANPATH
  do
    compressed_page="$dir/man3/$1.3.gz"
  uncompressed_page="$dir/man3/$1.3"

  if [ -f "$uncompressed_page" ]
  then
  extract_header "$(cat $uncompressed_page)" && return 0
  elif [ -f "$compressed_page" ]
  then
  extract_header "$(gunzip -c $compressed_page)" && return 0
  fi
    done
  echo "---"
  return 1
}



while read -r name
do
find_include $name
done