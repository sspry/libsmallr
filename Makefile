CC=gcc
CF=-g

libsmallr: main.o regex.o hashmap.o
	${CC} ${CF} main.o regex.o parse.o compile.o hashmap.o -o libsmallr

main.o: main.c *.h
	${CC} ${CF} -c main.c
regex.o: regex.c regex.h *.h
	${CC} ${CF} -c regex.c
parse.o: parse.c parse.h *.h
	${CC} ${CF} -c parse.c
compile.c: compile.c compile.h *.h
	${CC} ${CF} -c compile.c
hashmap.o: hashmap.c hashmap.h *.h
	${CC} ${CF} -c hashmap.c

clean:
	rm *.o

run:
	./libsmallr
