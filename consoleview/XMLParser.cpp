#include <iostream>
#include <string>
#include <fstream>
#include <libxml\parser.h>
#include <libxml\tree.h>

#define USERPASS "password"
#define USERGROUP "group"

#define USER "users"
#define CFG "config"

#define CONFIG 0
#define USERS 1

#define ADD 2
#define DELETE 3

class XMLParser {
public:

	XMLParser(const std::string& xmlFile) : xmlFile(xmlFile) {
		xmlInitParser();
		LIBXML_TEST_VERSION;
		doc = xmlReadFile(xmlFile.c_str(), "UTF-8", 0);
		if (doc == nullptr) {
			throw "Error opening XML";
		}
		rootNode = xmlDocGetRootElement(doc)->children;
		if (rootNode == nullptr) {
			xmlFreeDoc(doc);
			throw "NoRootNode";
		}
	}
	~XMLParser() {
		xmlFreeDoc(doc);
		xmlCleanupParser();
	}
	/**
	* Парсер конфига
	* Передает программе: <p>
	* Id разрешенных девайсов <p>
	* Тип авторизации <p>
	* Максимальное количество попыток для входа <p>
	* Значение таймера блокировки при превышенни количества неправильных попыток
	* @param none
	* @returns void
	* 
	*/
	void ParseConfig() {

	}
	/**
	* Передача списка юзернеймов из XML в массив
	* 
	* 
	*/
	void GetUsersList() {
		xmlNode* node = NULL;
		node = FindNode(rootNode, ADD, USER);
		userscount = xmlChildElementCount(node);
		userStruct* toScreenArray = new userStruct[userscount];

		for (int i = 0; i < userscount; i++) {
			userStruct* nigga = GetUserInfoToStruct(node);
			toScreenArray[i].username = nigga->username;
			toScreenArray[i].usergroup = nigga->usergroup;
		}

	}
	/**
	*	Поиск пароля юзера по его имени
	* 
	*	@param string Имя юзера 
	*	@returns string Пароль
	* 
	*	Возможное расширение: поиск не только пароля, но и токена для авторизации
	*/
	std::string FindPasswordByUserName(std::string *username) {
		xmlNode* currentNode = rootNode;

		if (*username == "") throw "NoUserName";
		std::string UsersPass;

		currentNode = currentNode->next;

		UsersPass = ParseXMLUsers(*username, currentNode);

		return UsersPass;
	}
	/**
	* Добавление ноды в хмл
	* Принимает на вход 5 аргумментов
	*
	* @param Integer: тип добавления (0 - в конфиг, 1 - в датабазу)
	* @param String: Param1 (Config: Header; Users: Username)
	* @param String: Param2 (Config: Argument; Users: Password)
	* @param String: Param3 (Config: None; Users: Group)
	*
	*/
	void AddNode(int type, const char* param1, const char* param2, const char* param3) {
		xmlNode* node = NULL;
		switch (type) {
		case CONFIG:
			node = FindNode(rootNode,ADD, "config");
			if (node == NULL) throw "AddNodeXMLTagNotFound";
			AddConfigParam(node, param1, param2);
			break;
		case USERS:
			node = FindNode(rootNode,ADD, "users");
			if (node == NULL) throw "AddNodeXMLTagNotFound";
			AddUser(node, param1, param2, param3);
			break;
		default:
			throw "AddNodeWrongParam";
			return;
		}
	}
	/**
	* Удаляет ноду из листа
	* 
	* @param Integer: тип удаления (0 - из конфига / 1 - из датабазы)
	* @param String: Param1 (Cofig: Header; Users: Username)
	*/
	void DeleteNode(int type, const char* param1) {
		xmlNode* node = NULL;
		switch (type) {
		case CONFIG:
			node = FindNode(rootNode, DELETE, "config", param1);
			if (node == NULL) throw "AddNodeXMLTagNotFound";
			DeleteNode(node);
			break;
		case USERS:
			node = FindNode(rootNode, DELETE, "user", param1);
			if (node == NULL) throw "AddNodeXMLTagNotFound";
			DeleteNode(node);
			break;
		default:
			throw "AddNodeWrongParam";
			return;
		}
	}
	/**
	* Метод модификации ноды
	* 
	* @param Integer: тип (0 - конфиг / 1 - из датабаза)
	* @param String: Param1 (Config: Header; Users: Username)
	* @param String: Param2 (Config: None; Users: Password)
	* @param String: Param3 (Config: None; Users: Group)
	 */
	void ModifyNode(int type, const char* param1, const char* param2 = "", const char* param3 = "") {
		DeleteNode(type, param1);
		AddNode(type, param1, param2, param3);
	}
	void save() {
		SaveFile();
	}

private:
	std::string username;
	std::string userpassword;
	std::string usergroup;
	std::string* pUserpassword = &userpassword;
	std::string* pUsergroup = &usergroup;
	std::string* pUsername = &username;
	xmlDocPtr doc = nullptr;
	xmlNodePtr rootNode = nullptr;
	xmlNode* newNode = NULL;
	xmlNode* childNewNode = NULL;
	std::string xmlFile;
	int userscount;
	struct userStruct {
		std::string* username;
		std::string* usergroup;
	};

	
	/**
	* Эта залупа ищет по списку юзеров, пока не найдет совпадение по юзернейму
	* И если находит совпадение, то передает поинтер на место в файле для метода
	* который эти данные достанет
	*/
	std::string ParseXMLUsers(std::string username, xmlNode *node) {

		xmlNode* currentNode = NULL;
		for (currentNode = node; currentNode; currentNode = currentNode->next) {
			if (currentNode->type == XML_TEXT_NODE) continue;
			if (xmlStrcmp(currentNode->name, (const xmlChar*)"users") == 0) {
				node = currentNode->children;
				break;
			}
		}
		for (currentNode = node; currentNode; currentNode = currentNode->next) {
			if (currentNode->type == XML_TEXT_NODE) continue;
			if (xmlStrcmp(currentNode->children->next->children->content, BAD_CAST username.c_str()) == 0) {
				XMLGetUserDataByPtr(currentNode);
				//return XMLGetUserDataByPtr(currentNode);
				return *pUserpassword;
			}
		}
		throw "WrongUsername";
	}

	/**
	* Эта ебала крч достает данные юзера из XML
	* 
	* На входе нода из метода выше
	*/
	void XMLGetUserDataByPtr(xmlNode* node) {
		xmlNode* currentNode = NULL;

		for (currentNode = node->children; currentNode; currentNode = currentNode->next) {
			if (xmlStrcmp(currentNode->name, BAD_CAST "username") == 0) {
				int size = strlen((char*)currentNode->children->content);
				*pUsername = std::string((char*)currentNode->children->content, size);
			}
			if (xmlStrcmp(currentNode->name, BAD_CAST USERPASS) == 0) {
				int size = strlen((char*)currentNode->children->content);
				*pUserpassword = std::string((char*)currentNode->children->content, size);
			}
			if (xmlStrcmp(currentNode->name, BAD_CAST USERGROUP) == 0) {
				int size = strlen((char*)currentNode->children->content);
				*pUsergroup = std::string((char*)currentNode->children->content, size);			
			}
		}
	}
//Вспомогательные методы для доббавления, удаления и измененения
	xmlNode* FindNode(xmlNode* node, int type, const char* param1, const char* param2="", const char* param3="") {
		switch (type) {
		case ADD:
			return AddFind(node, param1);
		case DELETE:
			return DelFind(node, param1, param2);
		default:
			throw "XMLFinderWrongType";
		}
	}

	xmlNode* AddFind(xmlNode* node, const char* name) {
		xmlNode* result;

		if (node == NULL) return NULL;

		while (node) {
			if ((node->type == XML_ELEMENT_NODE) && (xmlStrcmp(node->name, BAD_CAST name) == 0)) {
				return node;
			}
			if (result = AddFind(node->children, name)) return result;
			node = node->next;
		}
		return NULL;
	}
	xmlNode* DelFind(xmlNode* node, const char* param1, const char* param2) {
		xmlNode* result;
		if (node == NULL) return NULL;
		while (node) {
			if ((node->type == XML_ELEMENT_NODE)
				&& (xmlStrcmp(node->name, BAD_CAST param1) == 0)
				&& (xmlStrcmp(node->children->next->children->content, BAD_CAST param2) == 0)) return node;
			if (result = DelFind(node->children, param1, param2)) return result;
			node = node->next;
		}
		return NULL;
	}	
	void AddUser(xmlNode* node, const char* username, const char* password, const char* group) {
		newNode = xmlNewNode(NULL, BAD_CAST "user");
		childNewNode = xmlNewNode(NULL, BAD_CAST "username");
		xmlNodeSetContent(childNewNode, BAD_CAST username);
		xmlAddChild(newNode, childNewNode);
		childNewNode = xmlNewNode(NULL, BAD_CAST "password");
		xmlNodeSetContent(childNewNode, BAD_CAST password);
		xmlAddChild(newNode, childNewNode);
		childNewNode = xmlNewNode(NULL, BAD_CAST "group");
		xmlNodeSetContent(childNewNode, BAD_CAST group);
		xmlAddChild(newNode, childNewNode);

		xmlAddChild(node, newNode);
	}

	void AddConfigParam(xmlNode* node, const char* header, const char* content) {
		newNode = xmlNewNode(NULL, BAD_CAST header);
		xmlNodeSetContent(newNode, BAD_CAST content);
		xmlAddChild(node, newNode);
		SaveFile();
	}
	void DeleteNode(xmlNode* node) {
		if (node == NULL) throw "DeleteNodeEmptyValue";
		xmlUnlinkNode(node);
		xmlFreeNode(node);
	}

	void SaveFile() {
		xmlChar* xmlbuff;
		int buffersize;
		std::ofstream outfile;
		outfile.open("config.xml");
		xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);
		outfile << xmlbuff << std::endl;
		outfile.close();
	}

	userStruct* GetUserInfoToStruct(xmlNode* node) {
		userStruct* user = new userStruct;
		user->usergroup = (std::string*)"";
		user->username = (std::string*)"";

		xmlNode* currentNode = node->children;
		for (currentNode = node->children; currentNode; currentNode = currentNode->next) {
			if (currentNode->type == XML_TEXT_NODE) continue;
			XMLGetUserDataByPtr(currentNode);
			if (pUsername == nullptr) continue;
			user->username = pUsername;
			user->usergroup = pUsergroup;
			return user;
		}
		
	}
};