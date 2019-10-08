printf "\nStarting timings!\n"

N=100000000 # Nr of elements
NT=1 # Nr of threads

NT_BREAK_STR="\n***\t***\t***\t***\t***\t***\t***\t***\n"
N_BREAK_STR=$NT_BREAK_STR
N_BREAK_STR+="\n***********************************************************\n"

N=100000000
printf $N_BREAK_STR

NT=1
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=2
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=4
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=8
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=16
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=32
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=64
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=128
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=256
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT

N=200000000
printf $N_BREAK_STR

NT=1
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=2
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=4
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=8
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=16
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=32
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=64
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=128
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=256
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT

N=400000000
printf $N_BREAK_STR

NT=1
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=2
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=4
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=8
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=16
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=32
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=64
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=128
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=256
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT

N=800000000
printf $N_BREAK_STR

NT=1
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=2
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=4
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=8
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=16
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=32
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=64
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=128
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
NT=256
printf $NT_BREAK_STR
./parallelqsort_demo $N $NT
