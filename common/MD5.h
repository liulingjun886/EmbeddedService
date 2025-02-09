#pragma once
#include <string>
#include <fstream>
#include <core/UserTypeDefines.h>

/* Type define */
//typedef unsigned char UINT8;
//typedef unsigned int UIN32;

using std::ifstream;
using std::string;

/* MD5 declaration. */
class MD5
{
public:
    MD5();
    MD5(const void *input, size_t length);
    MD5(const string &str);
    MD5(ifstream &in);
    void update(const void *input, size_t length);
    void update(const string &str);
    void update(ifstream &in);
    const UINT8 *digest();
    string toString();
    void reset();

private:
    void update(const UINT8 *input, size_t length);
    void final();
    void transform(const UINT8 block[64]);
    void encode(const UINT32 *input, UINT8 *output, size_t length);
    void decode(const UINT8 *input, UINT32 *output, size_t length);
    string bytesToHexString(const UINT8 *input, size_t length);

    /* class uncopyable */
    MD5(const MD5 &);
    MD5 &operator=(const MD5 &);

private:
    UINT32 _state[4]; /* state (ABCD) */
    UINT32 _count[2]; /* number of bits, modulo 2^64 (low-order word first) */
    UINT8 _buffer[64]; /* input buffer */
    UINT8 _digest[16]; /* message digest */
    bool _finished;   /* calculate finished ? */

    static const UINT8 PADDING[64]; /* padding for calculate */
    static const char HEX[16];
    enum
    {
        BUFFER_SIZE = 1024
    };
};