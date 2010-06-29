CC=gcc
CF=-g

libsmallr: main.o regex.o hashmap.o
	${CC} ${CF} main.o regex.o hashmap.o -o libsmallr

main.o: main.c *.h
	${CC} ${CF} -c main.c
regex.o: regex.c regex.h
	${CC} ${CF} -c regex.c
hashmap.o: hashmap.c hashmap.h
	${CC} ${CF} -c hashmap.c

clean:
	rm *.o

run:
	./libsmallr
