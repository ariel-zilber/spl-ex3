
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
        std::cout<<"message:"<<message<<std::endl;

        if (message.length() > 0) {
            std::vector<std::string> serverOutputMessage = parseByLine(message);
            if (serverOutputMessage[0] == "CONNECTED") {
                handleConnectedFrame();
            } else if (serverOutputMessage[0] == "RECEIPT") {
                std::string receiptId = serverOutputMessage[1].substr(serverOutputMessage[1].find(':') + 1);
                handleReceiptFrame(receiptId);
            } else if (serverOutputMessage[0] == "ERROR") {
                std::string errorMessage = serverOutputMessage[2].substr(serverOutputMessage[1].find(':') - 1);
                handleErrorFrame(errorMessage);
            } else if (serverOutputMessage[0] == "MESSAGE") {
                std::string msgBody = serverOutputMessage[5];
                std::string topic = serverOutputMessage[3].substr(serverOutputMessage[3].find(':') + 1);
                std::cout << std::string(topic + ":" + msgBody) << std::endl;
                handleMessageFrame(topic, msgBody);
            }
        }
    }
}

void StompServerHandler::handleMessageFrame(std::string topic, std::string msgBody) {
    //todo
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

void StompServerHandler::handleErrorFrame(std::string errorMessage) {
    connectionHandler->close();
    userData->logout();
    std::cout << errorMessage << std::endl;
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
