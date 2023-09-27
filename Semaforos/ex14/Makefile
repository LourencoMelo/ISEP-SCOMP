run: writer reader

writer:  writer.o
		gcc -Wall -g  writer.o -o writer -lrt -lpthread

writer.o: writer.c
		gcc -Wall -g -c writer.c -o writer.o -lrt -lpthread

reader:  reader.o
		gcc -Wall -g  reader.o -o reader -lrt -lpthread

reader.o: reader.c
		gcc -Wall -g -c reader.c -o reader.o -lrt -lpthread

clear:
		rm *.o