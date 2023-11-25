#include <iostream>
#include "auth.cpp"
#include "XMLParser.cpp"

using namespace std;

int main()
{
    /*
    Auth<string> auth("nigger", "faggot");
    auth.MakeAuth();
    auth.CheckUSBDrive();
    */
    XMLParser asdf("config.xml");
    string* nigga = new string();
    *nigga = "admin";
    std::string sex = asdf.FindPasswordByUserName(nigga);
    std::string username = "pedik";
    std::string password = "nigger";
    std::string group = "fag";
    std::cout << sex;
    delete nigga;
    asdf.GetUsersList();
    return 0;
}