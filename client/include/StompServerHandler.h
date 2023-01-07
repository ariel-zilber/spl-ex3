#pragma once
#include "ConnectionHandler.h"
#include "UserData.h"

class StompServerHandler {
public:
    StompServerHandler(ConnectionHandler &connectionHandler, UserData &userData); 
    StompServerHandler(const StompServerHandler& other);
    StompServerHandler& operator=(const StompServerHandler& other); 
    virtual ~StompServerHandler(); 

    void run();

private:
    ConnectionHandler *connectionHandler;
    UserData *userData;
    void handleMessageFrame(std::string topic, std::string msgBody);
    void handleConnectedFrame();
    void handleReceiptFrame(std::string receiptId);
    void handleErrorFrame(std::string errorMessage);
    std::vector<std::string> parseByLine(std::string message);
    void sendMessage (std::string msg);

};

