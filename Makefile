CC = g++

all:
	$(CC) ./src/serv.cpp -o serv && $(CC) ./src/client.cpp -o cli