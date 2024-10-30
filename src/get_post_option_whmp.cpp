// Copyright Vladimir Prus 2002-2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

/* The simplest usage of the library.
 */

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>
#include "get_post_option.hpp"
#include "GBKtoUTF8.h"
using namespace std;
retCode get_post_option(int ac, char* av[],request_body & requestBody){


   try {
	   //std::locale::global(std::locale(""));
        po::options_description desc("Allowed options",500);
        desc.add_options()
            ("help,H", "产品帮助")
            ("domain,D", po::value<string>(), "设置域名或IP")
			("port,P", po::value<string>(), "设置端口")
			("requestpage,R", po::value<string>(), "设置请求页")
			("body,B", po::value<string>(), "设置请求体")
		;

        po::variables_map vm;        
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);    
            cout << desc << "\n";

        if (vm.count("help")) {
        	return Help;
        }

        if (vm.count("domain")) {

            requestBody.domain=vm["domain"].as<string>();
            if  (!IsTextUTF8(requestBody.domain)) requestBody.domain=ConvertGBKToUtf8(requestBody.domain);
#ifdef DEBUG
             cout << "domain was set to "
                 << requestBody.domain << ".\n";
#endif
        } else {
            cout << "域名或IP没设置";
            return Error;
        }
        if (vm.count("port")) {

            requestBody.port=vm["port"].as<string>();
            if  (!IsTextUTF8(requestBody.port)) requestBody.port=ConvertGBKToUtf8(requestBody.port);
#ifdef DEBUG
            cout << "port was set to "
                 << requestBody.port << ".\n";
#endif
        } else {
            cout << "端口没设置.\n";
            return Error;
        }
        if (vm.count("requestpage")) {

            requestBody.requestpage=vm["requestpage"].as<string>();
            if  (!IsTextUTF8(requestBody.requestpage)) requestBody.requestpage=ConvertGBKToUtf8(requestBody.requestpage);
#ifdef DEBUG
            cout << "requestpage was set to "
                 << requestBody.requestpage << ".\n";
#endif
        } else {
            cout << "请求页没设置\n";
            return Error;
        }
        if (vm.count("body")) {

//        	char data_array[1024];
//        	strcmp(data_array,vm["data"].as<string>().c_str());
//            cout << "data was set to "
//                 << requestBody.data<< ".\n";

            requestBody.body=vm["body"].as<string>();
            if  (!IsTextUTF8(requestBody.body)) requestBody.body=ConvertGBKToUtf8(requestBody.body);
#ifdef DEBUG
            cout << "data  was set to "
                                       <<requestBody.body<<endl;
#endif
        } else {
            cout << "请求体没设置\n";
            return Error;
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return Error;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
        return Error;
    }

    return Success;
}
