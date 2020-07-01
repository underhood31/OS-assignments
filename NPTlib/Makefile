default: app race1 race2 leak

context.o: context.s
	gcc -m32 -c context.s -g

thread.o: thread.c
	gcc -Werror -m32 -O3 -c thread.c -g

app: app.c thread.o context.o
	gcc -O3 -m32 thread.o context.o -o app app.c -g

race1: race1.c thread.o context.o
	gcc -O3 -m32 thread.o context.o -o race1 race1.c -g

race2: race2.c thread.o context.o
	gcc -O3 -m32 thread.o context.o -o race2 race2.c -g

leak: leak.c thread.o context.o
	gcc -O3 -m32 thread.o context.o -o leak leak.c -g

run: app
	./app

test: app
	./app 1024
	./app 1024 1

test2:
	/usr/bin/time -v ./leak 1024000 2>&1 |egrep "kbytes|counter"

clean:
	rm -f thread.o app context.o race1 race2 leak
