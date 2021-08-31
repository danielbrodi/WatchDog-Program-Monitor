#COMPILER
CC		= gcc
CFLAGS	+= -Ischeduler/include -L. -Wl,-rpath=. -ansi -pedantic-errors -Wall -Wextra -lrt -pthread -fPIC
LDFLAGS:= -shared


debug: CFLAGS+=-g
debug: all

release: CFLAGS+=-DNDEBUG
release: all

all: user_app

user_app:  user_app.c libwd.so
	$(CC) $(CFLAGS)  user_app.c -lds -lwdint -lwd -o $@ -pthread
	
libwd.so: libwdint.so watchdog.o
	$(CC) $(LDFLAGS) $(CFLAGS) watchdog.o -lds -lwdint -o $@
	
wd_internal.o: wd_internal.c
	$(CC) $(CFLAGS) $^	-c
	
libwdint.so: wd_internal.o
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -lds -o $@

watchdog: libwdint.so watchdog_main.c
	$(CC) $(CFLAGS) watchdog_main.c -lwdint -lds -o $@

watchdog.o: watchdog watchdog.c
	$(CC) $(CFLAGS) watchdog.c	-c

	
#CLEAN
clean:
	rm -f wd_internal.o libwdint.so watchdog libwd.so a.out watchdog.o user_app.o
	

