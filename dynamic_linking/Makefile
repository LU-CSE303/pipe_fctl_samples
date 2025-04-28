main: main.c vector.h libvector
	gcc -Wall -o main main.c -ldl
libvector: vector.c
	gcc -Wall -shared -fpic -o libvector.so vector.c -ldl
clean:
	rm main
	rm libvector.so