all: chuser
	
chuser: chuser.c
	$(CC) -g -o chuser chuser.c

install: chuser
	cp -f chuser /usr/local/bin/
