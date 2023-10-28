#include <iostream>
using namespace std;


class PasswordChecker {
private:
	__int8 straightOrderRules[7]{1,2,4,8,16,32,64};
	__int8 reverserOrderRules[7]{1,2,4,8,16,32,64};
	int arraysSize = (sizeof(straightOrderRules)/sizeof(straightOrderRules[0]));
	__int8 rule;
	void ParseRules(__int8 &rule) {
		RestoreArraysToDefault();
		while (rule > 0) {
			for (int i = arraysSize; i >= 0; i--) {
				if (straightOrderRules[i] > rule) {
					straightOrderRules[i] = 0;
					continue;
				}
				if (straightOrderRules[i] <= rule) {
					rule -= straightOrderRules[i];
					reverserOrderRules[i] = 0;
				}
			}
		}
	}
	void RestoreArraysToDefault() {
		for (int i = 0; i < arraysSize; i++) {
			straightOrderRules[i] = pow(2, i);
			reverserOrderRules[i] = pow(2, i);
		}
	}
	void setRule(__int8& inputRule) {
		rule = inputRule;
	}
public:
	int check(__int8 &rule, string &password) {

		setRule(rule);
		ParseRules(rule);

		for (int i = 0; i < arraysSize; i++) {
			switch (straightOrderRules[i]) {
			case 1:
				//Todo написать регул€рки
			case 2:
			case 4:
			case 8:
			case 16:
			case 32:
			case 64:
			default:
				break;
			}
		}
				
		//Todo переключить свчитем значени€
		return 0;
	}
};