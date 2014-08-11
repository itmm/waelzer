all: only
	make -C ../2.2-unit-tests/
	make -C ../2.1-logging/	

only: test
	./test

clean:
	make -C ../2.1-logging/ clean
	rm *.o test

test: test.o book_order.o ../2.2-unit-tests/unit.o ../2.1-logging/log.o
