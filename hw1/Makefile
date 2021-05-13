all:
	gcc -o sentences sentences.c

clean:
	rm sentences

grind: all
	@echo "Hello. This is sample input." | valgrind --leak-check=full --track-origins=yes ./sentences