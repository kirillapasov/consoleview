#include <iostream>
#include <iomanip>
#include <fstream>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>

using namespace CryptoPP;

class Crypto {
public:
    bool EncryptFile(const std::string& inputFile, const std::string& outputFile, const std::string& key) {
        if (key.size() != AES::MAX_KEYLENGTH) {
            std::cerr << "Ошибка: Неверная длина ключа." << std::endl;
            return false;
        }

        SecByteBlock keyBlock(reinterpret_cast<const byte*>(key.data()), key.size());
        SecByteBlock iv(AES::BLOCKSIZE);

        // Вы можете выбрать генерацию случайного IV здесь, если это необходимо
        AutoSeededRandomPool prng;
        prng.GenerateBlock(iv, iv.size());

        std::cout << "Используемый ключ: ";
        PrintBlock(keyBlock);

        std::cout << "Сгенерированный IV: ";
        PrintBlock(iv);

        std::ifstream inFile(inputFile, std::ios::binary);
        if (!inFile.is_open()) {
            std::cerr << "Ошибка открытия входного файла." << std::endl;
            return false;
        }

        std::ofstream outFile(outputFile, std::ios::binary);
        if (!outFile.is_open()) {
            std::cerr << "Ошибка открытия выходного файла." << std::endl;
            return false;
        }

        outFile.write(reinterpret_cast<const char*>(keyBlock.data()), keyBlock.size());
        outFile.write(reinterpret_cast<const char*>(iv.data()), iv.size());

        if (!EncryptData(inFile, outFile, keyBlock, iv)) {
            std::cerr << "Ошибка шифрования содержимого файла." << std::endl;
            return false;
        }

        return true;
    }

    bool DecryptFile(const std::string& inputFile, const std::string& outputFile, const std::string& key) {
        if (key.size() != AES::MAX_KEYLENGTH) {
            std::cerr << "Ошибка: Неверная длина ключа." << std::endl;
            return false;
        }

        std::ifstream inFile(inputFile, std::ios::binary);
        if (!inFile.is_open()) {
            std::cerr << "Ошибка открытия входного файла." << std::endl;
            return false;
        }

        SecByteBlock keyBlock(reinterpret_cast<const byte*>(key.data()), key.size());
        SecByteBlock iv(AES::BLOCKSIZE);
        inFile.read(reinterpret_cast<char*>(iv.data()), iv.size());

        std::ofstream outFile(outputFile, std::ios::binary);
        if (!outFile.is_open()) {
            std::cerr << "Ошибка открытия выходного файла." << std::endl;
            return false;
        }

        if (!DecryptData(inFile, outFile, keyBlock, iv)) {
            std::cerr << "Ошибка расшифрования содержимого файла." << std::endl;
            return false;
        }

        return true;
    }

private:
    void PrintBlock(const SecByteBlock& block) {
        for (size_t i = 0; i < block.size(); ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(block[i]);
        }
        std::cout << std::dec << std::endl;
    }

    bool EncryptData(std::ifstream& inFile, std::ofstream& outFile, const SecByteBlock& key, const SecByteBlock& iv) {
        try {
            CBC_Mode<AES>::Encryption encryption(key, key.size(), iv);
            FileSource fileSource(inFile, true, new StreamTransformationFilter(encryption, new FileSink(outFile)));

            return true;
        }
        catch (const Exception& e) {
            std::cerr << "Исключение Crypto++: " << e.what() << std::endl;
            return false;
        }
    }

    bool DecryptData(std::ifstream& inFile, std::ofstream& outFile, const SecByteBlock& key, const SecByteBlock& iv) {
        try {
            CBC_Mode<AES>::Decryption decryption(key, key.size(), iv);
            FileSource fileSource(inFile, true, new StreamTransformationFilter(decryption, new FileSink(outFile)));

            return true;
        }
        catch (const Exception& e) {
            std::cerr << "Исключение Crypto++: " << e.what() << std::endl;
            return false;
        }
    }
};
