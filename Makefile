all: acm110 acm259

acm110: acm110.c
	$(CC) -o acm110 acm110.c

acm259: acm259.c
	$(CC) -o acm259 acm259.c

test:
	./acm110 < input110.txt > 110.txt
	./acm259 < input259.txt > 259.txt
	diff 110.txt output110.txt && diff 259.txt output259.txt

clean:
	rm acm110 110.txt acm259 259.txt
