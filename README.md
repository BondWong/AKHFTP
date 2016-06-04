# AKHFTP
This is a UDP based protocol that works like FTP. Using UDP makes it faster and efficient.

Please refer to this article (https://docs.google.com/document/d/1eSPbz-AIAC2w8Gre_zjATTk9AO13b1Q-CgkciSAc614/edit?usp=sharing) to check out more info.


## Environment
Linux 64 bit

## Language
C

## How to compile
### compile server
type 'make server'
it generates excutable file 'server' for server side under AKHFTP/src/server_dir
The server binds to port 9190 (default). The port number can be changed in Makefile.

### compile client
type 'make client'
it generates excutable file 'client for client side under AKHFTP/src/client_dir
The default host IP address is 127.0.0.1 and default port number is 9190. The host IP address and port number can be changed in Makefile.

## How to run
### run server
type 'make run_server'
it runs server side program

### test download
type 'make run_client_download'
it request download file 'server.txt' to server. The file is stored under AKHFTP/src/client_dir

### test upload
type 'make run_client_upload'
It request upload file 'client.txt' to server. The server stores the file under AKHFTP/src/server_dir

### test upload reject
Fo the test case, we made limitation of server application. If client requests upload file greater than 1KB, server rejects the requestion. The real program only rejects upload request when it does not have enough space.
To test that, type 'make run_client_upload2'
After client requests upload, it gets back deny message from server because the file size is greater than 1 KB.

## Extra Credit:
We did not implement extra credit task.
