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
    asdf.GetUsersList();
    return 0;
}