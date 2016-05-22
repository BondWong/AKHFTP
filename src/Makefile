CC = gcc
CFLAGS = -g -Wall -W -O2 -c # debugging, compiler warnings, warning, optimize, object file

MESSAGE_UTIL = message_util
ERROR_HANDLING = error_handling

AKH_SERVER_MAIN = akh_server_main
AKH_CLIENT_MAIN = akh_client_main

FILES = $(MESSAGE_UTIL).c $(ERROR_HANDLING).c $(AKH_SERVER_MAIN).c $(AKH_CLIENT_MAIN).c
OBJ =  $(MESSAGE_UTIL).o $(ERROR_HANDLING).o

SERVER = server
CLIENT = client

IP = 127.0.0.1
PORT = 9190


compile: $(FILES) 
	$(CC) $(CFLAGS) $(FILES)

server: $(OBJ) $(AKH_SERVER_MAIN).o
	$(CC) $(CLIENT_O) $(OBJ) $(AKH_SERVER_MAIN).o -o $(SERVER)

client: $(OBJ) $(AKH_CLIENT_MAIN).o
	$(CC) $(CLIENT_O) $(OBJ) $(AKH_CLIENT_MAIN).o -o $(CLIENT)

run_server: $(SERVER)
	./$(SERVER) $(PORT)

run_client: $(CLIENT)
	./$(CLIENT) $(IP) $(PORT)

clean:
	rm -rf *.o
	rm -rf *.gch
	rm -rf $(SERVER) $(CLIENT)

