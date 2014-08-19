all: only
	make -C ../4.1-book-order/ only
	make -C ../2.6-directories/ only
	make -C ../2.4-string-list/ only
	make -C ../2.3-strings/ only
	make -C ../2.2-unit-tests/ only
	make -C ../2.1-logging/	only

only: ../bin/copy_files
	cd .. && bin/copy_files

doc:
	../bin/build_markdown

clean:
	make -C ../4.1-book-order/ clean
	make -C ../2.6-directories/ clean
	rm *.o copy_files

../bin/copy_files: copy_files
	mkdir -p ../bin
	cp copy_files ../bin/copy_files

copy_files: copy_files.o ../4.1-book-order/book_order.o ../2.6-directories/dir.o ../2.4-string-list/str_lst.o ../2.3-strings/str.o ../2.2-unit-tests/unit.o ../2.1-logging/log.o
