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
    XMLParser asdf("D:/repos/vsrepos/consoleview/consoleview/config.xml");
    string* nigga = new string();
    *nigga = "admin";
    std::string sex = asdf.FindPasswordByUserName(nigga);
    std::cout << sex;
    delete nigga;
    return 0;
}