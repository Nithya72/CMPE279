// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>

#define PORT 80

int goto_method(int new_socket_no);

int main(int argc, char const *argv[])
{

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";


    printf("execve=0x%p\n", execve);

    // if(argc > 1){
    if(strcmp(argv[0], "CHILD") == 0){
        printf("new exec'ed child process \n");
        int new_socket_no = atoi(argv[1]);
        goto_method(new_socket_no);
    }
    else{

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        // Attaching socket to port 80
        // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
        //                                               &opt, sizeof(opt)))
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                                                      &opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( PORT );

        // Forcefully attaching socket to the port 80
        if (bind(server_fd, (struct sockaddr *)&address,
                                     sizeof(address))<0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        pid_t p_id=fork();
        int status = 0;

        //printf("p_id: %d\n", p_id);

        if(p_id < 0){
            perror("fork failed!\n");
            exit(EXIT_FAILURE);
        }
        else if(p_id == 0){

            struct passwd * nobodyInfo;
            
            //getting the userID for the nobody user
            nobodyInfo = getpwnam("nobody");
            // printf("nobody id: %d", nobodyInfo->pw_uid);

            if(setuid(nobodyInfo->pw_uid) == -1){
                perror("privilege drop failed!\n");
                exit(EXIT_FAILURE);
            }

            printf("privilege drop success\n");

            char fd_string[10];
            snprintf(fd_string,10,"%d",new_socket);            
            char *argv1[] = {"CHILD", fd_string, NULL};

            int exec_status = execvp("./server", argv1);
            if(exec_status<0){
                perror("child re-exec failed!\n");
                exit(EXIT_FAILURE);
            }
        }
        else{
            printf("waiting for child to exit\n");
            waitpid(p_id, &status, 0);
            exit(0);
            printf("child process exited\n");
        }
    }

    return 0;
}

int goto_method(int new_socket_no){

    char buffer[1024] = {0};
    char *hello = "Hello from server";

    int valread = read( new_socket_no , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket_no , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");

    return 0;
}
