#ifndef BASE64_H
#define BASE64_H

#include <string>

std::string base64_encode(const unsigned char* data, size_t len);
std::string base64_decode(const std::string& encoded);

#endif // BASE64_H
