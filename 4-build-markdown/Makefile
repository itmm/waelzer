all: only
	make -C ../2.1-logging/	only
	make -C ../2.3-strings/ only
	make -C ../2.5-string-list/ only

only: build_markdown
	./build_markdown

clean:
	make -C ../2.1-logging/ clean
	make -C ../2.3-strings/ clean
	make -C ../2.5-string-list/ clean
	rm *.o build_markdown

build_markdown: build_markdown.o ../2.1-logging/log.o ../2.3-strings/str.o ../2.5-string-list/str_lst.o
