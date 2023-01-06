#pragma once

#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "StompServerHandler.h"

using boost::asio::ip::tcp;

class KeyboardHandler {
private:
    StompServerHandler handler;
public:
	KeyboardHandler();
std::vector<std::string>  tokenizer( const std::string& p_pcstStr, char delim );
	virtual ~KeyboardHandler();

    void login();
    void run();


}; 
