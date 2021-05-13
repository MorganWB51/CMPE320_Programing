all:
	gcc -o part1 part1.c -pthread
	gcc -o part2 part2.c -pthread
	gcc -o part3 part3.c -pthread -lm

test:
	gcc -o demo part3withoutThread.c -pthread -lm

clean:
	rm part1
	rm part2
	rm part3

