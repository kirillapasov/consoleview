#include <iostream>
#include <string>
#include <libxml\parser.h>
#include <libxml\tree.h>

#define USERPASS "password"
#define USERGROUP "groupId"

class XMLParser {
public:

	XMLParser(const std::string& xmlFile) : xmlFile(xmlFile) {
		xmlInitParser();
		LIBXML_TEST_VERSION;
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

		if (*username == "") throw "NoUserName";
		std::string UsersPass;
		xmlDocPtr doc = xmlReadFile(xmlFile.c_str(), "UTF-8", 0);

		if (doc == nullptr) {
			throw "Error opening XML";
			return "";
		}
		xmlNodePtr rootNode = xmlDocGetRootElement(doc);
		if (rootNode == nullptr) {
			xmlFreeDoc(doc);
			throw "NoRootNode";
			return "";
		}

		rootNode = rootNode->children->next;

		UsersPass = ParseXMLUsers(*username, rootNode);

		xmlFreeDoc(doc);
		return UsersPass;
	}
	/**
	* ���������� ������
	* @param Username
	* @param Password
	* @param Group
	* 
	* @returns void
	* 
	* �����������: �������� ���������� �����
	*/
	void AddUser() {
	}
	/**
	* �������� ������
	* 
	* @param (string) ��� �����, ���� ��� �����
	* @returns void
	*/
	void DeleteUser() {
		
	}

private:
	std::string userpassword;
	std::string usergroup;
	std::string* pUserpassword = &userpassword;
	std::string* pUsergroup = &usergroup;

	std::string xmlFile;

	void XMLWriteData(const std::string& tagName, const std::string& data) {
		xmlDocPtr doc = xmlReadFile(xmlFile.c_str(), nullptr, 0);
		xmlFreeDoc(doc);
	}

	void XMLDeleteData() {
		xmlDocPtr doc = xmlReadFile(xmlFile.c_str(), nullptr, 0);

		xmlFreeDoc(doc);
	}
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
};