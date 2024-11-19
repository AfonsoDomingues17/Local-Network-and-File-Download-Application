CC=gcc
CFLAGS=-Wall

download: download.c
	$(CC) $(CFLAGS) download.c -o download
	
run: download
	./download ftp://mirrors.up.pt/debian/README.html

clean:
	rm download
	rm README.html
