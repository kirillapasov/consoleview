#include <iostream>
#include <iomanip>
#include <fstream>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h> 

using namespace CryptoPP;

class CryptoUtils {
public:
    static bool EncryptFile(const std::string& inputFile, const std::string& outputFile) {
        SecByteBlock key(AES::MAX_KEYLENGTH/2), iv(AES::BLOCKSIZE);
        AutoSeededRandomPool prng;
        prng.GenerateBlock(key, key.size());
        prng.GenerateBlock(iv, iv.size());
        
        std::cout << "Сгенерированный ключ: ";
        PrintBlock(key);

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

        outFile.write(reinterpret_cast<const char*>(key.data()), key.size());
        outFile.write(reinterpret_cast<const char*>(iv.data()), iv.size());

        if (!EncryptData(inFile, outFile, key, iv)) {
            std::cerr << "Ошибка шифрования содержимого файла." << std::endl;
            return false;
        }

        return true;
    }

    static bool DecryptFile(const std::string& inputFile, const std::string& outputFile) {
        std::ifstream inFile(inputFile, std::ios::binary);
        if (!inFile.is_open()) {
            std::cerr << "Ошибка открытия входного файла." << std::endl;
            return false;
        }

        SecByteBlock key(AES::MAX_KEYLENGTH/2), iv(AES::BLOCKSIZE);
        inFile.read(reinterpret_cast<char*>(key.data()), key.size());
        inFile.read(reinterpret_cast<char*>(iv.data()), iv.size());

        std::ofstream outFile(outputFile, std::ios::binary);
        if (!outFile.is_open()) {
            std::cerr << "Ошибка открытия выходного файла." << std::endl;
            return false;
        }

        if (!DecryptData(inFile, outFile, key, iv)) {
            std::cerr << "Ошибка расшифрования содержимого файла." << std::endl;
            return false;
        }

        return true;
    }

private:
    static void PrintBlock(const SecByteBlock& block) {
        for (size_t i = 0; i < block.size(); ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(block[i]);
        }
        std::cout << std::dec << std::endl;
    }

    static bool EncryptData(std::ifstream& inFile, std::ofstream& outFile, const SecByteBlock& key, const SecByteBlock& iv) {
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

    static bool DecryptData(std::ifstream& inFile, std::ofstream& outFile, const SecByteBlock& key, const SecByteBlock& iv) {
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

int main() {
    setlocale(LC_ALL, "Russian");
    std::string inputFile = "input.txt";
    std::string encryptedFile = "encrypted.bin";
    std::string decryptedFile = "decrypted.txt";

    if (CryptoUtils::EncryptFile(inputFile, encryptedFile)) {
        std::cout << "Файл успешно зашифрован." << std::endl;
    }
    else {
        std::cerr << "Ошибка шифрования файла." << std::endl;
        return 1;
    }

    if (CryptoUtils::DecryptFile(encryptedFile, decryptedFile)) {
        std::cout << "Файл успешно расшифрован." << std::endl;
    }
    else {
        std::cerr << "Ошибка расшифрования файла." << std::endl;
        return 1;
    }

    return 0;
}
