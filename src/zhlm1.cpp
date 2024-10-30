/*
 * zhlm1.cpp
 *
 *  Created on: 2024骞?10鏈?11鏃?
 *      Author: dsm
 */



#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <fstream>
using namespace std;
int main(int argc, char *argv[])
{
   //std::wstring str = L"123,中文";
	string str1 ={(char)0xcc,(char)0xcc,(char)0xd1,(char)0xcf};
	cout<<str1<<endl;
//  wsstring str="烫";
	//std::wstring str = {'0xcc','0xcc',0x00};
//     wstring_convert<std::codecvt_utf8<char_t>> conv;
//   string narrowStr = conv.to_bytes(str1);
//   cout<<narrowStr<<endl;
//   std::wstring wideStr = conv.from_bytes(narrowStr);
//   {
//      //std::locale::global(std::locale("Chinese-simplified"));
//      std::locale::global(std::locale(""));
//      std::wofstream ofs (L"testW.txt");
//      ofs << wideStr;
//   }
}
