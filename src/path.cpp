#include "path.h"

std::string Path_Finder::find_path(std::string directory){
    std::string path;
    int size = directory.length();
    
    if(size == 1 && directory[0] == '/'){
        path = "www/index.html";
        return path;
    }

    path = "www" + directory;
    return path;

}