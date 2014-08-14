all: only
	make -C ../2.1-logging only

only: test
	./test

doc:
	../bin/build_markdown

clean:
	make -C ../2.1-logging clean
	rm *.o test

test: test.o unit.o ../2.1-logging/log.o
