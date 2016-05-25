CC = gcc

CFLAGS = -g -Wall -W -O2 -c -I /usr/local/opt/openssl/include # debugging, compiler warnings, warning, optimize, object file

LDFLAGS = -lssl -lcrypto -L /usr/local/opt/openssl/lib # link the ssl library, link the crypto library, library location

ERROR_HANDLING = error_handling
FILE_UTIL = file_util
MESSAGE_UTIL = message_util
SECURITY_UTIL = security_util

AKH_SERVER_MAIN = akh_server_main
AKH_CLIENT_MAIN = akh_client_main

FILES = $(ERROR_HANDLING).c $(FILE_UTIL).c $(MESSAGE_UTIL).c $(SECURITY_UTIL).c $(AKH_SERVER_MAIN).c $(AKH_CLIENT_MAIN).c
OBJ =  $(ERROR_HANDLING).o $(FILE_UTIL).o $(MESSAGE_UTIL).o $(SECURITY_UTIL).o

SERVER = server
CLIENT = client

IP = 127.0.0.1
PORT = 9190


compile: $(FILES) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(FILES)

server: $(OBJ) $(AKH_SERVER_MAIN).o
	$(CC) $(CLIENT_O) $(OBJ) $(AKH_SERVER_MAIN).o -o $(SERVER) $(LDFLAGS)

client: $(OBJ) $(AKH_CLIENT_MAIN).o
	$(CC) $(CLIENT_O) $(OBJ) $(AKH_CLIENT_MAIN).o -o $(CLIENT) $(LDFLAGS)

run_server: $(SERVER)
	./$(SERVER) $(PORT)

run_client: $(CLIENT)
	./$(CLIENT) $(IP) $(PORT)

clean:
	rm -rf *.o
	rm -rf *.gch
	rm -rf $(SERVER) $(CLIENT)

