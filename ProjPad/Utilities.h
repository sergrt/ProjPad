#pragma once
#include <vector>
#include <stdexcept>



namespace Utilities {
    long long fileSize(const std::string& fileName);
    int utf8len(const std::string& str);
}
namespace Cryptopp {
    std::vector<unsigned char> encrypt(const std::string& src, const std::string& password);
    std::vector<unsigned char> decrypt(const std::vector<unsigned char>& src, const std::string& password);
}