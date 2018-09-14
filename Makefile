all:
	gcc -msse2 -O3 -o victim.o victim.c
	gcc -msse2 -O3 -o attack.o attack.c
