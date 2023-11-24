#include <iostream>
#include <string>
#include <libxml\parser.h>
#include <libxml\tree.h>

#define USERPASS "password"
#define USERGROUP "groupId"

#define CONFIG 0
#define USERS 1

class XMLParser {
public:

	XMLParser(const std::string& xmlFile) : xmlFile(xmlFile) {
		xmlInitParser();
		LIBXML_TEST_VERSION;
		doc = xmlReadFile(xmlFile.c_str(), "UTF-8", 0);
		if (doc == nullptr) {
			throw "Error opening XML";
		}
		rootNode = xmlDocGetRootElement(doc);
		if (rootNode == nullptr) {
			xmlFreeDoc(doc);
			throw "NoRootNode";
		}
	}
	~XMLParser() {
		xmlCleanupParser();
	}
	/**
	* ������ �������
	* �������� ���������: <p>
	* Id ����������� �������� <p>
	* ��� ����������� <p>
	* ������������ ���������� ������� ��� ����� <p>
	* �������� ������� ���������� ��� ���������� ���������� ������������ �������
	* @param none
	* @returns void
	* 
	*/
	void ParseConfig() {

	}
	/**
	*	����� ������ ����� �� ��� �����
	* 
	*	@param string ��� ����� 
	*	@returns string ������
	* 
	*	��������� ����������: ����� �� ������ ������, �� � ������ ��� �����������
	*/
	std::string FindPasswordByUserName(std::string *username) {
		xmlNode* currentNode = rootNode;

		if (*username == "") throw "NoUserName";
		std::string UsersPass;

		currentNode = currentNode->children->next;

		UsersPass = ParseXMLUsers(*username, currentNode);

		xmlFreeDoc(doc);
		return UsersPass;
	}
	/**
	* �������� ��� �� XML
	* 
	* @param (string) ��� �����, ���� ��� �����
	* @returns void
	*/
	void DeleteUser() {
		
	}
	/**
	* ���������� ���� � ���
	* ��������� �� ���� 5 �����������
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
			node = FindNode(rootNode, "config");
			if (node == NULL) throw "AddNodeXMLTagNotFound";
			AddConfigParam(node, param1, param2);
			break;
		case USERS:
			node = FindNode(rootNode, "users");
			if (node == NULL) throw "AddNodeXMLTagNotFound";
			AddUser(node, param1, param2, param3);
			break;
		default:
			throw "AddNodeWrongParam";
			return;
		}
	}

private:
	std::string userpassword;
	std::string usergroup;
	std::string* pUserpassword = &userpassword;
	std::string* pUsergroup = &usergroup;
	xmlDocPtr doc = nullptr;
	xmlNodePtr rootNode = nullptr;
	xmlNode* newNode = NULL;
	xmlNode* childNewNode = NULL;
	std::string xmlFile;

	
	/**
	* ��� ������ ���� �� ������ ������, ���� �� ������ ���������� �� ���������
	* � ���� ������� ����������, �� �������� ������� �� ����� � ����� ��� ������
	* ������� ��� ������ ��������
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
	* ��� ����� ��� ������� ������ ����� �� XML
	* 
	* �� ����� ���� �� ������ ����
	*/
	void XMLGetUserDataByPtr(xmlNode* node) {
		xmlNode* currentNode = NULL;

		for (currentNode = node->children; currentNode; currentNode = currentNode->next) {
			if (xmlStrcmp(currentNode->name, BAD_CAST USERPASS) == 0) {
				int size = strlen((char*)currentNode->children->content);
				//std::string res((char*)currentNode->children->content, size);
				*pUserpassword = std::string((char*)currentNode->children->content, size);
				//*userpassword = currentNode->children->content;
				//return currentNode->children->content;
			}
			if (xmlStrcmp(currentNode->name, BAD_CAST USERGROUP) == 0) {
				int size = strlen((char*)currentNode->children->content);
				//std::string res((char*)currentNode->children->content, size);
				*pUsergroup = std::string((char*)currentNode->children->content, size);


				//return currentNode->children->content;
			}
		}
	}

	xmlNode* FindNode(xmlNode* node, const char* name) {
		xmlNode* result;

		if (node == NULL);
		
		while (node) {
			if ((node->type == XML_ELEMENT_NODE) && (xmlStrcmp(node->name, BAD_CAST name) == 0)) {
				return node;
			}
			if (result = FindNode(node->children, name)) return result;
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
	}

	void AddConfigParam(xmlNode* node, const char* header, const char* content) {
		
	}
};