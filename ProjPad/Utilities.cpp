#include "stdafx.h"
#include "Utilities.h"

#include "osrng.h"
#include "modes.h"
#include "hkdf.h"

namespace Utilities {
    long long fileSize(const std::string& fileName) {
        struct _stat64 stat_buf;
        const int rc = _stat64(fileName.c_str(), &stat_buf);
        if (rc == -1)
            throw std::runtime_error("Utilities::fileSize(const std::string&) - unable to get filestats");

        return stat_buf.st_size;
    }
}
namespace Cryptopp {
    CryptoPP::SecByteBlock getIv() {
        std::vector<CryptoPP::byte> data{
            0x0C, 0x3F, 0xF9, 0xA8, 0x00, 0x0A, 0x7E, 0x8C,
            0x12, 0x40, 0xD3, 0xAF, 0xC1, 0xAE, 0x3B, 0x1D
        };

        return CryptoPP::SecByteBlock(data.data(), data.size());
    }

    std::vector<unsigned char> encrypt(const std::string& src, const std::string& password) {
        using namespace CryptoPP;
        SecByteBlock iv = getIv();
        SecByteBlock key(0x00, AES::DEFAULT_KEYLENGTH);
        HKDF<SHA256> hkdf;
        hkdf.DeriveKey(key, key.size(), (const byte*)password.data(), password.size(), (const byte*)iv.data(), iv.size(), NULL, 0);

        CFB_Mode<AES>::Encryption cfbEncryption(key, key.size(), iv);
        std::vector<byte> out(src.size());
        cfbEncryption.ProcessData(out.data(), (const byte*)src.data(), src.size());

        return out;
    }
    std::vector<unsigned char> decrypt(const std::vector<unsigned char>& src, const std::string& password) {
        using namespace CryptoPP;
        SecByteBlock iv = getIv();
        SecByteBlock key(0x00, AES::DEFAULT_KEYLENGTH);
        HKDF<SHA256> hkdf;
        hkdf.DeriveKey(key, key.size(), (const byte*)password.data(), password.size(), (const byte*)iv.data(), iv.size(), NULL, 0);

        CFB_Mode<AES>::Decryption cfbDecryption(key, key.size(), iv);
        std::vector<byte> out(src.size());
        cfbDecryption.ProcessData(out.data(), (const byte*)src.data(), src.size());

        return out;
    }
}