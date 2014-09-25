
test: clean
	$(CC) eloop.c test.c -lpthread -o test
	./test

clean:
	rm -f test
