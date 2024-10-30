/*
 * example.cpp
 *
 *  Created on: 2024年9月26日
 *      Author: dsm
 */

#include <iostream>
#include "HttpBoost.h"
using namespace std;
int main(int argc, char ** argv) {

//	 SetConsoleOutputCP(CP_UTF8);
//	std::string str("http://192.168.104.47:8080/common/easy/sm_switch/8694");
//
//	str = cly::get(str);
//	std::cout << str.c_str() << std::endl;


	str = "http://192.168.104.47:8080/common/easy/sm_aircond_energy/list/[totalMonth=2024-03&regionName=合肥&pageNum=1&pageSize=10]";
	str = cly::post(str);
	std::cout << str.c_str() << std::endl;

	return 0;
}


