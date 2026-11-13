@echo off

if not exist bin mkdir bin

clang -Wall -Wextra -Werror -pedantic -Wno-long-long -Wno-unused-but-set-variable -Wno-unused-function -std=c89 -Iinclude tests/test.c -o bin/test.exe