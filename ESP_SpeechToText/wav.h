#ifndef __WAV__
#define __WAV__

#include <LittleFS.h> 

#define FILENAME "/recording.wav"
#define BODYNAME "/body.wav"
#define ENCODED_FILENAME "/encoded_recording.wav"
#define HEADER_SIZE 44


class Wav {
private:
    File body, file;
    void wavHeader(uint8_t* header, int wavSize);

public:
    Wav(void);

    void create(void);
    void close(void);
    void remove(void);
    void listLittleFS(void);
    void writeBody(uint8_t* samplesBuff, size_t bytes_read);
    void writeFile(void);

    void encodeFile(void);

    void bodyOpen(char* m);
    void bodyClose(void);
    void fileOpen(char* m);
    void fileClose(void);

};

#endif
