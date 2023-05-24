#include "logger.h"

Logger::Logger(){

}
Logger::~Logger(){

}

int Logger::save_http_request(char *request)
{
    std::ofstream saveFile;
    std::cout << "Logging" << std::endl;
    saveFile.open("log_files/http_requests", std::ios::out | std::ios::app);
    saveFile << request << "\n";
    saveFile.close();

    return 0;
}

int Logger::log_to_file(std::string request, std::string filename){
    std::string directory = "log_files/" + filename;
    std::ofstream saveFile;
    std::cout << "Logging to " << filename << std::endl;
    saveFile.open(directory, std::ios::out | std::ios::app);
    saveFile << request << "\n";
    saveFile.close();

    return 0;
}
