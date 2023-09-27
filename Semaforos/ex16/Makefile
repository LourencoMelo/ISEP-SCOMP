run: east_car west_car

east_car:  east_car.o
		gcc -Wall -g  east_car.o -o east_car -lrt -lpthread

east_car.o: east_car.c
		gcc -Wall -g -c east_car.c -o east_car.o -lrt -lpthread

west_car:  west_car.o
		gcc -Wall -g  west_car.o -o west_car -lrt -lpthread

west_car.o: west_car.c
		gcc -Wall -g -c west_car.c -o west_car.o -lrt -lpthread

clear:
		rm *.o