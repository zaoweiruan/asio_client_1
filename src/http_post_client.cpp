/*
 * http_post_client.cpp
 *
 *  Created on: 2024年10月10日
 *      Author: dsm
 */

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/json/src.hpp>
#include <boost/json/value.hpp>
#include "get_post_option.hpp"
#include "GBKtoUTF8.h"
//#include "debugMarco.h"
using namespace boost::gregorian;

using boost::asio::ip::tcp;
using std::string;
static const auto ConsoleOutputCP=GetConsoleOutputCP();
retCode  post(const string& host, const string& port, const string& page, const string& data, string& reponse_data)
{
  try
  {
#ifdef DEBUG
	  std::cout<<"page is :"<<page<<std::endl;
	  std::cout<<"host is :"<<host<<std::endl;
	  std::cout<<"port is :"<<port<<std::endl;
	  std::cout<<"data is :"<<data<<std::endl;
#endif
    boost::asio::io_service io_service;
    //如果io_service存在复用的情况
    if(io_service.stopped())
      io_service.reset();
    //tcp::endpoint ep(boost::asio::ip::address::from_string(host), short(atoi(port.c_str())));
    // 从dns取得域名下的所有ip

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(host, port);
    boost::system::error_code err;
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query,err), end;
	if (err)
	{
		reponse_data="domain not exists\n";
//			cout<<"resolve domain is error :"<<err.category().name()<<" "<<err.value()<<endl;
		return Error;
	}

	tcp::socket socket(io_service);
	tcp::endpoint ep;
	while (endpoint_iterator != end) {
		 ep= *endpoint_iterator++;
		socket.connect(ep,err);
		if (!err) break;
		if (endpoint_iterator == end)
			{
			reponse_data="ip address is not valid\n";
			return Error;
			}
	}
    // 尝试连接到其中的某个ip直到成功


    //boost::asio::connect(socket, endpoint_iterator);
    //socket.connect(ep);
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "POST " << page << " HTTP/1.0\r\n";
    request_stream << "Host: " << host << ":" << port << "\r\n";
    request_stream << "Accept: application/json, text/plain,*/*\r\n";
    request_stream << "Content-Length: " << data.length() << "\r\n";
    request_stream << "Content-Type: application/json;charset=UTF-8\r\n";
	request_stream << "X-APP-ID: ca9ca029de84f1c2156c66f627079efa\r\n";
	request_stream << "X-APP-KEY: f5d452eea2071daace7d228bd899cedf\r\n";
    request_stream << "Connection: keep-alive\r\n\r\n";
    request_stream << data;

    // Send the request.
    boost::asio::write(socket, request);

    // Read the response status line. The response streambuf will automatically
    // grow to accommodate the entire line. The growth may be limited by passing
    // a maximum size to the streambuf constructor.
    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n");

    // Check that response is OK.
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
      reponse_data = "Invalid response";
      return Error;
    }
    // 如果服务器返回非200都认为有错,不支持301/302等跳转
    if (status_code != 200)
    {
      reponse_data = "Response returned with status code "+to_string(status_code);
      return Error;
    }

    // 传说中的包头可以读下来了
    std::string header;
    std::vector<string> headers;
    while (std::getline(response_stream, header) && header != "\r")
      headers.push_back(header);

    // 读取所有剩下的数据作为包体
    boost::system::error_code error;
    while (boost::asio::read(socket, response,
        boost::asio::transfer_at_least(1), error))
    {
    }

    //响应有数据
    if (response.size())
    {
      std::istream response_stream(&response);
      std::istreambuf_iterator<char> eos;
      reponse_data = string(std::istreambuf_iterator<char>(response_stream), eos);
    }

    if (error != boost::asio::error::eof)
    {
      reponse_data = error.message();
      return Error;
    }
  }
  catch(std::exception& e)
  {
    reponse_data = e.what();
      return Error;
  }
  return Success;
}

int main(int argc, char* argv[])
{
	//SetConsoleOutputCP(CP_UTF8);
	//std::locale::global(std::locale(""));
	//cout<<"代码页"<<GetConsoleOutputCP()<<endl;
	request_body RequestBody;
	if (get_post_option(argc,argv,RequestBody)!=Success) return Error;
//  string host = RequestBody.domain;
//  string port = RequestBody.port;
//  string page = RequestBody.requestpage;
//  string data = RequestBody.data;

	string reponse_data;
//#define SHOW_TIME
  retCode ret = post(RequestBody.domain, RequestBody.port, RequestBody.requestpage, RequestBody.body, reponse_data);
  switch (ret)
  {
  case Error :
			{
			std::cout << reponse_data << std::endl;
			break;
			}
  case Help:
	  	  	  break;
	default:
	  {
#ifdef OUTPUT_CONSOLE
		  if  (ConsoleOutputCP!=CP_UTF8)
			  reponse_data=ConvertUtf8ToGBK(reponse_data);
#endif

		  boost::system::error_code ec;

  //if  (!IsTextUTF8(reponse_data)) reponse_data=ConvertGBKToUtf8(reponse_data);
//		  auto decode_val = boost::json::parse(reponse_data,ec).as_object();
		  auto decode_val = boost::json::parse(reponse_data,ec);
//		  boost::json::value  decode_val = boost::json::parse(reponse_data,ec);
//		  std::cout << decode_val<< std::endl;
		  if (ec)
			  {
			  std::cout <<"ec message is :"<< ec.message() << std::endl;
			  return 1;
			  }
		  else
		  {
		  cout<<"decode_val[total]="<<decode_val.at("total")<<endl;
		  cout<<"decode_val[code]="<<decode_val.at("code")<<endl;
		  cout<<"decode_val[msg]="<<decode_val.at("msg")<<endl;

/*		  if (decode_val.is_null()) cout<<"decode_val is null:"<<endl;
		  if (decode_val.is_object()) cout<<"decode_val is object:"<<endl;
		  if (decode_val.is_array()) cout<<"decode_val is array:"<<endl;
		  if (decode_val.is_array()) cout<<"decode_val is array:"<<endl;
		  if (decode_val.at("rows").is_array()) cout<<"decode_val.at(\"row\") is is_array:"<<endl;*/

		  if( auto p = decode_val.at("rows").if_array())
		  {

//			  cout<<"decode_val[row]="<<decode_val.at("rows")<<endl;
			    cout<<"p size:"<< p->size()<<endl;
			  for (size_t i=0;i<p->size();i++)
			  {
		  		      cout<<p->at(i)<<endl;
		  		      cout<<fixed << setprecision(3)<<p->at(i).at("complianceRate").as_double()<<endl;
			  }
/*		      auto opType = p->at("total").as_int64();
		      cout<<opType<<endl;*/
		  }
		  }
//		  std::cout<<decode_val[0]<<endl;

			/*auto opType = decode_val.at("total").as_int64();
			auto pValue = decode_val.if_contains("OP_STR");
			if (pValue)
			{
				if (pValue->as_string().empty())
				{
					// ...
				}
				else
				{
					auto pStr = std::make_shared<std::string>(pValue->as_string().c_str());
					std::cout << pStr << std::endl;
					// ...
				}
			}*/

		  //boost::json::object val2=reponse_data;
		 // std::cout << boost::json::serialize(decode_val) << std::endl;

		  break;
	  }
  }
  return 0;
}


