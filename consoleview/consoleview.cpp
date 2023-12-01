#include <iostream>
#include "auth.cpp"
#include "XMLParser.cpp"

using namespace std;

int main()
{
    Auth ax;
    ax.GetDiskListAndSerialNumbers();
    std::cout << ax.MakeAuthKey(1, ax.GetDiskListAndSerialNumbers());
    return 0;
}