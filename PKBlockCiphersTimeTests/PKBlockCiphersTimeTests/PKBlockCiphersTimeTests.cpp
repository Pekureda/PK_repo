#include <iostream>
#include <chrono>
#include <exception>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <regex>
#include <sstream>
#include <functional>
#include <ctime>

#include <intrin.h>

#include <windows.h>
#include <processthreadsapi.h>

#include <iostream>

#include <openssl\aes.h>
#include <openssl\evp.h>
//#include <boost/timer/timer.hpp>

#pragma execution_character_set("utf-8")

int encrypt(unsigned char* plaintext, int plaintextLen, unsigned char* key, unsigned char* iv, const EVP_CIPHER* mode, unsigned char* cipherText) {
    EVP_CIPHER_CTX* ctx;

    int len;
    int cipherText_len;

    if (!(ctx = EVP_CIPHER_CTX_new())) {
        throw std::runtime_error("EVP_CIPHER_CTX_new failed");
    }

    if (1 != EVP_EncryptInit_ex(ctx, mode, NULL, key, iv)) {
        throw std::runtime_error("EVP_EncryptInit_ex failed");
    }

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if (1 != EVP_EncryptUpdate(ctx, cipherText, &len, plaintext, plaintextLen)) {
        throw std::runtime_error("EVP_EncryptUpdate failed");
    }
    cipherText_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if (1 != EVP_EncryptFinal_ex(ctx, cipherText + len, &len)) {
        throw std::runtime_error("EVP_EncryptFinal_ex failed");
    }
    cipherText_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return cipherText_len;
}

int decrypt(unsigned char* cipherText, int cipherTextLen, unsigned char* key, unsigned char* iv, const EVP_CIPHER* mode, unsigned char* plaintext) {
    EVP_CIPHER_CTX* ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        throw std::runtime_error("EVP_CIPHER_CTX_new failed");
    }

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_DecryptInit_ex(ctx, mode, NULL, key, iv)) {
        throw std::runtime_error("EVP_DecryptInit_ex failed");
    }
    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, cipherText, cipherTextLen)) {
        throw std::runtime_error("EVP_DecryptUpdate failed");
    }
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
        throw std::runtime_error("EVP_DecryptFinal_ex failed");
    }
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

//double get_cpu_time() {
//    FILETIME a, b, c, d;
//    if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
//        //  Returns total user time.
//        //  Can be tweaked to include kernel times as well.
//        return
//            (double)(d.dwLowDateTime |
//                ((unsigned long long)d.dwHighDateTime << 32));
//    }
//    else {
//        //  Handle error
//        return 0;
//    }
//}

//uint64_t rdtsc() {
//    return __rdtsc();
//}

size_t measure(std::string message, std::function<int(unsigned char*, int, unsigned char*, unsigned char*, const EVP_CIPHER*, unsigned char*)> func,    
    unsigned char* textIn, int textInLen, unsigned char* key, unsigned char* iv, const EVP_CIPHER* mode, unsigned char* textOut) {
    
    // measuring wall time
    std::cout << message;
    std::chrono::high_resolution_clock::time_point timePoint1, timePoint2;
    //boost::timer::cpu_timer timer;

    timePoint1 = std::chrono::high_resolution_clock::now();

    //timer.start();
    //std::clock_t start = std::clock();

    size_t size = func(textIn, textInLen, key, iv, mode, textOut);

    //std::clock_t end = std::clock();
    //timer.stop();

    timePoint2 = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(timePoint2 - timePoint1).count() << "us" << std::endl;
    //std::cout << timer.format() << std::endl;
    //std::cout << end - start << "ms" << std::endl;

    return size;
}

int main()
{
    SetConsoleOutputCP(65001);
    std::regex pattern("\.txt$", std::regex_constants::icase);
    std::string path = std::filesystem::current_path().generic_string();

    std::fstream fs;
    
    std::vector<std::string> fnames;

    // populate fnames vector
    for (auto f : std::filesystem::directory_iterator(path)) {
        if (std::regex_search(f.path().filename().generic_string(), pattern)) {
            fnames.push_back(f.path().generic_string());
        }
    }

    unsigned char* key = (unsigned char*)"politechnikapozn";
    unsigned char* iv = (unsigned char*)"kryptografiapods";

    for (auto elem : fnames) {
        fs.open(elem, std::ios_base::in);
        size_t size = std::filesystem::file_size(elem);
        size_t encLength;

        std::string temp;
        temp.resize(size);

        fs.read(&temp[0], size);
        // filesize does not always correspond to readable bytes
        size = fs.gcount();
        temp.resize(size);
        fs.close();

        Sleep(2000);

        unsigned char* result = new unsigned char[size];
        unsigned char* res = new unsigned char[size+16];
        unsigned char* decres = new unsigned char[size];
        std::copy(temp.begin(), temp.end(), result);

        
        std::cout << elem << "\tsize: " << size << "B" << std::endl << "========" << std::endl;
        encLength = measure("AES128 ECB encryption time : ", &encrypt, result,  temp.length(),  key, iv, EVP_aes_128_ecb(), res);
                    measure("AES128 ECB decryption time : ", &decrypt, res,     encLength,      key, iv, EVP_aes_128_ecb(), decres);

        encLength = measure("AES128 CBC encryption time : ", &encrypt, result,  temp.length(),  key, iv, EVP_aes_128_cbc(), res);
                    measure("AES128 CBC decryption time : ", &decrypt, res,     encLength,      key, iv, EVP_aes_128_cbc(), decres);

        encLength = measure("AES128 CFB encryption time : ", &encrypt, result,  temp.length(),  key, iv, EVP_aes_128_cfb(), res);
                    measure("AES128 CFB decryption time : ", &decrypt, res,     encLength,      key, iv, EVP_aes_128_cfb(), decres);

        //encLength = measure("AES128 XTS encryption time : ", &encrypt, result,  temp.length(),  key, iv, EVP_aes_128_xts(), res);
        //            measure("AES128 XTS decryption time : ", &decrypt, res,     encLength,      key, iv, EVP_aes_128_xts(), decres);

        encLength = measure("AES128 OFB encryption time : ", &encrypt, result,  temp.length(),  key, iv, EVP_aes_128_ofb(), res);
                    measure("AES128 OFB decryption time : ", &decrypt, res,     encLength,      key, iv, EVP_aes_128_ofb(), decres);

        encLength = measure("AES128 CTR encryption time : ", &encrypt, result,  temp.length(),  key, iv, EVP_aes_128_ctr(), res);
                    measure("AES128 CTR decryption time : ", &decrypt, res,     encLength,      key, iv, EVP_aes_128_ctr(), decres);

        delete[] result, res, decres;
        //std::cout << "AES128 ECB enctyption time: ";
        //timePoint1 = std::chrono::high_resolution_clock::now();

        //size_t encryptedSize = encrypt(result, temp.length(), key, iv, EVP_aes_128_ecb(), res);

        //timePoint2 = std::chrono::high_resolution_clock::now();
        //std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::chrono::nanoseconds>>(timePoint2 - timePoint1).count() << "ns" << std::endl;
        
        //// pomiar
        //size_t decryptedSize = decrypt(res, encryptedSize, key, iv, EVP_aes_128_ecb(), decres);
        //// pomiar
        //// print


        ////std::string hexstr(reinterpret_cast<char*>(decres));
        //std::string hexstr;
        //hexstr.resize(encryptedSize);
        //std::stringstream ss;
        //for (int i = 0; i < encryptedSize; ++i) {
        //    ss << std::hex << std::setfill('0') << std::setw(2) << int(res[i]);
        //}
        //hexstr = ss.str();

        //fs.open(path + "/test.txt", std::ios_base::out);
        //fs.write(&hexstr[0], hexstr.size());
        //fs.close();
    }

}
