a.out: main.o dictionary.o binTree.o entry.o
	gcc -std=c99 main.o dictionary.o binTree.o entry.o
main.o: main.c
	gcc -c -std=c99 main.c
dictionary.o: dictionary.c dictionary.h
	gcc -c -std=c99 dictionary.c
binTree.o: binTree.c binTree.h
	gcc -c -std=c99 binTree.c
entry.o: entry.c entry.h
	gcc -c -std=c99 entry.c
clean:
	rm -f *.o a.out
run: a.out
	./a.out
