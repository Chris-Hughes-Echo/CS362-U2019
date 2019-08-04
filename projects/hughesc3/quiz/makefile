CFLAGS= -Wall -g -coverage -std=c99

testme: testme.c
	gcc -o runtest testme.c $(CFLAGS)
	./runtest

clean:
	rm -f runtest *.gcov *.gcda *.gcno
