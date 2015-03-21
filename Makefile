all: acm110

acm110: acm110.c
	gcc -o acm110 acm110.c

test:
	./acm110 < input110.txt > res110.txt
	diff res110.txt output110.txt
