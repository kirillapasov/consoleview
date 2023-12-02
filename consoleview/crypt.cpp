#include <iostream>
#include <fstream>
#include <cryptopp/files.h> 
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>

using namespace CryptoPP;

void encryptFile(const std::string& inputFile, const std::string& outputFile, const byte key[], const byte iv[]) {
    std::ifstream ifs(inputFile, std::ios::binary);
    std::ofstream ofs(outputFile, std::ios::binary);

    AES::Encryption aesEncryption(key, AES::DEFAULT_KEYLENGTH);
    CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

    FileSource fileSource(ifs, true,
        new StreamTransformationFilter(cbcEncryption,
            new FileSink(ofs)
        )
    );
}

void decryptFile(const std::string& inputFile, const std::string& outputFile, const byte key[], const byte iv[]) {
    std::ifstream ifs(inputFile, std::ios::binary);
    std::ofstream ofs(outputFile, std::ios::binary);

    AES::Decryption aesDecryption(key, AES::DEFAULT_KEYLENGTH);
    CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

    FileSource fileSource(ifs, true,
        new StreamTransformationFilter(cbcDecryption,
            new FileSink(ofs)
        )
    );
}

int main() {
    // Замените ключ и вектор инициализации на свои значения
    byte key[AES::DEFAULT_KEYLENGTH] = { 'k', 'e', 'y', '1', '2', '3', '4', '5', '6', '7', '8', '9', '1', '0', '1', '1'};
    byte iv[AES::BLOCKSIZE] = { 'i', 'v', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4' };

    // Замените пути к файлам на свои
    std::string inputFile = "123.txt";
    std::string encryptedFile = "encrypted.txt";
    std::string decryptedFile = "decrypted.txt";

    // Шифрование файла
    encryptFile(inputFile, encryptedFile, key, iv);
    std::cout << "File encrypted successfully." << std::endl;

    // Расшифрование файла
    decryptFile(encryptedFile, decryptedFile, key, iv);
    std::cout << "File decrypted successfully." << std::endl;

    return 0;
}