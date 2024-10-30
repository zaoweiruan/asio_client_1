/*
 * http_ssl_example_host.cpp
 *
 *  Created on: 2024年10月9日
 *      Author: dsm
 */
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

using boost::asio::ip::tcp;
using std::placeholders::_1;
using std::placeholders::_2;

enum { max_length = 1024 };
namespace ssl = boost::asio::ssl;
typedef ssl::stream<tcp::socket>  ssl_socket;

int main(int argc, char* argv[])
{
// Create a context that use the defualt paths for
// finding CA certification
		ssl::context ctx(ssl::context::sslv23);
		ctx.set_default_verify_paths();
		std::string hostname="localhost",portname="443";

		// Open a socket and connect it to the remote host.
		boost::asio::io_service my_io_service;
		ssl_socket sock(my_io_service, ctx);
		tcp::resolver reslvr(my_io_service);
		tcp::resolver::query qery(hostname, portname);
		boost::asio::connect(sock.lowest_layer(), reslvr.resolve(qery));
		sock.lowest_layer().set_option(tcp::no_delay(true));

		// Perform SSL handshake and verify the remote host's
		// certification
		sock.set_verify_mode(ssl::verify_peer);
		sock.set_verify_callback(ssl::rfc2818_verification(hostname));
	    if(! SSL_set_tlsext_host_name(sock.native_handle(), hostname.c_str()))
	    {
	        throw boost::system::system_error(
	            ::ERR_get_error(), boost::asio::error::get_ssl_category());
	    }
		sock.handshake(ssl_socket::client);
}

