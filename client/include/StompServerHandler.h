#pragma once

#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "ConnectionHandler.h"

using boost::asio::ip::tcp;

class StompServerHandler {
private:

    ConnectionHandler *connectionHandler;
    
public:
	StompServerHandler();

	virtual ~StompServerHandler();

    // 
    bool isLoggedIn
    bool initConnection(std::string host,int port); 
    void stopConnection(); 
    bool processCommandLogin(std::string username,std::string password);
    
    
    //
    bool processCommandJoin(std::string gameName);
    bool processCommandExit(std::string gameName);
    bool processCommandReport(std::string filename);
    bool processCommandSummary(std::string gameName,std::string user,std::string file);
    bool processCommandLogout();

}; 
