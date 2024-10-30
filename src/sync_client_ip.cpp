//
// sync_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2024 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <string>
#include <stdlib.h>
#include <boost/asio.hpp>
#include <stringapiset.h>
using namespace std;
using namespace boost::asio;

	inline WCHAR* UTF8ToUnicode(const char *str) throw()
			 {
			  int i = MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,str,-1,NULL,0);
			  WCHAR   *strUnicode=new   WCHAR[i];
			  MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,str,-1,strUnicode,i);
			  return strUnicode;
			  delete []strUnicode;
			 }

int main(int argc, char* argv[])
{
	SetConsoleOutputCP(CP_UTF8);

  try
  {
  
//    cout<<"command line args num is :"<<argc;
//  	for (int i=0;i<argc;i++)
//  	cout<<"arg index "<<i<<" ="<<argv[i]<<endl;
  	
	if (argc<2){
		cout<<"command lines argement is error"<<endl;
		return 1;
	}
	 	string ip_addr(argv[1]),port;
		string::size_type pos=ip_addr.find(':');
		
		if (pos!=ip_addr.npos)
			{
				port=ip_addr.substr(pos+1,sizeof(ip_addr));
				ip_addr=ip_addr.substr(0,pos);
			}   
			
		io_service service;
		ip::tcp::endpoint ep(ip::address::from_string(ip_addr), short(atoi(port.c_str())));
		ip::tcp::socket sock(service);
		try {    
			sock.connect(ep);
			
		} 
			catch(boost::system::system_error &e) 
				{    
					std::cout << e.code() << std::endl;
						return 1;
					}
  		
    // Try each endpoint until we successfully establish a connection.
    //tcp::socket socket(io_context);
    //boost::asio::connect(socket, endpoints);

    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET " << argv[2] << " HTTP/1.0\r\n";
    request_stream << "Content-Type: application/json;charset=UTF-8 \r\n";
    request_stream << "Host: " << ip_addr << "\r\n";
    request_stream << "Accept: application/json, text/plain, */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Send the request.
    boost::asio::write(sock, request);

    // Read the response status line. The response streambuf will automatically
    // grow to accommodate the entire line. The growth may be limited by passing
    // a maximum size to the streambuf constructor.
    boost::asio::streambuf response;
    std::istream out(&response);
    std::string response_content,myString;
    boost::system::error_code error;
    //bool flag=false;
    while(boost::asio::read(sock, response,
    		//boost::asio::transfer_all(),
			boost::asio::transfer_at_least(1),
			error))
		   {
    			//std::getline(response_stream, header)
    	//cout<<&response;
    			while (std::getline(out,response_content) && response_content!="\r")
    			if ((pos=response_content.find('}'))!=response_content.npos|| (pos=response_content.find('{'))!=response_content.npos)
    				{
//    				if (myString.empty())
//    					response_content=response_content.substr(pos,sizeof(response_content));
    				myString.append(response_content);
    				};
		   };

    //cout<<UTF8ToUnicode(myString.c_str());
    cout<<myString;
				
    if (error != boost::asio::error::eof)
      throw boost::system::system_error(error);
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
  }
	//sock.close()
  return 0;
}
