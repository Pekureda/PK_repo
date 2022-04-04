#ifndef AES_CRYPTO_H
#define AES_CRYPTO_H

#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/aes.h>

#include <stdexcept>
#include <sstream>
#include <iomanip>

#define BLOCK_SIZE 16

namespace std {
    typedef basic_string<unsigned char> ustring;
}

std::ustring AES_core_encrypt(const std::ustring& plaintext, const std::ustring& key) {
    if (plaintext.length() > BLOCK_SIZE)    throw std::invalid_argument("plaintext length > BLOCK_SIZE");
    if (key.length() != BLOCK_SIZE)         throw std::invalid_argument("key length != BLOCK_SIZE");

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("EVP_CIPHER_CTX_new() failed");

    int len = 0;//, cipher_len = 0;

    unsigned char* cipher = new unsigned char[2*BLOCK_SIZE];
    std::ustring result;

    if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key.c_str(), NULL))           throw std::runtime_error("EVP_EncryptInit_ex() failed");
    //if (plaintext.length() == BLOCK_SIZE) EVP_CIPHER_CTX_set_padding(ctx, EVP_CIPH_NO_PADDING);
    if (!EVP_EncryptUpdate(ctx, cipher, &len, plaintext.c_str(), plaintext.length()))   throw std::runtime_error("EVP_EncryptUpdate() failed");
    //cipher_len += len;
    if (!EVP_EncryptFinal_ex(ctx, cipher + len, &len))                                  throw std::runtime_error("EVP_EncryptFinal_ex() failed");
    //cipher_len += len;

    EVP_CIPHER_CTX_free(ctx);
    result = std::ustring(cipher, cipher + (plaintext.length() == 16 ? 2 * BLOCK_SIZE : BLOCK_SIZE));
    delete[] cipher;

    return result;
}

std::ustring AES_core_decrypt(const std::ustring& encryptedtext, const std::ustring& key) {
    //if (encryptedtext.length() != BLOCK_SIZE)    throw std::invalid_argument("encryptedtext length > BLOCK_SIZE");
    if (key.length() != BLOCK_SIZE)             throw std::invalid_argument("key length != BLOCK_SIZE");

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("EVP_CIPHER_CTX_new() failed");

    int len = 0;
    //int text_len = 0;

    unsigned char* cipher = new unsigned char[BLOCK_SIZE];
    std::ustring result;

    if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key.c_str(), NULL))                   throw std::runtime_error("EVP_DecryptInit_ex() failed");
    if (encryptedtext.length() == BLOCK_SIZE) EVP_CIPHER_CTX_set_padding(ctx, 0);
    if (!EVP_DecryptUpdate(ctx, cipher, &len, encryptedtext.c_str(), encryptedtext.length()))   throw std::runtime_error("EVP_DecryptUpdate() failed");
    //text_len += len;
    if (!EVP_DecryptFinal_ex(ctx, cipher + len, &len))                                          throw std::runtime_error("EVP_DecryptFinal_ex() failed");
    //text_len += len;

    EVP_CIPHER_CTX_free(ctx);
    result = std::ustring(cipher, cipher + (BLOCK_SIZE));
    delete[] cipher;

    return result;
}

std::ustring AES_ECB_encrypt(const std::ustring& plaintext, const std::ustring& key) {
    std::ustring result = (unsigned char*)"";
    // może zastosować padding w takim samym stylu jak w cbc
    unsigned int i = 0;
    do {
        result = result.substr(0, result.size() - BLOCK_SIZE);
        result += AES_core_encrypt(plaintext.substr(i, BLOCK_SIZE), key);
        i += BLOCK_SIZE;
    } while (i < plaintext.length());

    return result;
}

std::ustring AES_ECB_decrypt(const std::ustring& encryptedtext, const std::ustring& key) {
    std::ustring result = (unsigned char*)"";

    unsigned int i = 0;
    do {
        //result = result.substr(0, result.size() - BLOCK_SIZE);
        result += AES_core_decrypt(encryptedtext.substr(i, BLOCK_SIZE), key);
        i += BLOCK_SIZE;
    } while (i < encryptedtext.length());

    return result;
}

std::ustring ustring_XOR(const std::ustring& a, const std::ustring& b) {
    std::ustring result = (unsigned char*)"";
    size_t max_idx = std::min(a.length(), b.length());
    for (size_t i = 0; i < max_idx; ++i) {
        result += a[i] ^ b[i];
    }
    return result;
}

std::ustring AES_CBC_encrypt(std::ustring plaintext, const std::ustring& key, const std::ustring& iv) {
    if (plaintext.size() % BLOCK_SIZE == 0) {
        plaintext.resize(plaintext.length() + BLOCK_SIZE);
        std::fill(plaintext.end() - BLOCK_SIZE, plaintext.end(), 0x10);
    }
    std::ustring result = (unsigned char*)"";
    std::ustring constructionString = iv;

    unsigned int i = 0;
    do {
        std::ustring temp = plaintext.substr(i, BLOCK_SIZE);
        if (temp.size() < BLOCK_SIZE) {
            unsigned char fillByte = BLOCK_SIZE - temp.size();
            temp.resize(BLOCK_SIZE);
            std::fill(temp.begin() + BLOCK_SIZE - fillByte, temp.end(), fillByte);
        }
        constructionString = ustring_XOR(constructionString, temp);
        constructionString = AES_core_encrypt(constructionString, key);
        result += constructionString;
        i += BLOCK_SIZE;
        result = result.substr(0, result.size() - BLOCK_SIZE);
    } while (i < plaintext.length());

    return result;
}

std::ustring AES_CBC_decrypt(const std::ustring& cipher, const std::ustring& key, const std::ustring& iv) {
    std::ustring result = (unsigned char*)"";
    std::ustring constructionString = iv;
    // tutaj powinna jeszcze być kontrola czy padding jest poprawny
    unsigned int i = 0;
    do {
        std::ustring decoded = AES_core_decrypt(cipher.substr(i, BLOCK_SIZE), key);
        constructionString = ustring_XOR(constructionString, decoded);
        result += constructionString;
        constructionString = cipher.substr(i, BLOCK_SIZE);
        i += BLOCK_SIZE;
    } while (i < cipher.length());

    return result;
}

std::ustring AES_CFB_encrypt(std::ustring plaintext, const std::ustring& key, const std::ustring& iv) {
    if (plaintext.size() % BLOCK_SIZE == 0) {
        plaintext.resize(plaintext.length() + BLOCK_SIZE);
        std::fill(plaintext.end() - BLOCK_SIZE, plaintext.end(), 0x10);
    }
    std::ustring result = (unsigned char*)"";
    std::ustring constructionString = iv;

    unsigned int i = 0;
    do {
        std::ustring temp = plaintext.substr(i, BLOCK_SIZE);
        //if (temp.size() < BLOCK_SIZE) {
        //    unsigned char fillByte = BLOCK_SIZE - temp.size();
        //    temp.resize(BLOCK_SIZE);
        //    std::fill(temp.begin() + BLOCK_SIZE - fillByte, temp.end(), fillByte);
        //}
        constructionString = AES_core_encrypt(constructionString, key);
        constructionString = ustring_XOR(constructionString, temp);

        result += constructionString;
        i += BLOCK_SIZE;
        //result = result.substr(0, result.size() - BLOCK_SIZE);
    } while (i < plaintext.length());

    return result;
}

std::ustring AES_CFB_decrypt(const std::ustring& cipher, const std::ustring& key, const std::ustring& iv) {
    std::ustring result = (unsigned char*)"";
    std::ustring constructionString = iv;

    unsigned int i = 0;
    do {
        std::ustring decoded = AES_core_encrypt(constructionString, key);
        constructionString = ustring_XOR(cipher.substr(i, BLOCK_SIZE), decoded);
        result += constructionString;
        constructionString = cipher.substr(i, BLOCK_SIZE);
        i += BLOCK_SIZE;
    } while (i < cipher.length());

    return result;
}

std::string ustrtostr(const std::ustring& str) {
    std::string res;
    for (std::ustring::const_iterator it = str.begin(); it != str.end(); ++it) {
        res += (char)(*it);
    }
    return res;
}

std::ustring strtoustr(const std::string& str) {
    std::ustring res;
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        res += (unsigned char)(*it);
    }
    return res;
}

std::string ustrtohex(const std::ustring str) {
    std::stringstream ss;
    for (std::ustring::const_iterator it = str.begin(); it != str.end(); ++it) {
        ss << std::hex << std::setfill('0') << std::setw(2) << int(*it);
    }
    return ss.str();
}

unsigned char hexval(unsigned char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    else if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    else abort();
}

std::string hextostr(const std::string& str) {
    std::string res;

    for (std::string::const_iterator p = str.begin(); p != str.end(); p++)
    {
        unsigned char c = hexval(*p);
        p++;
        if (p == str.end()) break; // incomplete last digit - should report error
        c = (c << 4) + hexval(*p); // + takes precedence over <<
        res.push_back(c);
    }
    return res;
}

#endif // AES_CRYPTO_H
