#pragma once

#include<string>
#include<stdlib.h>
#include<iostream>
#include<fstream>

class Logger{
    public:
        Logger();
        ~Logger();

        int save_http_request(char* request);
        int log_to_file(std::string request, std::string filename);
    private:
};