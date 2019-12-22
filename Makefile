drbdstats : drbdstats.o
	        cc -o drbdstats drbdstats.o

drbdstats.o : drbdstats.c
	        cc -c drbdstats.c
clean :
	        rm drbdstats drbdstats.o
install : drbdstats
	install -m 6755 drbdstats /usr/bin/
