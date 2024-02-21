#ifndef __FILE_H__
#define __FILE_H__

#include <LittleFS.h>
#include "FS.h"

void readFile(fs::FS &fs, const char * path);

void copyFileToEndGrow(fs::FS &fs, const char * pathIn, const char * pathToEnd);

void appendFile(fs::FS &fs, const char * path, const char * message);

void writeFile(fs::FS &fs, const char * path, const char * message);


#endif
