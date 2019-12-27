fsutil file createnew 0.bin 128
copy /B 0.bin+CPM22.bin 2.bin
fsutil file setEOF 2.bin 32768
del 0.bin
ren 2.bin 0.bin
echo ееееееееееееееее > 1.bin
fsutil file setEOF 1.bin 16
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
@copy /B 1.bin+1.bin 2.bin > NUL & del 1.bin & ren 2.bin 1.bin
copy /B 0.bin+1.bin DISK_0
fsutil file setEOF DISK_0 8388608
del 0.bin
del 1.bin

copy DISK_0 DISK_1
copy DISK_0 DISK_2
copy DISK_0 DISK_3
copy DISK_0 DISK_4
copy DISK_0 DISK_5
copy DISK_0 DISK_6
copy DISK_0 DISK_7

copy CPM22.Z80 ASM

cd win_tools/cpmtools
cpmcp -f 8mb-hd ../../DISK_0 ../../UTILS/* 0:
cpmcp -f 8mb-hd ../../DISK_1 ../../BASIC/* 0:
cpmcp -f 8mb-hd ../../DISK_1 ../../BGAMES/* 1:
cpmcp -f 8mb-hd ../../DISK_2 ../../HITECH_C/* 0:
cpmcp -f 8mb-hd ../../DISK_3 ../../ASM/* 0:
cpmcp -f 8mb-hd ../../DISK_6 ../../GAMES/* 0:
