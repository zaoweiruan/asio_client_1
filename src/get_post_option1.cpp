// Copyright Vladimir Prus 2002-2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

/* Shows how to use both command line and config file. */

#include <boost/program_options.hpp>
namespace po = boost::program_options;


#include <iostream>
#include <fstream>
#include <iterator>

#include "get_post_option.hpp"
#include "GBKtoUTF8.h"
using namespace std;
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(os, " ")); 
    return os;
}


retCode get_post_option(int ac, char* av[],request_body & requestBody)
{
	//std::locale::global(std::locale("zh_CN"));
    try {
        int pagenum,pagesize;
        string config_file;
    
        // Declare a group of options that will be 
        // allowed only on command line
        po::options_description generic("Generic options",500);
        generic.add_options()
            ("version,v", "print version string")
            ("help", "produce help message")
            ("config", po::value<string>(&config_file)->default_value("get_post_options.cfg"),
                  "配置文件.")
            ;
    
        // Declare a group of options that will be 
        // allowed both on command line and in
        // config file
        po::options_description config("Configuration",500);
        config.add_options()
			("domain,D", po::value<string>(), "设置域名或IP")
			("port,P", po::value<string>(), "设置端口")
			("requestpage,R", po::value<string>(), "设置请求页")
			("totalmonth,M", po::value<string>(), "设置统计月份")
			("regionname,C", po::value<string>(), "设置统计地市")
			("countyname,N", po::value<string>(), "设置统计区县")
			("pagenum,G", po::value<int>(&pagenum)->default_value(1),
				  "配置单次请求页面数量")
			("pagesize,S", po::value<int>(&pagesize)->default_value(10),
					"配置页面中行数.")
            ;

        // Hidden options, will be allowed both on command line and
        // in config file, but will not be shown to the user.
        po::options_description hidden("Hidden options");
        hidden.add_options()
            ("input-file", po::value< vector<string> >(), "input file")
            ;

        
        po::options_description cmdline_options;
        cmdline_options.add(generic).add(config).add(hidden);

        po::options_description config_file_options;
        config_file_options.add(config).add(hidden);

        po::options_description visible("基站节能数字化配置");
        visible.add(generic).add(config);
        
        po::positional_options_description p;
        p.add("input-file", -1);
        
        po::variables_map vm;
        store(po::command_line_parser(ac, av).allow_unregistered().
              options(cmdline_options).positional(p).run(), vm);
        notify(vm);
        
        if (vm.count("help")) {
            cout << visible << "\n";
            return Help;
        }
        if (vm.count("version")) {
            cout << "基站节能数字化 version 1.0\n";
            return Help;
        }

        ifstream ifs(config_file.c_str());
        if (!ifs)
        {
            cout << "can not open config file: " << config_file << "\n";
            return Error;
        }
        else
        {
            store(parse_config_file(ifs, config_file_options,true), vm);
            notify(vm);
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

        if (vm.count("totalmonth")) {

 //        	char data_array[1024];
 //        	strcmp(data_array,vm["data"].as<string>().c_str());
 //            cout << "data was set to "
 //                 << requestBody.data<< ".\n";
         	string totalmonth=vm["totalmonth"].as<string>();
             if  (!IsTextUTF8(totalmonth)) totalmonth=ConvertGBKToUtf8(totalmonth);

             requestBody.body="{\"totalMonth\":\""+totalmonth+"\"";
 #ifdef DEBUG
             cout << "body  was set to "
                                        <<requestBody.body<<endl;
 #endif
         } else {
             cout << "请求统计月份没设置\n";
             return Error;
         }
         if (vm.count("regionname"))
         {
         	string regionname=vm["regionname"].as<string>();
         	if  (!IsTextUTF8(regionname)) regionname=ConvertGBKToUtf8(regionname);
         	requestBody.body=requestBody.body+",\"regionName\":\""+regionname+"\"";
 #ifdef DEBUG
             cout << "body  was set to "
                                        <<requestBody.body<<endl;
 #endif
         }

         if (vm.count("countyname"))
         {
         	string countyname=vm["countyname"].as<string>();
#ifdef DEBUG
         	cout<<"countyname is "<<countyname<<" conver after is :"<<ConvertGBKToUtf8(countyname)<<endl;
#endif
         	if  (!IsTextUTF8(countyname)) countyname=ConvertGBKToUtf8(countyname);
         	requestBody.body=requestBody.body+",\"countyName\":\""+countyname+"\"";
 #ifdef DEBUG
             cout << "body  was set to "
                                        <<requestBody.body<<endl;
 #endif
         }

         if (vm.count("pagenum"))
         {
         	requestBody.body=requestBody.body+",\"pageNum\":\""+to_string(pagenum)+"\"";
         }
         if (vm.count("pagesize"))
         {
         	requestBody.body=requestBody.body+",\"pageSize\":\""+to_string(pagesize)+"\"}";
         }
 #ifdef DEBUG
             cout << "body  was set to "
                                        <<requestBody.body<<endl;
 #endif
        //cout << "Optimization level is " << opt << "\n";
    }
    catch(exception& e)
    {
        cout << e.what() << "\n";
        return Error;
    }    
    return Success;
}
