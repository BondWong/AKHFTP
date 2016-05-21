CC = gcc
CFLAGS = -g -Wall -W -O2 -c # debugging, compiler warnings, warning, optimize, object file

ERROR_HANDLING_C = ../../common/error_handling.c
ERROR_HANDLING_O = error_handling.o

SERVER_C = file_server.c
CLIENT_C = file_client.c

SERVER_O = file_server.o
CLIENT_O = file_client.o

SERVER = server
CLIENT = client

IP = 127.0.0.1
PORT = 9190

link: compile
	$(CC) $(SERVER_O) $(ERROR_HANDLING_O) -o $(SERVER)
	$(CC) $(CLIENT_O) $(ERROR_HANDLING_O) -o $(CLIENT)

compile: error_handling.o $(SERVER_C) $(CLIENT_C)
	$(CC) $(CFLAGS) $(SERVER_C)
	$(CC) $(CFLAGS) $(CLIENT_C)

run_server: link
	./$(SERVER) $(PORT)

run_client: link
	./$(CLIENT) $(IP) $(PORT)

error_handling.o: $(ERROR_HANDLING_C)
	$(CC) $(CFLAGS) $(ERROR_HANDLING_C)

clean:
	rm -rf *.o
	rm -rf *.gch
	rm -rf $(SERVER) $(CLIENT)
	rm -rf *.dat

