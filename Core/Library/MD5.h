﻿#ifndef MD5_H
#define MD5_H

#include <stdint.h>
#include <string>
#include <fstream>
#include <QString>

/* Type define */
typedef unsigned char byte;
using std::string;
using std::ifstream;

class MD5
{
public:
    MD5();
    MD5(const void *input, size_t length);
    MD5(const string &str);
    MD5(ifstream &in);
    void calc(const char *inStr, size_t inStrLen, char* outStr);

private:
    void update(const byte *input, size_t length);
    void final();
    void transform(const byte block[64]);
    void encode(const quint32 *input, byte *output, size_t length);
    void decode(const byte *input, quint32 *output, size_t length);

    string bytesToHexString(const byte *input, size_t length);
    void bytesToString(const byte *input, char* output);
    void stringTobytes(const char *input, byte* output);

    void update(const void *input, size_t length);
    void update(const string &str);
    void update(ifstream &in);

    int charToHex(char ch);
    const byte* digest();
    string toString();
    void reset();

private:
    quint32 _state[4];
    quint32 _count[2];
    byte _buffer[64];
    byte _digest[16];
    bool _finished;

    static const byte PADDING[64];
    static const char HEX[16];
    static const size_t BUFFER_SIZE = 1024;
};

#endif/*MD5_H*/
