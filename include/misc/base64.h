// Taken from https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c

#ifndef _BASE64_H_
#define _BASE64_H_

#include <vector>
#include <string>
typedef unsigned char BYTE;

namespace base64
{
    std::string encode(BYTE const* buf, unsigned int bufLen);
    std::vector<BYTE> decode(std::string const&);
}

#endif