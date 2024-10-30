/*
 * zhlm2.cpp
 *
 *  Created on: 2024年10月11日
 *      Author: dsm
 */

#include <iostream>
#include <locale>
using namespace std;

int main(int argc, char **argv)
{
    ios::sync_with_stdio(false);
    std::locale defaultLoc("");

    std::locale::global(defaultLoc);
    //std::setlocale(std::locale::all, "");
    std::wcout.imbue(defaultLoc);

    std::wcout << L"中文abc" << endl;
    std::cout << "abc" << endl;

    printf("我们 adbdn 166 \n");

    return 0;
}




