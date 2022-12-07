#!/bin/bash

rm *.txt
make clean
make

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
start=$(date +%s%N)
./seq 3 "$DIR/files/files1" sout_seq_files1_3.txt
end=$(date +%s%N)

if diff -w sout_seq_files1_3.txt files/correct_files1_3.txt; then
    echo Test 1 - Success--------------------seq-files1-3------------------------------Success
else
    echo Test 1 - Fail-----------------------seq-files1-3------------------------------Fail
fi
echo -e "Elapsed time (seq-files1-3): $(($(($end-$start))/1000000)) ms\n"

sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
start=$(date +%s%N)
./par_t 3 "$DIR/files/files1" sout_par_t_files1_3.txt
end=$(date +%s%N)

if diff -w sout_par_t_files1_3.txt files/correct_files1_3.txt; then
    echo Test 2 - Success--------------------par_t-files1-3----------------------------Success
else
    echo Test 2 - Fail-----------------------par_t-files1-3----------------------------Fail
fi
echo -e "Elapsed time (par_t-files1-3): $(($(($end-$start))/1000000)) ms\n"

sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
start=$(date +%s%N)
./par_p 3 "$DIR/files/files1" sout_par_p_files1_3.txt
end=$(date +%s%N)

if diff -w sout_par_p_files1_3.txt files/correct_files1_3.txt; then
    echo Test 3 - Success--------------------par_p-files1-3----------------------------Success
else
    echo Test 3 - Fail-----------------------par_p-files1-3----------------------------Fail
fi
echo -e "Elapsed time (par_p-files1-3): $(($(($end-$start))/1000000)) ms\n"

sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
start=$(date +%s%N)
./seq 10 "$DIR/files/files2" sout_seq_files2_10.txt
end=$(date +%s%N)

if diff -w sout_seq_files2_10.txt files/correct_files2_10.txt; then
    echo Test 4 - Success--------------------seq-files2-10-----------------------------Success
else
    echo Test 4 - Fail-----------------------seq-files2-10-----------------------------Fail
fi
echo -e "Elapsed time (seq-files2-10): $(($(($end-$start))/1000000)) ms\n"

sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
start=$(date +%s%N)
./par_t 10 "$DIR/files/files2" sout_par_t_files2_10.txt
end=$(date +%s%N)

if diff -w sout_par_t_files2_10.txt files/correct_files2_10.txt; then
    echo Test 5 - Success--------------------par_t-files2-10---------------------------Success
else
    echo Test 5 - Fail-----------------------par_t-files2-10---------------------------Fail
fi
echo -e "Elapsed time (par_t-files2-10): $(($(($end-$start))/1000000)) ms\n"

sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
start=$(date +%s%N)
./par_p 10 "$DIR/files/files2" sout_par_p_files2_10.txt
end=$(date +%s%N)

if diff -w sout_par_p_files2_10.txt files/correct_files2_10.txt; then
    echo Test 6 - Success--------------------par_p-files2-10---------------------------Success
else
    echo Test 6 - Fail-----------------------par_p-files2-10---------------------------Fail
fi
echo -e "Elapsed time (par_p-files2-10): $(($(($end-$start))/1000000)) ms\n"

sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
start=$(date +%s%N)
./seq 100 "$DIR/files/files4" sout_seq_files4_100.txt
end=$(date +%s%N)

if diff -w sout_seq_files4_100.txt files/correct_files4_100.txt; then
    echo Test 7 - Success--------------------seq-files4-100----------------------------Success
else
    echo Test 7 - Fail-----------------------seq-files4-100----------------------------Fail
fi
echo -e "Elapsed time (seq-files4-100): $(($(($end-$start))/1000000)) ms\n"

sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
start=$(date +%s%N)
./par_t 100 "$DIR/files/files4" sout_par_t_files4_100.txt
end=$(date +%s%N)

if diff -w sout_par_t_files4_100.txt files/correct_files4_100.txt; then
    echo Test 8 - Success--------------------par_t-files4-100--------------------------Success
else
    echo Test 8 - Fail-----------------------par_t-files4-100--------------------------Fail
fi
echo -e "Elapsed time (par_t-files4-100): $(($(($end-$start))/1000000)) ms\n"

sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
start=$(date +%s%N)
./par_p 100 "$DIR/files/files4" sout_par_p_files4_100.txt
end=$(date +%s%N)

if diff -w sout_par_p_files4_100.txt files/correct_files4_100.txt; then
    echo Test 9 - Success--------------------par_p-files4-100--------------------------Success
else
    echo Test 9 - Fail-----------------------par_p-files4-100--------------------------Fail
fi
echo -e "Elapsed time (par_p-files4-100): $(($(($end-$start))/1000000)) ms\n"

sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
start=$(date +%s%N)
./seq 1000 "$DIR/files/files8" sout_seq_files8_1000.txt
end=$(date +%s%N)

if diff -w sout_seq_files8_1000.txt files/correct_files8_1000.txt; then
    echo Test 10 - Success--------------------seq-files8-1000---------------------------Success
else
    echo Test 10 - Fail-----------------------seq-files8-1000---------------------------Fail
fi
echo -e "Elapsed time (seq-files8-1000): $(($(($end-$start))/1000000)) ms\n"

sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
start=$(date +%s%N)
./par_t 1000 "$DIR/files/files8" sout_par_t_files8_1000.txt
end=$(date +%s%N)

if diff -w sout_par_t_files8_1000.txt files/correct_files8_1000.txt; then
    echo Test 11 - Success--------------------par_t-files8-1000-------------------------Success
else
    echo Test 11 - Fail-----------------------par_t-files8-1000-------------------------Fail
fi
echo -e "Elapsed time (par_t-files8-1000): $(($(($end-$start))/1000000)) ms\n"

sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'
start=$(date +%s%N)
./par_p 1000 "$DIR/files/files8" sout_par_p_files8_1000.txt
end=$(date +%s%N)

if diff -w sout_par_p_files8_1000.txt files/correct_files8_1000.txt; then
    echo Test 12 - Success--------------------par_p-files8-1000-------------------------Success
else
    echo Test 12 - Fail-----------------------par_p-files8-1000-------------------------Fail
fi
echo -e "Elapsed time (par_p-files8-1000): $(($(($end-$start))/1000000)) ms\n"
