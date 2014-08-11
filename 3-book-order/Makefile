all: test
	make -C ../2.1-logging/	
	./test

clean:
	make -C ../2.1-logging/ clean
	rm *.o test

test: test.o book_order.o ../2.1-logging/log.o
