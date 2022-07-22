#!/bin/bash
#
# Copyright by The HDF Group.                                              
# All rights reserved.                                                     
#                                                                          
# This file is part of HDF5. The full HDF5 copyright notice, including     
# terms governing use, modification, and redistribution, is contained in   
# the COPYING file, which can be found at the root of the source code
# distribution tree, or in https://www.hdfgroup.org/licenses.
# If you do not have access to either file, you may request a copy from
# help@hdfgroup.org.
#

# Purpose: Combine subfiles into a single HDF5 file. Requires the subfiling 
# configuration file either as a command-line argument, or the script will 
# search for the *.config file in the current directory. 

GRN='\033[0;32m'
RED='\033[0;31m'
PUR='\033[0;35m'
NC='\033[0m' # No Color

## CONFIG FILE CHECKS ##

if [ $# -eq 0 ]; then
    nfiles=$(find . -maxdepth 1 -type f -iname "*.config" -printf '.' | wc -m)
    if [[ "$nfiles" != "1" ]]; then
       echo -e "$RED More than one .config file found in current directory. $NC"
       exit 1
    fi
    file_config=$(find . -maxdepth 1 -type f -iname "*.config")
else
    file_config=$1
fi

if [ ! -f "$file_config" ]; then
    echo -e "$RED $file_config does not exist. $NC"
    exit 1
fi

stripe_size=$(grep "stripe_size=" $file_config  | cut -d "=" -f2)
if test -z "$stripe_size"; then
    echo -e "$RED failed to find stripe_size in $file_config $NC"
    exit 1
fi

subfiles=( $( sed -e '1,/hdf5_file=/d' $file_config ) )
#for i in "${subfiles[@]}"; do
#      echo "$i"
#done
if test -z "$subfiles"; then
    echo -e "$RED failed to find subfiles list in $file_config $NC"
    exit 1
fi

hdf5_file=$(grep "hdf5_file=" $file_config  | cut -d "=" -f2)
if test -z "$hdf5_file"; then
    echo -e "$RED failed to find hdf5 output file in $file_config $NC"
    exit 1
fi

rm -f $hdf5_file

## COMBINE SUBFILES INTO AN HDF5 FILE ##

skip=0
status=$nfiles
START="$(date +%s%N)"
while [ $status -gt 0 ]; do
  icnt=0
  for i in "${subfiles[@]}"; do
      fsize=$(wc -c $i | awk '{print $1}')
      if [ $(($skip*$stripe_size)) -le $fsize ]; then
          EXEC="dd count=1 bs=$stripe_size if=$i of=$hdf5_file skip=$skip oflag=append conv=notrunc"
          echo -e "$GRN $EXEC $NC"
          err="$( $EXEC 2>&1 > /dev/null &)"
          icnt=$(($icnt+1)) 
      else
          subfiles=("${subfiles[@]:0:$icnt}" "${subfiles[@]:$(($icnt+1))}")
          status=${#subfiles[@]}
      fi
  done; wait
  skip=$(($skip+1))
done
END=$[ $(date +%s%N) - ${START} ]
DURATION_SEC=$(awk -vp=$END -vq=0.000000001 'BEGIN{printf "%.4f" ,p * q}')
echo -e "$PUR COMPLETION TIME = $DURATION_SEC s $NC"


