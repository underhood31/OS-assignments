default: app

context.o: context.s
	gcc -m32 -c context.s -g

thread.o: thread.c
	gcc -Werror -m32 -O3 -c thread.c -g

app: app.c thread.o context.o
	gcc -O3 -m32 thread.o context.o -o app app.c -g

run: app
	./app

test: app
	./app 1024
	./app 1024 1

clean:
	rm -f thread.o app context.o
