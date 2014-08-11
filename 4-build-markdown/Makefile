all: only
	make -C ../2.1-logging/	

only: build_markdown
	./build_markdown

clean:
	make -C ../2.1-logging/ clean
	rm *.o build_markdown

build_markdown: build_markdown.o ../2.1-logging/log.o 
