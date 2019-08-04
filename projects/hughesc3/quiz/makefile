CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme: testme.c
	gcc -o runtest testme.c -g $(CFLAGS)
	echo "Running Test..." > results.out
	./runtest >> results.out
	gcov testme.c -b -f >> results.out

clean:
	rm -f runtest results.out *.gcov *.gcda *.gcno
