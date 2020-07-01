SRC=$(wildcard *.c)
DST=$(patsubst %.c,%,$(SRC))

default: $(DST)

$(DST): $(SRC) usys.S
	gcc -o $@ $^ -g

clean:
	rm -rf $(DST)
