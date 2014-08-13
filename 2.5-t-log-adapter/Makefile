all: only
	make -C ../2.3-strings/ only
	make -C ../2.2-unit-tests/ only
	make -C ../2.1-logging/ only

only: test
	./test

clean:
	make -C ../2.3-strings/ clean
	rm *.o test

test: test.o t_logger.o ../2.3-strings/str.o ../2.2-unit-tests/unit.o ../2.1-logging/log.o
