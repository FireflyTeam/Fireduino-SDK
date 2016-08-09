 /**
 * @file TF.h
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
#ifndef _ARDUINO_TF_H_
#define _ARDUINO_TF_H_

#include "wirish_constants.h"
//#include <stdio.h>
//#include <stdint.h>
#include "Stream.h"

#define FILE_READ 0
#define FILE_WRITE 1

#define File	LFile
#define SD		TF
#define __SD_H__

class LFile : public Stream
{

public:
    LFile(void *name, unsigned char mode ); // wraps an underlying SdFile
    LFile(void);                           // 'empty' constructor
    ~LFile(void);                          // destructor

public:

    virtual unsigned int write(unsigned char val);

    virtual unsigned int write(unsigned char *buf, unsigned int size);

    virtual int read() ;

    virtual int peek() ;

    virtual int available() ;

    virtual void flush() ;

    int read(void *buf,unsigned int nbyte);

    unsigned char seek(unsigned int pos);

    uint32_t position();

    uint32_t size();

    void close();

    operator bool();


//    using Print::write;


public:
    unsigned char _mode;
//    unsigned char _is_dir;
//    unsigned int _fd;
    void * HDC;
};



class TFDrive
{

public:

	unsigned char begin(void);

    LFile open(char *filename,uint8_t mode = FILE_READ);

    LFile open(wchar_t *filename,uint8_t mode = FILE_READ);

    unsigned char exists(char *filepath);

    unsigned char mkdir(char *filepath);

    unsigned char remove(char *filepath);

    unsigned char rmdir(char *filepath);

    unsigned char scandir(const char *filepath,
    		unsigned char (*callback)(const char *filename, int isFile),
			bool onlyScanFiles=false);
};

extern TFDrive TF;


#endif

