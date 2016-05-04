all: clcg4.h clcg4.c template.c
	gcc -I. -g -lpthread -Wall -O3 -c clcg4.c -o clcg4.o
	mpicc -I. -g -lpthread  -Wall -O3 template.c clcg4.o -o antFarm
