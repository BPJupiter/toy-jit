@echo off

mkdir build
pushd build
cl /Z7 ..\src\toy_jit_main.c
popd
