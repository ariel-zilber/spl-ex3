#pragma once
#include "ConnectionHandler.h"
#include "UserData.h"
#include "GameSummary.h"

class StompServerHandler {
public:
    StompServerHandler(ConnectionHandler &connectionHandler, UserData &userData); 
    StompServerHandler(const StompServerHandler& other);
    StompServerHandler& operator=(const StompServerHandler& other); 
    virtual ~StompServerHandler(); 
    static std::vector<std::string> parseByLine(std::string message);

    void run();

private:
    ConnectionHandler *connectionHandler;
    UserData *userData;
    void handleMessageFrame(std::string username,std::string gamename,const std::string messageBody);
    void handleConnectedFrame();
    void handleReceiptFrame(std::string receiptId);
    void handleErrorFrame(std::vector<std::string> errorMessage);
    void sendMessage (std::string msg);

};

