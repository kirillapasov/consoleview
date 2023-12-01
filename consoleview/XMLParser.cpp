#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <vector>
#include <libxml\parser.h>
#include <libxml\tree.h>

constexpr auto USERNAME = "username";
constexpr auto USERPASS = "password";
constexpr auto USERGROUP = "group";
constexpr auto USERSTATUS = "status";

constexpr auto USERSTR = "users";
constexpr auto CONFIGSTR = "config";

constexpr auto CONFIG = 0;
constexpr auto USERS = 1;

constexpr auto ADD = 2;
constexpr auto DELETETYPE = 3;

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
	/// <summary>
	/// ������� ��������� �� �������
	/// </summary>
	/// <param name="configSize"> Int ������ �������</param>
	/// <returns>std::vector �������� configSize*2, ��� i - ���������, i+1 - ���������� ��������� </returns>
	std::vector<std::string> ParseConfig(int configSize){

		int xmlConfigSize = GetChildNodesCount(CONFIG);
		if (configSize > xmlConfigSize || configSize < xmlConfigSize) throw "Wrong XML Config Size. Using Defaults";

		xmlNode* node = FindNode(rootNode, ADD, CONFIGSTR);
		//Todo �������� �������� �� nullptr ��� NULL

		std::vector<std::string> result = GetConfigParameters(node);

		return result;
	}
	/// <summary>
	/// �������� ������ ������ ��� ������ � GUI
	/// </summary>
	/// <returns>std::vector �������� UsersCount*3, ��� i - ��������, i+1 - ������ �����, i+2 ������ �����</returns>
	std::vector<std::string> GetUsersList() {

		//Todo ���������� �����
		xmlNode* node = NULL;
		node = FindNode(rootNode, ADD, USERSTR);
		userscount = xmlChildElementCount(node);
		if (userscount > 33) throw "Too many users. Maximum is 33";
		std::vector<std::string> res = GetUsersInfo(node);
		return res;
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

	/// <summary>
	/// ����������� ��� ������ � ����� �� ��� ���������
	/// </summary>
	/// <param name="username">��� ������������ (String)</param>
	/// <returns>std::array �������� 4 � ����������, �������, ������� � ��������</returns>
	std::array<std::string,4> FindUsersDataByUsername(std::string username) {
		xmlNode* node = FindNode(rootNode, ADD, USERSTR);

		if (username == "") throw "NoUserName";
		std::array<std::string, 4> result = GetUsersPassword(username, node);
		return result;
	}
	/**
	* ���������� ���� � XML
	*
	* @param Integer: ��� ���������� (0 - � ������, 1 - � ��������)
	* @param String: Param1 (Config: Header; Users: Username)
	* @param String: Param2 (Config: Argument; Users: Password)
	* @param String: Param3 (Config: None; Users: Group)
	* @returns none
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
			node = FindNode(rootNode, DELETETYPE, CONFIGSTR, param1);
			if (node == NULL) throw "AddNodeXMLTagNotFound";
			DeleteNode(node);
			break;
		case USERS:
			node = FindNode(rootNode, DELETETYPE, USERSTR, param1);
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
	* 
	* @returns none
	 */
	void ModifyNode(int type, const char* param1, const char* param2 = "", const char* param3 = "") {
		DeleteNode(type, param1);
		AddNode(type, param1, param2, param3);
	}
	/// <summary>
	/// ��������� ��� ��������� � XML
	/// </summary>
	void save() {
		SaveFile();
	}
	
private:
	//Todo ��������
	std::string username = "";
	std::string userpassword = "";
	std::string usergroup = "";
	std::string userstatus = "";

	std::string* pUserpassword = &userpassword;
	std::string* pUsergroup = &usergroup;
	std::string* pUsername = &username;
	std::string* pUserstatus = &userstatus;

	xmlDocPtr doc = nullptr;
	xmlNodePtr rootNode = nullptr;
	xmlNode* newNode = NULL;
	xmlNode* childNewNode = NULL;
	std::string xmlFile;
	int userscount = 0;

	//Todo �������� ��� strlen �� strnlen_s
	
	std::array<std::string, 4> GetUsersPassword(std::string username, xmlNode *node) {
		std::array<std::string, 4> res;
		for (int i = 0; i < 4; i++) {
			res[i] = "";
		}

		xmlNode* currentNode = NULL;
		xmlNode* temp = NULL;

		for (temp = node->children; temp; temp = temp->next) {
			for (currentNode = temp->children; currentNode; currentNode = currentNode->next) {
				if (xmlStrcmp(currentNode->name, BAD_CAST USERNAME) == 0) {
					if (xmlStrcmp(currentNode->children->content, BAD_CAST username.c_str()) != 0) continue;
					int size = strlen((char*)currentNode->children->content);
					res[0] = std::string((char*)currentNode->children->content, size);
				}
				if (xmlStrcmp(currentNode->name, BAD_CAST USERPASS) == 0) {
					int size = strlen((char*)currentNode->children->content);
					res[1] = std::string((char*)currentNode->children->content, size);
				}
				if (xmlStrcmp(currentNode->name, BAD_CAST USERGROUP) == 0) {
					int size = strlen((char*)currentNode->children->content);
					res[2] = std::string((char*)currentNode->children->content, size);
				}
				if (xmlStrcmp(currentNode->name, BAD_CAST USERSTATUS) == 0) {
					int size = strlen((char*)currentNode->children->content);
					res[3] = std::string((char*)currentNode->children->content, size);
				}
			}
			if (res[0] != "") break;
		}
		if (res[0] == "") throw "WrongUsername";
		return res;
	}

	/// <summary>
	/// ���������� ��� ������ �� ������������� �����
	/// </summary>
	/// <param name="node">��������� �� ����� � XML</param>
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
			if (xmlStrcmp(currentNode->name, BAD_CAST USERSTATUS) == 0) {
				int size = strlen((char*)currentNode->children->content);
				*pUserstatus = std::string((char*)currentNode->children->content, size);
			}
		}
	}

	/// <summary>
	/// ���������� ��������� ��:
	/// ��������� ���� � ������� ��� ��������
	/// </summary>
	/// <param name="node">��������� �� �������� ���� XML ���������</param>
	/// <param name="type">������ ����� (0 - ��������� ���������, 1 - ������������ � ���������)</param>
	/// <param name="param1">��� ���������, ������� ������ ����� �����</param>
	/// <param name="param2">��� ������������, ������� ������ ����� �����</param>
	/// <param name="param3">Default: none</param>
	/// <returns>��������� �� ��������� ����</returns>
	xmlNode* FindNode(xmlNode* node, int type, const char* param1, const char* param2="", const char* param3="") {
		switch (type) {
		case ADD:
			return AddFind(node, param1);
		case DELETETYPE:
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
	void AddUser(xmlNode* node, const char* username, const char* password, const char* group, const char* status = "null") {
		newNode = xmlNewNode(NULL, BAD_CAST "user");
		childNewNode = xmlNewNode(NULL, BAD_CAST USERNAME);
		xmlNodeSetContent(childNewNode, BAD_CAST username);
		xmlAddChild(newNode, childNewNode);
		childNewNode = xmlNewNode(NULL, BAD_CAST USERPASS);
		xmlNodeSetContent(childNewNode, BAD_CAST password);
		xmlAddChild(newNode, childNewNode);
		childNewNode = xmlNewNode(NULL, BAD_CAST USERGROUP);
		xmlNodeSetContent(childNewNode, BAD_CAST group);
		childNewNode = xmlNewNode(NULL, BAD_CAST USERSTATUS);
		xmlNodeSetContent(childNewNode, BAD_CAST status);
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

	std::vector<std::string> GetUsersInfo(xmlNode* node) {
		std::vector<std::string> res;
		std::string username = "";
		std::string usergroup = "";
		std::string userstatus = "";

		xmlNode* currentNode = node->children;
		for (currentNode = node->children; currentNode; currentNode = currentNode->next) {
			if (currentNode->type == XML_TEXT_NODE) continue;
			XMLGetUserDataByPtr(currentNode);
			if (pUsername == nullptr) continue;
			username = *pUsername;
			usergroup = *pUsergroup;
			userstatus = *pUserstatus;
			res.push_back(username);
			res.push_back(usergroup);
			res.push_back(userstatus);
		}
		return res;
	}
	std::vector<std::string> GetConfigParameters(xmlNode* node) {
		std::vector<std::string> res;
		std::string header = "";
		std::string contents = "";
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