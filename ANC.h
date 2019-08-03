#pragma once
#ifndef ANC_H
#define ANC_H

#include <arpa/inet.h>
#include <memory.h>
class ANC
{
     public:
            ANC();
           ~ANC();
	   char * UpaddHeader(char* str, int len);
	   char * DownaddHeader(char* str, int len);
};
#endif
