a.out: main.o dictionary.o binTree.o data.o
	gcc -std=c99 main.o dictionary.o data.o
main.o: main.c
	gcc -c -std=c99 main.c
dictionary.o: dictionary.c dictionary.h
	gcc -c -std=c99 dictionary.c
binTree.o: binTree.c binTree.h
    gcc -c -std=c99 binTree.c
 data.o: data.c data.h
     gcc -c -std=c99 data.c
clean:
	rm -f main.o dictionary.o binTree.o a.out
run: a.out
	./a.out
