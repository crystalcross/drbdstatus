CC=cc
CFLAGS=-g
DEPS=

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

drbdstats: drbdstats.o drbdgetstat.o libutils.o
	$(CC) -o $@ $^ $(CFLAGS)

testproc : test.o libutils.o libll.o
	$(CC) -o $@ $^ $(CFLAGS)
clean :
	        rm -f drbdstats testproc *.o
install : drbdstats
	install -m 6755 drbdstats /usr/bin/
	install -m 755 php/drbdinfo.inc /var/www/html/
