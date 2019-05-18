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
    ANS=0
    CURRENT=$(grep ^"${ARG};" ${FILE} | cut -d ';' -f 3-)
    while [[ 1 == 1 ]]
    do
        MAL=$(echo ${CURRENT} | cut -d ';' -f 1)
        ANS=$(echo "$((MAL + ANS))")
        CURRENT=$(echo ${CURRENT} | cut -d ';' -f 4-)
        if [[ -z ${CURRENT} ]]
        then
            break;
        fi
    done
    if [[ ${ANS} != 0 ]]
    then
        echo ${ANS}
    fi
    shift
done