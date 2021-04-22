# CMPE279

Course Name: Software Security Technologies (Spring 2021) <br>
Student Name: Nithya Anbalagan  <br>
Student Id: 014608321  <br>

## Assignment 1 and Assignment 2


#### The following changes were made in the client and server files to run on mac terminal

#### client.c - Added two additional imports
#include <arpa/inet.h><br>
#include <unistd.h>

#### server.c - replaced the below line
   // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
   
 #### with
     if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
removed (SO_REUSEPORT)

#### Steps used to run the files:

#### Server terminal: 
1) sudo gcc server.c -o server
2) sudo ./server

#### Client terminal: 
1) gcc client.c -o client
2) ./client
