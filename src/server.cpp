#include "server.h"

/*
String Concatination researched from:https://stackoverflow.com/questions/8465006/how-do-i-concatenate-two-strings-in-c
*/
char* concat(char *string1, char *string2){
    char *new_string = (char*)malloc(strlen(string1) + strlen(string2) + 1);
    strcpy(new_string, string1);
    strcat(new_string,string2);
    return new_string;
}

TCP_Server::TCP_Server(int port){
    TCP_Server::start_TCP(port);
    TCP_Server::listen_for_connections(&listenfd);
}
TCP_Server::~TCP_Server(){

}

int TCP_Server::start_TCP(int port)
{
    //Creating TCP Server
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if(listenfd < 0){
        TCP_Server::handle_error("Unable to open socket");
        return -1;
    }

    //Binding Exception
    int optval = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

    //Prepare Socket Address
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    //Bind to the socket 
    bind_val = bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    if(bind_val < 0){
        TCP_Server::handle_error("Unable to bind to socket");
        return -1;
    }

    listen_val = listen(listenfd, 1);
    printf("Server is running, awaiting connections...\n");
    return 0;
}


int TCP_Server::listen_for_connections(int *listenfd){
    for(; ;){
        client_len = sizeof(client_addr);
        connfd = accept(*listenfd, (struct sockaddr *) &client_addr, &client_len);
        printf("Received Request From: %d\n",connfd);
        
        int rec_amount = recv(connfd, buf, MAX_BUFFER_SIZE, 0);
        if(rec_amount < 1){
            TCP_Server::handle_error("Read Error from client");
        }
        
        logger.save_http_request(buf);
        //std::cout << buf << std::endl;
        TCP_Server::parse_HTTP(buf);
        bzero(buf, MAX_BUFFER_SIZE);

        printf("Closing Connection with Client: %d\n", connfd);
        close(connfd);
    }

    return 0;
}

int TCP_Server::parse_HTTP(char *message){
    HTTP_Header header = parser.parseHTTP(message);


    if(header.request_type == ""){
        TCP_Server::handle_error("404 Invalid Request Received: Improper request type");
        return -1;
    }
    else if(header.directory == ""){
        TCP_Server::handle_error("404 Invalid Request Received: Improper directory");
        return -1;
    }
    else if(header.version == ""){
        TCP_Server::handle_error("404 Invalid Request Received: Improper version");
        return -1;
    }
    std::cout << header.request_type << std::endl;
    if(header.request_type.compare(0,4,"POST")==0){
        std::cout << "What the hell!" << std::endl;
        return -1;
    }
    if(header.request_type.compare(0,3,"GET")){
        TCP_Server::handle_error("405 Invalid Request: Not a GET");
        return -1;
    }
    else if(header.version.compare(0,8,"HTTP/1.1")){
        TCP_Server::handle_error("405 Invalid Request: Wrong Version");
        return -1;
    }

    std::string path = path_finder.find_path(header.directory);
    TCP_Server::send_file(path,header.version);
    return 0;
}


void TCP_Server::handle_error(std::string error){
    std::cout << "ERROR: " << error << std::endl;
    //really need a timestamp for errors...
    //and more details..
    logger.log_to_file(error,"error_log");
}

int TCP_Server::send_file(std::string path, std::string version){
    char* new_path = (char*)path.c_str();
    char* type = TCP_Server::file_type(new_path);
    int file_size = 0;
    char *header;
    char buff[MAX_BUFFER_SIZE];
    char temp_char[4];
    if(access(path.c_str(),F_OK)==0){
        
    }
    else{
        TCP_Server::handle_error("File path " + path + " doesn't exist");
        return -1;
    }

    //Finding the file size
    FILE *file;
    file = fopen(path.c_str(),"rb");
    fseek(file, 0L, SEEK_END);
    file_size = ftell(file);
    rewind(file);
    //uhh...
    sprintf(temp_char,"%d",file_size);
    header = concat((char*)version.c_str(), (char*)" 200 OK\r\nContent-Type:");
    header = concat(header,type);
    header = concat(header, (char*)"\r\nContent-Length:");
    header = concat(header,temp_char);
    header = concat(header, (char*)"\r\n\r\n");

    int num_reads_required = 0;
    int leftover = file_size % MAX_BUFFER_SIZE;
    num_reads_required = (int)((float)file_size / MAX_BUFFER_SIZE);
    
    send(connfd, header, strlen(header), 0);
    for(int i = 0; i < num_reads_required; i++){
        fread(buff, MAX_BUFFER_SIZE, 1, file);
        send(connfd,buff, MAX_BUFFER_SIZE,0);
        
    }
    fread(buff,leftover,1,file);
    send(connfd,buff, leftover,0);

    fclose(file);
    return 0;
}

char* TCP_Server::file_type(char* path){
    int size = strlen(path);
    int index, type_size;
    char *type;
    char *return_type;
    for(int i = size - 1; i > 0; i--){
        if(path[i] == '.'){
            index = i;
        }
    }

    type_size = size - (index);
    type = (char*)malloc(type_size);

    for(int i = 0; i < type_size; i++){
        type[i] = path[index + i];
    }
    

    if(!strncmp(type,".html",4)){
        return_type = (char*)"text/html";
    }
    else if(!strncmp(type,".txt",3)){
        return_type = (char*)"text/plain";
    }
    else if(!strncmp(type,".png",3)){
        return_type = (char*)"image/png";
    }
    else if(!strncmp(type,".gif",3)){
        return_type = (char*)"image/gif"; 
    }
    else if(!strncmp(type,".jpg",3)){
        return_type = (char*)"image/jpg";
    }
    else if(!strncmp(type,".css",3)){
        return_type = (char*)"text/css";
    }
    else if(!strncmp(type,".js",2)){
        return_type = (char*)"application/javascript";
    }
    else if(!strncmp(type,".ico",3)){
        return_type = (char*)"image/x-icon";
    }
    else{
        return (char*)"application/octet-stream";
    }

    return return_type;
    
}

int TCP_Server::receive_data(){

}