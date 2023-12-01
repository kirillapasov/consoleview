#include <iostream>
#include <windows.h>
#include <string>

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
	/// Устанавливает максимальное количество попыток для входа
	/// </summary>
	/// <param name="value"> Int значение от 1 до MAXINT</param>
	void SetMaxAuthTries(int value) {
		if (value > 0 && value <= INT32_MAX)
			this->maxAuthTries = value;
		else this->maxAuthTries = DEFAULT_MAX_AUTH_TRIES;
	}
	/// <summary>
	/// Устанавливет значение таймаута при провале аутентификации
	/// </summary>
	/// <param name="value">Int значение от 1 до MAXINT</param>
	void SetTimeoutInSecond(int value) {
		if (value > 0 && value <= INT32_MAX)
			this->timeoutInSeconds = value;
		else this->timeoutInSeconds = DEFAULT_TIMEOUT_IN_SECONDS;
	}
	/// <summary>
	/// Устанавливает тип аутентификации
	/// 1 - Аутентификация по паролю
	/// 2 - Аутентификация по токену
	/// 3 - Двухфакторная аутентификация
	/// </summary>
	/// <param name="value">Int значение от 1 до 3</param>
	void SetAuthType(int value) {
		if (value > 0 && value < 4)
			this->authType = value + 419;
		else this->authType = DEFAULT_AUTH_TYPE;
	}
	/// <summary>
	/// Устанавлиает политику паролей в соответствии с правилами
	/// </summary>
	/// <param name="value">Integer от 1 до 127</param>
	void SetPasswordPolicy(int value) {
		if (value > 0 && value < 128)
			this->passwordPolicy = value;
		else this->passwordPolicy = DEFAULT_PASSWORD_POLICY;
	}
	/// <summary>
	/// Устанавливает максимальную длину пароля
	/// </summary>
	/// <param name="value">Integer от минимальной длины пароля до 255</param>
	void SetMaxPasswordLength(int value) {
		if (value > minimalPasswordLength && value < 256)
			this->maxPasswordLength = value;
		if (value < minimalPasswordLength) this->maxPasswordLength = minimalPasswordLength + 1;
		if (value >= 256) this->maxPasswordLength = 255;
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
	* Получение данных о USB накопителе
	* Читает данные с накопителя с меткой "F:"
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
	* Берет данные о материнской плате и процессоре
	*/
	void GetHWData() {

		//Вызов системной команды wmic для получения инфы о серийнике материнки и процессора
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
	int CompareArrs() {
		//Todo написать метод сравнения двумя массивами, возвращающий константный инт
	}
	
};