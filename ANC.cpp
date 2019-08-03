#include"ANC.h"
#include<iostream>

ANC::ANC()
{

}
ANC::~ANC(){

}

char* ANC::UpaddHeader(char* str, int len){//上行加上sc2头

	char* ret = new char[len + 8];
	/*前5比特是UEID*/
	*(ret+5) = 0x00;
	short l = htons(len);
	memcpy(ret+6,(char*)&l,2);
	memcpy(ret+8,str,len);

	return ret;

}

char* ANC::DownaddHeader(char* str, int len){//下行加上sc2头

	char* ret = new char[len + 8];
	/*前5比特是UEID*/
	*(ret+5) = 0x01;
	short l = htons(len);
	memcpy(ret+6,(char*)&l,2);
	memcpy(ret+8,str,len);

	return ret;
}
