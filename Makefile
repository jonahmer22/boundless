all:
	gcc -Wall -Wextra -I ./include ./src/*.c -o boundless

clean:
	rm boundless
