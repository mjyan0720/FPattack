all:
	gcc -m64 -O3 -o victim.o victim.c
	gcc -m64 -O3 -o attack.o attack.c
