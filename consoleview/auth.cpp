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
		//Todo ������� ������� ����������� ��������� �� ��������

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
		//Todo �������� ����� �������� ������
	}
	static void ReAuth() {
		//Todo �������� ���������������� �� ������ ������ �� XML �������.

		//Todo ������� �������� ������ � ������


	}
	void CheckBan() {
		//Todo ������� ����� �������� ���� ����� xml ������
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

	//� ���� ��, ���� ��� ��� ���, ������� ���
	//Todo ������������� ����� ������������
	//Todo ������������� ��������� �����
	const int KEYS[3]{6,12,22};
	const int FKEYS[3]{7,13,23};
	const DWORD TRUSTED_USB_SERIAL = 1712026262;
	const DWORD TRUSTED_USB_FILE_SYSTEM_FLAGS = 131590;



	void MakeHashedPassword(T& username, T& password) {
		//Todo ������� ������� �������� ������ � ���
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

		//����� ��������� ������� wmic ��� ��������� ���� � ��������� ��������� � ����������
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

		//Todo ������� ���������� ���� � ������������
		
	}
	int CompareArrs() {
		//Todo �������� ����� ��������� ����� ���������, ������������ ����������� ���
	}
	void GetDataFromXML() {
		//Todo ���������� XML ������
		//Todo ������� ����� ������ � XML �������
	}
	void ReadXMLConfig() {
		//Todo ������� ������ ������� �� �����
	}
	void GetKey() {
		//?
	}

	//Todo �������� ��������� ���������� �������

	/**
	* ������, ���� �������
	* 
	* ����� ������� �����, � �������� ����� MakeAuth() ����������.
	* ��� ������ ����� ���������� ��� ��� ������, � ������� ����� ������� ������
	* � MakeAuth() ����� �������� �� ������ � ������ ����� ������ �����-�� ��������\
	* ����, ���� ������ (�������) 0, �� �������������� ������
	* ���� (1, 72, 2465), �� ���� ��� ����� ��������� �������
	* ���� ���������, ��� ��������� ������� ��� ������
	*/

	//Todo �������� ���������� ��� ���������� ���������� ������

	/**
	* � ��� ��� ����� ������ ������� ��������� ��� � xml �����, ���� ������ ����������
	* ����� ����� ��� �� ������� ���� ����� �������, ���� ������ ��������������
	*/

};