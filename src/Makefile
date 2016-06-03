CC = gcc

CFLAGS = -g -Wall -W -O2 -c -I /usr/local/opt/openssl/include # debugging, compiler warnings, warning, optimize, object file

LDFLAGS = -lssl -lcrypto -L /usr/local/opt/openssl/lib # link the ssl library, link the crypto library, library location

ERROR_HANDLING = error_handling
NETWORK_UTIL = network_util
FILE_UTIL = file_util
MESSAGE_UTIL = message_util
SECURITY_UTIL = security_util
CONNECTION = connection
FILE_TRANSMISSION = file_transmission
DISCONNECTION = disconnection
BUFFER_UTIL = buffer_util
MAP_UTIL = map_util

AKH_SERVER_MAIN = akh_server_main
AKH_CLIENT_MAIN = akh_client_main

FILES = $(ERROR_HANDLING).c $(FILE_UTIL).c $(MESSAGE_UTIL).c $(SECURITY_UTIL).c $(CONNECTION).c $(FILE_TRANSMISSION).c $(DISCONNECTION).c $(AKH_SERVER_MAIN).c $(AKH_CLIENT_MAIN).c $(NETWORK_UTIL).c $(BUFFER_UTIL).c $(MAP_UTIL).c
OBJ =  $(ERROR_HANDLING).o $(FILE_UTIL).o $(MESSAGE_UTIL).o $(SECURITY_UTIL).o $(CONNECTION).o $(FILE_TRANSMISSION).o $(DISCONNECTION).o $(NETWORK_UTIL).o $(BUFFER_UTIL).o $(MAP_UTIL).o

SERVER = server
CLIENT = client

IP = 127.0.0.1 # The IP address of the server
#/**************** SERVICE ************/#
PORT = 9190  # The hard coded port number
DOWNLOAD = -d
UPLOAD = -u
UFILENAME = client.txt # name of the upload file
UFILENAME2 = War_and_Peace 
DFILENAME = server.txt # name of the download file


compile: $(FILES) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(FILES)

server: $(OBJ) $(AKH_SERVER_MAIN).o
	$(CC) $(CLIENT_O) $(OBJ) $(AKH_SERVER_MAIN).o -o ./server_dir/$(SERVER) $(LDFLAGS)

client: $(OBJ) $(AKH_CLIENT_MAIN).o
	$(CC) $(CLIENT_O) $(OBJ) $(AKH_CLIENT_MAIN).o -o ./client_dir/$(CLIENT) $(LDFLAGS)

run_server: $(SERVER)
	cd ./server_dir && ./$(SERVER) $(PORT)

run_client_download: $(CLIENT)
	cd ./client_dir && ./$(CLIENT) $(IP) $(PORT) $(DOWNLOAD) $(DFILENAME)

run_client_upload: $(CLIENT)
	cd ./client_dir && ./$(CLIENT) $(IP) $(PORT) $(UPLOAD) $(UFILENAME)

run_client_upload2: $(CLIENT)
	cd ./client_dir && ./$(CLIENT) $(IP) $(PORT) $(UPLOAD) $(UFILENAME2)

clean:
	rm -rf *.o
	rm -rf *.gch
	rm -rf ./server_dir/$(SERVER) ./client_dir/$(CLIENT)
	rm -rf ./server_dir/client.txt ./client_dir/server.txt

bond: compile
	$(CC) $(CLIENT_O) $(OBJ) test_buffer.c -o test $(LDFLAGS)

