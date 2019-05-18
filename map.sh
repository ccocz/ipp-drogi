#!/bin/bash

if [[ $# == 1 || $# == 0 ]]
then
    exit 1
fi

FILE="$1"
shift

while [[ $# -ne 0 ]]
do
    ARG="$1"
    grep ^"${ARG};" ${FILE}
    shift
done

#grep ^'13;'

#sed