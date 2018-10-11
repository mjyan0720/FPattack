#!/bin/bash


taskset -c 5 ./attack.o idle 20
taskset -c 5 ./attack.o normal_simd 20
taskset -c 5 ./attack.o sub_simd 20
taskset -c 5 ./attack.o int 20
taskset -c 5 ./attack.o normal_div 20
taskset -c 5 ./attack.o sub_div 20
