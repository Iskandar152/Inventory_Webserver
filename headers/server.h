#pragma once 

#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string>
#include<iostream>
#include<unistd.h>
#include<cstring>

#include "logger.h"
#include "parser.h"
#include "def_requ.h"
#include "path.h"
class TCP_Server{
    public:
        int listenfd;

        TCP_Server(int port=DEFAULT_PORT);
        ~TCP_Server();

    private:
        Logger logger = Logger();
        Parser parser = Parser();
        Path_Finder path_finder = Path_Finder();
        //Server Variables 
        int bind_val;
        int listen_val;
        sockaddr_in server_addr;

        //Client Variables
        socklen_t client_len;
        sockaddr_in client_addr;
        int connfd;
        char buf[MAX_BUFFER_SIZE];
        
        int start_TCP(int port);
        int listen_for_connections(int *listenfd);
        int parse_HTTP(char *message);
        void handle_error(std::string error);
        int send_file(std::string path, std::string version);
        char* file_type(char* path);
        int receive_data();
};
