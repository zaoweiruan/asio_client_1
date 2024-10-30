/*
 * get_post_option.hpp
 *
 *  Created on: 2024年10月10日
 *      Author: dsm
 */

#ifndef SRC_GET_POST_OPTION_HPP_
#define SRC_GET_POST_OPTION_HPP_
#include <string>
using namespace std;
struct request_body{
	string domain;
	string port;
	string requestpage;
	string body;
};
enum retCode{Success,Error,Help};
retCode get_post_option(int ac, char** av,request_body & requestBody);


#endif /* SRC_GET_POST_OPTION_HPP_ */
