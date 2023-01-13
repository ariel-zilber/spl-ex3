
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include "StompServerHandler.h"

// todo:: change it

StompServerHandler::StompServerHandler(ConnectionHandler &connectionHandler, UserData &userData) : connectionHandler(
        &connectionHandler), userData(&userData) {}

StompServerHandler::StompServerHandler(const StompServerHandler &other) : connectionHandler(), userData() {
    connectionHandler = other.connectionHandler;
    userData = other.userData;
}

StompServerHandler &StompServerHandler::operator=(const StompServerHandler &other) {
    if (this == &other)
        return *this;
    delete connectionHandler;
    delete userData;
    connectionHandler = other.connectionHandler;
    userData = other.userData;
    return *this;
}

// destructor
StompServerHandler::~StompServerHandler() {}


void StompServerHandler::run() {
    std::cout<<"[DEBUG] StompServerHandler::run()"<<std::endl;
    while (connectionHandler->isConnected()) {
        std::cout<<"[DEBUG] StompServerHandler::loop()"<<std::endl;

        std::string message;
        if (!connectionHandler->getLine(message)) {
            std::cout << "Failed to get answer form server, connection lost" << std::endl;
            connectionHandler->close();
            userData->logout();
            userData->setLogOutLock(false);
            break;
        }

        if (message.length() > 0) {
            std::vector<std::string> serverOutputMessage = parseByLine(message);

            if (serverOutputMessage[0] == "CONNECTED") {
                handleConnectedFrame();
            } else if (serverOutputMessage[0] == "RECEIPT") {
                std::string receiptId = serverOutputMessage[1].substr(serverOutputMessage[1].find(':') + 1);
                handleReceiptFrame(receiptId);
            } else if (serverOutputMessage[0] == "ERROR") {
                handleErrorFrame(serverOutputMessage);
            } else if (serverOutputMessage[0] == "MESSAGE") {
                std::string username=serverOutputMessage[5];
                std::string gamename = serverOutputMessage[1].substr(serverOutputMessage[1].find(':') + 1);
                std::string frameBody("");
                for(int i=5;i<(int) serverOutputMessage.size();i++){
                    std::cout<<"[Body] "<<serverOutputMessage[i]<<std::endl;
                    frameBody+=serverOutputMessage[i]+"\n";
                }

                //
                if(username!=userData->getUserName()){
                    handleMessageFrame(username, gamename,frameBody);
                }
                
            }
        }
    }
}

void StompServerHandler::handleMessageFrame(std::string username,std::string gamename,const std::string messageBody) {
    userData->getGameSummary(username)->addEventToGame(gamename,  Event(messageBody));
} 


void StompServerHandler::handleConnectedFrame() {
    userData->login();
    userData->setLoginLock(false);
    std::cout << "Login successful" << std::endl;
}

void StompServerHandler::handleReceiptFrame(std::string receiptId) {
    // todo
    if (receiptId == userData->getReceiptIdToDisconnect()) {
        connectionHandler->close();
        userData->logout();
        userData->setLogOutLock(false);
    } else if (userData->getCommandByReceiptId(receiptId) == "SUBSCRIBE") {
    } else { 
       // case UNSUBSCRIBE
        std::string topic = userData->getTopicByReceiptId(receiptId);
        userData->removeSubscribeFromTopic(topic);
        userData->removeTopicFromReceiptId(receiptId);
    }
}



void StompServerHandler::handleErrorFrame(std::vector<std::string> errorVector) {
    connectionHandler->close();
    userData->logout();

    // print the error message
    for(int i=2;i<(int) errorVector.size();i++){
        if(errorVector.at(i).length()>0){
            std::cout<<errorVector.at(i)<<std::endl;
        }
    }

    //
    userData->setLogOutLock(false);
    userData->setLoginLock(false);
}


  std::vector<std::string> StompServerHandler::parseByLine(std::string message) {
    std::stringstream ss(message);
    std::string line;
    std::vector<std::string> results;
    while (std::getline(ss, line, '\n')) {
        results.push_back(line);
    }
    return results;
}

void StompServerHandler::sendMessage(std::string msg) {
    if (!connectionHandler->sendLine(msg)) {
        connectionHandler->close();
        userData->logout();
        userData->setLogOutLock(false);
        std::cout << "Failed to send message, connection lost" << std::endl;
    }
}
