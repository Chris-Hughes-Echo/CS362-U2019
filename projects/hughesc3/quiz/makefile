CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme: testme.c
	gcc -o runtest testme.c -g $(CFLAGS)
	./runtest

clean:
	rm -f runtest *.gcov *.gcda *.gcno
