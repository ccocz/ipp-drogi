#!/bin/bash

if (( $# == 1 || $# == 0 ))
    then
        exit 1
fi

FILE="$1"

if ! [[ -f ${FILE} ]]
    then
        exit 1
fi

shift

for scale in "$@"
    do
    if ! [[ "$scale" =~ ^[0-9]+$ ]]
        then
            exit 1
    fi
    if ! (( "$scale" > 0 && "$scale" < 1000 ))
        then
            exit 1
    fi
done

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
    if (( ${ANS} != 0 ))
        then
            echo ${ARG}';'${ANS}
    fi
    shift
done