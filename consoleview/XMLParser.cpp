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
	*	Поиск пароля юзера по его имени
	* 
	*	@param1 (string) Имя юзера 
	*	@returns (string) Пароль
	* 
	*	Возможное расширение: поиск не только пароля, но и токена для авторизации
	*/
	std::string FindPasswordByUserName() {
		std::string UsersPass;
		xmlDocPtr doc = xmlReadFile(xmlFile.c_str(), "UTF-8", 0);

		xmlFreeDoc(doc);
		return UsersPass;
	}
	/**
	* Добавление юзеров
	* @param Username
	* @param Password
	* @param Group
	* 
	* @returns void
	* 
	* Опицонально: добавить уникальный токен
	*/
	void AddUser() {
	}
	/**
	* Удаление юзеров
	* 
	* @param (string) Имя юзера, либо его токен
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
			std::cerr << "Ошибка при загрузке XML-файла." << std::endl;
			return "";
		}

		xmlNodePtr rootNode = xmlDocGetRootElement(doc);

		if (rootNode == nullptr) {
			std::cerr << "Ошибка: корневой элемент не найден." << std::endl;
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
						// Извлекаем текстовое содержимое элемента
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