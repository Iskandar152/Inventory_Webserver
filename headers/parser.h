#pragma once

#include "httpstructure.h"
#include<iostream>
#include<stdlib.h>
#include<string>
#include<sstream>
#include "def_requ.h"

class Parser{
    public:
        HTTP_Header parseHTTP(char *message);
};