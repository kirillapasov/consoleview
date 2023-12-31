#include <iostream>
#include <windows.h>
#include <string>
#include <chrono>
#include <ctime>
#include <time.h>
#include <sstream>
#include <vector>

constexpr auto PASSWORD_AUTH = 420;
constexpr auto TOKEN_AUTH = 421;
constexpr auto TWO_FACTOR_AUTH = 422;
constexpr auto CONFIGLENGTH = 6;

using namespace std;
class Auth {
public:
	Auth() {

	}
	/// <summary>
	/// ������������� ������������ ���������� ������� ��� �����
	/// </summary>
	/// <param name="value"> Int �������� �� 1 �� MAXINT</param>
	void SetMaxAuthTries(int value) {
		if (value > 0 && value <= INT32_MAX)
			this->maxAuthTries = value;
		else this->maxAuthTries = DEFAULT_MAX_AUTH_TRIES;
	}
	/// <summary>
	/// ������������ �������� �������� ��� ������� ��������������
	/// </summary>
	/// <param name="value">Int �������� �� 1 �� MAXINT</param>
	void SetTimeoutInSecond(int value) {
		if (value > 0 && value <= INT32_MAX)
			this->timeoutInSeconds = value;
		else this->timeoutInSeconds = DEFAULT_TIMEOUT_IN_SECONDS;
	}
	/// <summary>
	/// ������������� ��� ��������������
	/// 1 - �������������� �� ������
	/// 2 - �������������� �� ������
	/// 3 - ������������� ��������������
	/// </summary>
	/// <param name="value">Int �������� �� 1 �� 3</param>
	void SetAuthType(int value) {
		if (value > 0 && value < 4)
			this->authType = value + 419;
		else this->authType = DEFAULT_AUTH_TYPE;
	}
	/// <summary>
	/// ������������ �������� ������� � ������������ � ���������
	/// </summary>
	/// <param name="value">Integer �� 1 �� 127</param>
	void SetPasswordPolicy(int value) {
		if (value > 0 && value < 128)
			this->passwordPolicy = value;
		else this->passwordPolicy = DEFAULT_PASSWORD_POLICY;
	}
	/// <summary>
	/// ������������� ������������ ����� ������
	/// </summary>
	/// <param name="value">Integer �� ����������� ����� ������ �� 255</param>
	void SetMaxPasswordLength(int value) {
		if (value > minimalPasswordLength && value < 256)
			this->maxPasswordLength = value;
		if (value < minimalPasswordLength) this->maxPasswordLength = minimalPasswordLength + 1;
		if (value >= 256) this->maxPasswordLength = 255;
	}

	 /// <summary>
	 /// ������� ������ ���� ������ �� ����������
	 /// </summary>
	 /// <returns>std::vector �������� ���������� ������ * 3</returns>
	 std::vector<std::string> GetDiskListAndSerialNumbers() {
		DWORD drives = GetLogicalDrives();
		std::vector<std::string> result;

		for (char i = 'A'; i <= 'Z'; ++i) {
			if ((drives & 1) != 0) {

				std::string driveLetter = " :\\";
				driveLetter[0] = i;

				DWORD serialNumber;
				DWORD fileSystemFlags;
				std::stringstream ss;

				if (GetVolumeInformationA(driveLetter.c_str(), nullptr, 0, &serialNumber, nullptr, &fileSystemFlags, nullptr, 0)) {

					result.push_back(driveLetter);
					ss << serialNumber;
					result.push_back(ss.str());
					ss.clear();
					ss << fileSystemFlags;
					//std::cout << ss.str();
					result.push_back(ss.str());
				}
				else {
					throw "Cant get drive info";
				}
			}
			drives >>= 1;
		}
		return result;
	 }

	 void GetTime() {
		 GetCurrentDate();
	}

	 //Todo �������� �������� ��������� �����
	 std::string MakeAuthKey(int driveNumber, std::vector<std::string> origin) {

		 if (!(driveNumber >= 0 && driveNumber << origin.size() / 3)) throw "Auth Key creation failed";

		 std::string keyPart = origin[(driveNumber * 3) + 2] + origin[(driveNumber * 3)+1] + origin[(driveNumber * 3) + 2];
		 int len = strnlen_s(keyPart.c_str(), 64);
		 if (len != 32) {
			 int i = 32 - len;
			 if (i < 0) return std::string(keyPart.c_str(), 32);
		 }
	 }
	 void MakeAuth(std::string username, std::string password) {
		//Todo ��������� 
	 }
private:
	const int DEFAULT_MAX_AUTH_TRIES = 3;
	const int DEFAULT_TIMEOUT_IN_SECONDS = 60;
	const int DEFAULT_AUTH_TYPE = PASSWORD_AUTH;
	const int DEFAULT_MAX_PASSWORD_LENGTH = 16;
	const int DEFAULT_PASSWORD_POLICY = 2;
	const int DEFAULT_MIN_PASSWORD_LENGTH = 3;

	int maxAuthTries = DEFAULT_MAX_AUTH_TRIES;
	int timeoutInSeconds = DEFAULT_TIMEOUT_IN_SECONDS;
	int authType = DEFAULT_AUTH_TYPE;
	int maxPasswordLength = DEFAULT_MAX_PASSWORD_LENGTH;
	int passwordPolicy = DEFAULT_PASSWORD_POLICY;
	int minimalPasswordLength = DEFAULT_MIN_PASSWORD_LENGTH;
	
	DWORD serialNumber;
	DWORD fileSystemFlags;
	WCHAR volumeName[MAX_PATH] = { 0 };
	WCHAR fileSystemName[MAX_PATH] = { 0 };

	string* cpuSerial, *moboserial, *diskserial;

	enum UserStatus {
		normal = 0,
		needsToChangePassword = 1,
		temporarilyBanned = 2,
		permanentlyBanned = 3,
	};

	/**
	* ��������� ������ � USB ����������
	* ������ ������ � ���������� � ������ "F:"
	*/
	void GetUsbData() {
		(GetVolumeInformation(
			L"F:\\",
			NULL,
			NULL,
			&serialNumber,
			NULL,
			&fileSystemFlags,
			NULL,
			NULL
		));
	}

	/*
	* ����� ������ � ����������� ����� � ����������
	*/
	void GetHWData() {

		//����� ��������� ������� wmic ��� ��������� ���� � ��������� ��������� � ����������
		string moboSerialCommand = "wmic baseboard get serialnumber | find /v \"SerialNumber\"";
		char moboSerialBuf[1024]{};
		FILE* moboSerialStream = _popen(moboSerialCommand.c_str(), "r");
		if (moboSerialStream) {
			if (fgets(moboSerialBuf, sizeof(moboSerialBuf), moboSerialStream) != nullptr) {
				*moboserial = *moboSerialBuf;
			}
			_pclose(moboSerialStream);
		}

		 moboSerialCommand = "wmic cpu get processorId | find /v \"ProcessorId\"";

		 moboSerialStream = _popen(moboSerialCommand.c_str(), "r");
		if (moboSerialStream) {
			if (fgets(moboSerialBuf, sizeof(moboSerialBuf), moboSerialStream) != nullptr) {
				*cpuSerial = *moboSerialBuf;
			}
			_pclose(moboSerialStream);
		}
	}
	int CompareStrings(std::string stringone, std::string stringtwo) {

		std::vector<char> vone;
		std::vector<char> vtwo;

		int sizeone = strnlen_s(stringone.c_str(), 192);
		int sizetwo = strnlen_s(stringtwo.c_str(), 192);
		if (sizeone == 0 || sizetwo == 0) throw "Null Sized string in input";
		int res=0;

		int diff = sizeone - sizetwo;
		switch (diff) {
		case 0:
			break;
		default:
			return -1;
		}

		for (int i = 0; i < sizeone; i++) {
			vone.push_back(stringone[i]);
			vtwo.push_back(stringtwo[i]);
		}
		for (int i = 0; i < sizeone; i++) {
			res += (int)vone[i] - (int)vtwo[i];
		}

		switch (res) {
		case 0:
			return 0;
		default:
			return -1;
		}
	}
	std::string GetCurrentDate() {
		std::time_t currentTime = std::time(nullptr);
		struct tm timeInfo;
		char buffer[26];

		// ���������� ���������� ������� ctime_s ��� �������������� �������
		if (ctime_s(buffer, sizeof(buffer), &currentTime) == 0) {
			std::cout << "" << buffer << std::endl;
		}
		else {
			std::cerr << "������ ��� ��������� ������� ���� � �������." << std::endl;
		}
		return buffer;
	}

};