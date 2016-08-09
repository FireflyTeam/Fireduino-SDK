 /**
 * @file TF.cpp
 * @brief Definitions TF class for Fireduino
 * @author jiang<jdz@t-chip.com.cn> 
 * @version V1.0
 * @date 2016.02
 * 
 * @par Copyright:
 * Copyright (c) 2016 T-CHIP INTELLIGENCE TECHNOLOGY CO.,LTD. \n\n
 *
 * For more information, please visit website <http://www.t-firefly.com/>, \n\n
 * or email to <service@t-firefly.com>.
 */ 
#include "TF.h"
#include "ard_sd.h"


LFile::LFile()
{
//	_fd = 0;
	_mode = 0;
	HDC = 0;
}

LFile::~LFile()
{
	_mode = 0;
	HDC = 0;
//	_is_dir = 0;
}


LFile::LFile(void *name, unsigned char mode )
{
	_mode = mode;

//	HDC = ard_sd_open((char*)name,mode,&_is_dir);
	HDC = ard_sd_open((char*)name,mode,NULL);
}

int LFile::read()
{
	int temp = 0;
	if(read(&temp,1) == 1)
	{
		  return temp;
	}
	else
	{
		  return -1;
	}
}

int LFile::peek()
{
	unsigned int cur = position();
	int c = read();
	if(c != -1)
	{
		seek(cur);
		return c;
	}
    return 0;
}

int LFile::available()
{
	return size() - position();
}

void LFile::flush()
{
	ard_sd_flush(HDC);
}

int LFile::read(void *buf, unsigned int nbyte)
{
	return ard_sd_read(HDC,buf,nbyte);
}

unsigned int LFile::write(unsigned char val) {
  return write(&val, 1);
}

unsigned int LFile::write(unsigned char *buf, unsigned int size) {
	if(_mode & 0x01)
	{
		return ard_sd_write(HDC,buf,size);
	}
	else
	{
		return 0;
	}
}

unsigned char LFile::seek(unsigned int pos)
{

    return ard_sd_seek(HDC,pos);
}

uint32_t LFile::position()
{
    return ard_sd_position(HDC);
}

uint32_t LFile::size()
{
    return ard_sd_size(HDC);
}

void LFile::close()
{
	ard_sd_close(HDC);
	HDC = 0;
}

LFile::operator bool() {
	if(HDC <= 0 )
		return false;
	else
		return true;
}



unsigned char TFDrive::begin(void)
{
	if(ard_sd_init() == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
extern "C" unsigned int  Ascii2Unicode(unsigned char * pbAscii, unsigned short * pwUnicode, unsigned int len);
LFile TFDrive::open(char *filename,uint8_t mode)
{
	unsigned short path_t[256];

	Ascii2Unicode((unsigned char *)filename,(unsigned short *)path_t,strlen(filename)*2);
	path_t[strlen(filename)] = 0x0000;
	return LFile((void *)path_t,mode);
}

LFile TFDrive::open(wchar_t *filename,uint8_t mode)
{
	return LFile((void *)filename,mode);
}

unsigned char TFDrive::exists(char *filepath)
{
	unsigned short path_t[256];
	Ascii2Unicode((unsigned char *)filepath,(unsigned short *)path_t,strlen(filepath)*2);
	path_t[strlen(filepath)] = 0x0000;

	return ard_sd_exists((char*)path_t);
}

unsigned char TFDrive::mkdir(char *filepath)
{
	unsigned short path_t[256];
	Ascii2Unicode((unsigned char *)filepath,(unsigned short *)path_t,strlen(filepath)*2);
	path_t[strlen(filepath)] = 0x0000;

	return ard_sd_mkdir((char*)path_t);
}

unsigned char TFDrive::remove(char *filepath)
{
	unsigned short path_t[256];
	Ascii2Unicode((unsigned char *)filepath,(unsigned short *)path_t,strlen(filepath)*2);
	path_t[strlen(filepath)] = 0x0000;

	return ard_sd_remove((char*)path_t);
}

unsigned char TFDrive::rmdir(char *filepath)
{
	unsigned short path_t[256];
	Ascii2Unicode((unsigned char *)filepath,(unsigned short *)path_t,strlen(filepath)*2);
	path_t[strlen(filepath)] = 0x0000;

	return ard_sd_rmdir((char*)path_t);
}

unsigned char TFDrive::scandir(const char *filepath,
		unsigned char (*callback)(const char *filename, int isFile),
		bool onlyScanFiles)
{

	return ard_sd_scandir(filepath, callback, onlyScanFiles ? 1:0);
}


TFDrive TF;

