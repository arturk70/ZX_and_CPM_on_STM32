#!/bin/bash

truncate -s 128 0.bin
cp 0.bin DISK_0
cat CPM22.bin >> DISK_0
truncate -s 32k DISK_0
printf '\xE5%.0s' {1..8355840} > 1.bin
cat 1.bin >> DISK_0
rm 0.bin
rm 1.bin

cp DISK_0 DISK_1
cp DISK_0 DISK_2
cp DISK_0 DISK_3
cp DISK_0 DISK_4
cp DISK_0 DISK_5
cp DISK_0 DISK_6
cp DISK_0 DISK_7

cp CPM22.Z80 ASM

cpmcp -f 8mb-hd DISK_0 UTILS/* 0:
cpmcp -f 8mb-hd DISK_1 BASIC/* 0:
cpmcp -f 8mb-hd DISK_2 HITECH_C/* 0:
cpmcp -f 8mb-hd DISK_3 ASM/* 0:
cpmcp -f 8mb-hd DISK_4 BGAMES/* 0:
cpmcp -f 8mb-hd DISK_6 GAMES/* 0:
