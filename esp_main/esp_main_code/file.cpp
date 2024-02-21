#include "file.h"

void readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

void copyFileToEndGrow(fs::FS &fs, const char * pathIn, const char * pathToEnd){
  Serial.printf("Reading file: %s\r\n", pathIn);
  Serial.printf("Reading file: %s\r\n", pathToEnd);
  
  File fileIn = fs.open(pathIn);
  File fileToEnd = fs.open(pathToEnd);
  
  if(!fileIn || fileIn.isDirectory() && !fileToEnd || fileToEnd.isDirectory()){
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.write("File read in\n");
  Serial.write(fileIn.read());
  Serial.write("\nFile read out\n");
  Serial.write(fileToEnd.read());
  fileToEnd.write(fileIn.read());
  Serial.write("\nFile read out after write\n");
  Serial.write(fileToEnd.read());
  Serial.write("\n");
  

  fileIn.close();
  fileToEnd.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\r\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("- failed to open file for appending");
    return;
  }
  if(file.print(message)){
    Serial.println("- message appended");
  } else {
    Serial.println("- append failed");
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}
