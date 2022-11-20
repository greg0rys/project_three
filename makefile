CC = g++
CPPFLAGS = -std=c++11 -g -Wall
DEPS = driver.h website.h table.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS)

app: driver.o website.o table.o
	$(CC) -o app driver.o website.o table.o

.PHONY: clean
clean:
	$(info -- cleaning up! --)
	rm -fv *.o
	rm -fv app

