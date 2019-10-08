CC = gcc
CFLAGS = -Wall -O3 #-march=native -funroll-loops #-ffast-math
#CFLAGS += -Xpreprocessor # Use for mac
CFLAGS += -fopenmp
#CFLAGS += -g
LDFLAGS = -lm
#LDFLAGS += -lomp # Use for mac
LDFLAGS += -fopenmp # Use for linux

parallelqsort_demo: parallelqsort.o main.o
	$(CC) parallelqsort.o main.o \
	-o parallelqsort_demo $(LDFLAGS)

parallelqsort.o: parallelqsort.c parallelqsort.h
	$(CC) $(CFLAGS) -c parallelqsort.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f parallelqsort_demo *.o
