/*
 * http_ssl_example_host1.cpp
 *
 *  Created on: 2024年10月10日
 *      Author: dsm
 */

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

using boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
using SSLSocket = ssl::stream<tcp::socket>;


int main() {
    boost::asio::io_context context;
    ssl::context ssl_context(ssl::context::tls);

    ssl_context.set_default_verify_paths();

    SSLSocket socket(context, ssl_context);

    tcp::resolver r(context);
    std::string hostname = "localhost";
    auto endpoints = r.resolve(hostname, "443");
    boost::asio::connect(socket.next_layer(), endpoints);
    socket.set_verify_mode(ssl::verify_peer);
    socket.set_verify_callback(ssl::host_name_verification(hostname));

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if(! SSL_set_tlsext_host_name(socket.native_handle(), hostname.c_str()))
    {
        throw boost::system::system_error(
            ::ERR_get_error(), boost::asio::error::get_ssl_category());
    }

    socket.handshake(SSLSocket::client);
}


