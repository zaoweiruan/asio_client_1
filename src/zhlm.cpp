/*
 * zhlm.cpp
 *
 *  Created on: 2024年10月11日
 *      Author: dsm
 */
#include <iostream>
#include <boost/program_options.hpp>
#include <windows.h>

namespace po = boost::program_options;
using namespace std;
po::variables_map vm;
bool IsTextUTF8(const std::string& str)
{
	char nBytes=0;//UFT8可用1-6个字节编码,ASCII用一个字节
	unsigned char chr;
	bool bAllAscii = true; //如果全部都是ASCII, 说明不是UTF-8

	for(int i=0; i < str.length();i++)
	{
		chr = str[i];

		// 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,
		// 但用一个字节存,最高位标记为0,o0xxxxxxx
		if( (chr&0x80) != 0 )
			bAllAscii= false;

		if(nBytes==0) //如果不是ASCII码,应该是多字节符,计算字节数
		{
			if(chr>=0x80)
			{
				if(chr>=0xFC&&chr<=0xFD)   nBytes=6;
				else if(chr>=0xF8)         nBytes=5;
				else if(chr>=0xF0)         nBytes=4;
				else if(chr>=0xE0)         nBytes=3;
				else if(chr>=0xC0)         nBytes=2;
				else{
					return false;
				}
				nBytes--;
			}
		}
		else //多字节符的非首字节,应为 10xxxxxx
		{
			if( (chr&0xC0) != 0x80 ){
				return false;
			}
			nBytes--;
		}
	}

	if( nBytes > 0 ) //违返规则
		return false;

	if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
		return false;

	return true;
}
int main(int argc, char** argv)
{
    std::locale::global(std::locale(""));
    SetConsoleOutputCP(CP_UTF8);
    try
    {
        // 声明被支持的选项。
        po::options_description desc("All options", 500);
        desc.add_options()
            ("help,h", ("检查一个文件的数据"))
            ("file,f", po::value<std::string>(), ("需要检查的文件的全路径名"))
            ("type,t", po::value<int>(), ("需要检查的文件的类型：.第一文件；2.第二文件；3.第三文件。4.aaaaaa文件"));

        //解析并保存命令行参数
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        if (vm.count("help"))
        {
            std::cout << desc << "\n";
        }
        if (vm.count("file"))
        {
        	string str=vm["file"].as<string>();
        	const char* GBK_LOCALE_NAME = ".936"; //GBK在windows下的locale name
        	std::string gbk_str {str.c_str()};  //0xCCCC，"烫"的GBK码

        	//构造GBK与wstring间的转码器（wstring_convert在析构时会负责销毁codecvt，
        	// 所以不用自己delete）
        	std::wstring_convert<codecvt_byname<wchar_t, char, mbstate_t>>
        	// 或 std::wstring_convert<std::codecvt<wchar_t, char, mbstate_t>>
        		cv1(new std::codecvt<wchar_t, char, mbstate_t>(GBK_LOCALE_NAME));
        	wstring tmp_wstr = cv1.from_bytes(gbk_str);
        	cout << "file was set to "
        	                 <<str<<" charset is ufo:"<<IsTextUTF8(str) <<"tmp_wstr is:"<< tmp_wstr<< ".\n";
        }
    }
    catch (const std::exception&)
    {
        std::cout << "未知的命令行参数" << "\n";
    }
    return 0;
}



