run: seller client

seller:  seller.o
		gcc -Wall -g  seller.o -o seller -lrt -lpthread

seller.o: seller.c
		gcc -Wall -g -c seller.c -o seller.o -lrt -lpthread

client:  client.o
		gcc -Wall -g  client.o -o client -lrt -lpthread

client.o: client.c
		gcc -Wall -g -c client.c -o client.o -lrt -lpthread

clear:
		rm *.o