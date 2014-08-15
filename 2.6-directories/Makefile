all: only
	make -C ../2.1-logging/	only
	make -C ../2.3-strings/ only
	make -C ../2.4-string-list/ only

only: test
	./test

doc:
	../bin/build_markdown

clean:
	make -C ../2.1-logging/ clean
	make -C ../2.3-strings/ clean
	make -C ../2.4-string-list/ clean
	rm *.o test ../bin/build_markdown

test: test.o dir.o ../2.4-string-list/str_lst.o ../2.3-strings/str.o ../2.2-unit-tests/unit.o ../2.1-logging/log.o
