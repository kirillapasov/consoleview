#include <iostream>
#include <windows.h>
#include <string>

using namespace std;
template <typename T>
class Auth {
public:
	Auth(const T& username, const T& password): username(username), password(password) {
		
	}
	void MakeAuth() {
		//Todo сделать функцию безопасного сравнения со свитчами

		bool checked;

		checked = (username == "nigger" & password == "faggot") ? true : false;
		checked ? (cout << "yes \n") : (cout << "no \n");

		GetHWData();
	}
	void CheckUSBDrive() {
		GetUsbData();
		DWORD res = TRUSTED_USB_SERIAL - serialNumber;
		cout << res << "\n";
	}
	void CheckHardWare() {
		//Todo написать метод проверки железа
	}
	static void ReAuth() {
		//Todo написать реаутентификацию на основе данных из XML парсера.

		//Todo описать проверку флешки и железа


	}
	void CheckBan() {
		//Todo описать метод проверки бана через xml конфиг
	}
private:
	T username;
	T password;
	T hashedPassword;

	string baseLogin;
	string baseHashedPassword;

	DWORD serialNumber;
	DWORD fileSystemFlags;
	WCHAR volumeName[MAX_PATH] = { 0 };
	WCHAR fileSystemName[MAX_PATH] = { 0 };

	string cpuSerial, moboserial, diskserial;

	int status;
	int bit = 0;

	//Я пока хз, надо оно или нет, оставим так
	//Todo сгенерировать ключи соответствия
	//Todo сгенерировать фальшивые ключи
	const int KEYS[3]{6,12,22};
	const int FKEYS[3]{7,13,23};
	const DWORD TRUSTED_USB_SERIAL = 1712026262;
	const DWORD TRUSTED_USB_FILE_SYSTEM_FLAGS = 131590;



	void MakeHashedPassword(T& username, T& password) {
		//Todo сделать функцию перевода пароля в хэш
	}
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
	void GetHWData() {

		//Вызов системной команды wmic для получения инфы о серийнике материнки и процессора
		string moboSerialCommand = "wmic baseboard get serialnumber | find /v \"SerialNumber\"";
		char moboSerialBuf[1024];
		FILE* moboSerialStream = _popen(moboSerialCommand.c_str(), "r");
		if (moboSerialStream) {
			if (fgets(moboSerialBuf, sizeof(moboSerialBuf), moboSerialStream) != nullptr) {
				moboserial = moboSerialBuf;
			}
			_pclose(moboSerialStream);
		}

		 moboSerialCommand = "wmic cpu get processorId | find /v \"ProcessorId\"";
		 moboSerialBuf[1024];
		 moboSerialStream = _popen(moboSerialCommand.c_str(), "r");
		if (moboSerialStream) {
			if (fgets(moboSerialBuf, sizeof(moboSerialBuf), moboSerialStream) != nullptr) {
				cpuSerial = moboSerialBuf;
			}
			_pclose(moboSerialStream);
		}

		//Todo слепить полученную инфу и захешировать
		
	}
	int CompareArrs() {
		//Todo написать метод сравнения двумя массивами, возвращающий константный инт
	}
	void GetDataFromXML() {
		//Todo подключить XML парсер
		//Todo сделать забор данных с XML парсера
	}
	void ReadXMLConfig() {
		//Todo описать чтение конфига из файла
	}
	void GetKey() {
		//?
	}

	//Todo добавить обработку неуспешных попыток

	/**
	* короче, есть вариант
	* 
	* можно сделать метод, к которому будет MakeAuth() обращаться.
	* эта параша будет возвращать инт или стринг, в котором будет спрятан статус
	* и MakeAuth() будет смотреть на статус и дальше будет делать какие-то действия\
	* типа, если статус (условно) 0, то аутентификация прошла
	* если (1, 72, 2465), то типа там будут запоротые попытки
	* надо придумать, как нормально сделать эту залупу
	*/

	//Todo добавить блокировку при превышении количества ошибок

	/**
	* а тут крч можно просто въебать отдельный тег в xml юзере, типа статус блокировки
	* можно точно так же въебать туда числа интовые, либо строку закодированную
	*/

};