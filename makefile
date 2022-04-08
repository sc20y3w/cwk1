main:main.c interface.o book.o 
	gcc mian.c interface.o -o main
	
UI.o:UI.c
	gcc -c UI.c
	
book.o: book.c
	gcc -c book.c
	
clean:
	rm *.o main