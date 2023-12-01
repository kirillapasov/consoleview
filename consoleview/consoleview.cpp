#include <iostream>
#include "auth.cpp"
#include "XMLParser.cpp"

using namespace std;

int main()
{
    XMLParser asdf("config.xml");
    asdf.FindUsersDataByUsername(std::string("churka",6));
    return 0;
}