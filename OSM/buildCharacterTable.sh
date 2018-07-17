#!/bin/bash

for((currentCharacter=1;currentCharacter<=65536;currentCharacter++))
do
    echo -e '\0'$currentCharacter;
done
