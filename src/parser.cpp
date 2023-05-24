#include "parser.h"

HTTP_Header Parser::parseHTTP(char *message){
    HTTP_Header header;
    char request[MAX_BUFFER_SIZE];
    
    //Only reading the first line until we hit /r
    //line saved in request[], to be parsed separately
    int i = 0;
    while(message[i] != '\r'){
        request[i] = message[i];
        i++;
    }
    request[i+1] = '\0';

    std::stringstream stream(request);
    stream>>header.request_type>>header.directory>>header.version;

    return header;
}