#include <iostream>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>


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
	*	@param1 (string) ��� ����� 
	*	@returns (string) ������
	* 
	*	��������� ����������: ����� �� ������ ������, �� � ������ ��� �����������
	*/
	std::string FindPasswordByUserName() {
		std::string UsersPass;
		xmlDocPtr doc = xmlReadFile(xmlFile.c_str(), "UTF-8", 0);

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
	void test() {
		std::string balls = XMLFindByTag("config");
		std::cout << balls;
	}

private:
	std::string xmlFile;

	void XMLWriteData(const std::string& tagName, const std::string& data) {
		xmlDocPtr doc = xmlReadFile(xmlFile.c_str(), nullptr, 0);

		xmlFreeDoc(doc);
	}

	void XMLDeleteData() {
		xmlDocPtr doc = xmlReadFile(xmlFile.c_str(), nullptr, 0);

		xmlFreeDoc(doc);
	}

	std::string XMLFindByTag(const std::string& tagName) {
		xmlDocPtr doc = xmlReadFile(xmlFile.c_str(), nullptr, 0);

		if (doc == nullptr) {
			std::cerr << "������ ��� �������� XML-�����." << std::endl;
			return "";
		}

		xmlNodePtr rootNode = xmlDocGetRootElement(doc);

		if (rootNode == nullptr) {
			std::cerr << "������: �������� ������� �� ������." << std::endl;
			xmlFreeDoc(doc);
			return "";
		}

		xmlNodePtr currentNode = rootNode;
		/*
		while (currentNode != nullptr) {
			if (currentNode->type == XML_ELEMENT_NODE) {
				if (xmlStrcmp(currentNode->name, BAD_CAST tagName.c_str()) == 0) {
					std::cout << "sex";
					xmlNodePtr textNode = currentNode->children;
					if (textNode != nullptr && textNode->type == XML_TEXT_NODE) {
						// ��������� ��������� ���������� ��������
						const char* elementValue = (const char*)xmlNodeGetContent(textNode);
						xmlFreeDoc(doc);
						return std::string(elementValue);
					}
				}
			}
			currentNode = currentNode->next;
		}
		*/

		while (currentNode != nullptr) {
			if (currentNode->type == XML_ELEMENT_NODE) {
				if (xmlStrcmp(currentNode->name, BAD_CAST tagName.c_str()) == 0) {
					
				}
			}
		}
		return "pe";
	}
};