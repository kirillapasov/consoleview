#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <libxml\parser.h>
#include <libxml\tree.h>

constexpr auto USERNAME = "username";
constexpr auto USERPASS = "password";
constexpr auto USERGROUP = "group";

constexpr auto USERSTR = "users";
constexpr auto CONFIGSTR = "config";

constexpr auto CONFIG = 0;
constexpr auto USERS = 1;

constexpr auto ADD = 2;
constexpr auto DELETE = 3;

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
			throw "Error: No root node";
		}
	}
	~XMLParser() {
		xmlFreeDoc(doc);
		xmlCleanupParser();
	}
	/**
	* ������ �������
	* @param none
	* @returns void
	* 
	*/
	std::vector<std::string> ParseConfig(int configSize){

		int xmlConfigSize = GetChildNodesCount(CONFIG);
		if (configSize > xmlConfigSize || configSize < xmlConfigSize) throw "Wrong XML Config Size. Using Defaults";

		xmlNode* node = FindNode(rootNode, ADD, CONFIGSTR);
		//Todo �������� �������� �� nullptr ��� NULL

		std::vector<std::string> result = GetConfigParameters(node);

		return result;
	}
	/**
	* �������� ������ ���������� �� XML � ������
	* 
	* 
	*/
	void MakeUsersList() {

		//Todo ���������� �����
		xmlNode* node = NULL;
		node = FindNode(rootNode, ADD, USERSTR);
		userscount = xmlChildElementCount(node);
		dataStruct* toScreenArray = new dataStruct[userscount];

		for (int i = 0; i < userscount; i++) {
			dataStruct* nigga = GetUserInfoToStruct(node);
			toScreenArray[i].username = nigga->username;
			toScreenArray[i].usergroup = nigga->usergroup;
		}

	}
	/// <summary>
	/// ���������� ���������� �������� ����� � ���� ������� ��� ������
	/// 
	/// type = 0 ��� ������ ���������� ���������� � �������
	/// type = 1 ��� ������ ���������� ������
	/// </summary>
	/// <param name="type"> Int ����� ������</param>
	/// <returns></returns>
	int GetChildNodesCount(int type) {
		xmlNode* node = NULL;

		switch (type) {
		case CONFIG:
			node = FindNode(rootNode, ADD, CONFIGSTR);
			return xmlChildElementCount(node);

			break;
		case USERS:
			node = FindNode(rootNode, ADD, USERSTR);
			return xmlChildElementCount(node);
			break;
		}

	}

	/**
	*	����� ������ ����� �� ��� �����
	* 
	*	@param string ��� ����� 
	*	@returns string ������
	* 
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
	* ���������� ���� � XML
	*
	* @param Integer: ��� ���������� (0 - � ������, 1 - � ��������)
	* @param String: Param1 (Config: Header; Users: Username)
	* @param String: Param2 (Config: Argument; Users: Password)
	* @param String: Param3 (Config: None; Users: Group)
	*
	*/
	void AddNode(int type, const char* param1, const char* param2, const char* param3) {
		xmlNode* node = NULL;
		switch (type) {
		case CONFIG:
			node = FindNode(rootNode,ADD, CONFIGSTR);
			if (node == NULL) throw "AddNodeXMLTagNotFound";
			AddConfigParam(node, param1, param2);
			break;
		case USERS:
			node = FindNode(rootNode,ADD, USERSTR);
			if (node == NULL) throw "AddNodeXMLTagNotFound";
			AddUser(node, param1, param2, param3);
			break;
		default:
			throw "AddNodeWrongParam";
			return;
		}
	}
	/**
	* ������� ���� �� �����
	* 
	* @param Integer: ��� �������� (0 - �� ������� / 1 - �� ��������)
	* @param String: Param1 (Cofig: Header; Users: Username)
	*/
	void DeleteNode(int type, const char* param1) {
		xmlNode* node = NULL;
		switch (type) {
		case CONFIG:
			node = FindNode(rootNode, DELETE, CONFIGSTR, param1);
			if (node == NULL) throw "AddNodeXMLTagNotFound";
			DeleteNode(node);
			break;
		case USERS:
			node = FindNode(rootNode, DELETE, USERSTR, param1);
			if (node == NULL) throw "AddNodeXMLTagNotFound";
			DeleteNode(node);
			break;
		default:
			throw "AddNodeWrongParam";
			return;
		}
	}
	/**
	* ����� ����������� ����
	* 
	* @param Integer: ��� (0 - ������ / 1 - �� ��������)
	* @param String: Param1 (Config: Header; Users: Username)
	* @param String: Param2 (Config: Argument; Users: Password)
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
	//Todo Refactor
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
	int userscount = 0;
	struct dataStruct {
		std::string* username;
		std::string* usergroup;
	};

	

	std::string ParseXMLUsers(std::string username, xmlNode *node) {

		xmlNode* currentNode = NULL;
		for (currentNode = node; currentNode; currentNode = currentNode->next) {
			if (currentNode->type == XML_TEXT_NODE) continue;
			if (xmlStrcmp(currentNode->name, (const xmlChar*)USERSTR) == 0) {
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


	void XMLGetUserDataByPtr(xmlNode* node) {
		xmlNode* currentNode = NULL;

		for (currentNode = node->children; currentNode; currentNode = currentNode->next) {
			if (xmlStrcmp(currentNode->name, BAD_CAST USERNAME) == 0) {
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
		childNewNode = xmlNewNode(NULL, BAD_CAST USERNAME);
		xmlNodeSetContent(childNewNode, BAD_CAST username);
		xmlAddChild(newNode, childNewNode);
		childNewNode = xmlNewNode(NULL, BAD_CAST USERPASS);
		xmlNodeSetContent(childNewNode, BAD_CAST password);
		xmlAddChild(newNode, childNewNode);
		childNewNode = xmlNewNode(NULL, BAD_CAST USERGROUP);
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
	//��� ���� �����
	dataStruct* GetUserInfoToStruct(xmlNode* node) {
		dataStruct* user = new dataStruct;
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
	std::vector<std::string> GetConfigParameters(xmlNode* node) {
		std::vector<std::string> res;
		std::string header = "";
		std::string contents = "";
		//xmlNode* currentNode = node->children;
		for (xmlNode* currentNode = node->children; currentNode; currentNode = currentNode->next) {
			if (currentNode->type == XML_TEXT_NODE) continue;
			if (strnlen_s((char*)currentNode->name,64) > 0) header = std::string((char*) currentNode->name, strnlen_s((char*)currentNode->name, 64));
			if (strnlen_s((char*)currentNode->children->content, 64) > 0) contents = std::string((char*)currentNode->children->content, strnlen_s((char*)currentNode->children->content, 64));
			res.push_back(header);
			res.push_back(contents);
		}
		return res;
	}
};