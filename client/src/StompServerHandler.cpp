
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include "StompServerHandler.h"
#include "string.h"

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
    while (connectionHandler->isConnected()) {

        std::string message;
        if (!connectionHandler->getLine(message)) {
            if(userData->isLoggedIn()){
                std::cout << "Failed to get answer from server, connection lost" << std::endl;
            }
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
                std::string username=serverOutputMessage[5].substr(serverOutputMessage[5].find(':') + 2);
                std::string gamename = serverOutputMessage[1].substr(serverOutputMessage[1].find(':') + 1);
                std::string frameBody("");
                for(int i=5;i<(int) serverOutputMessage.size();i++){
                    frameBody+= serverOutputMessage[i] +"\n";
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
    std::cout << "\tLogin successful"<<std::endl;
    userData->login();
    userData->setLoginLock(false);
}

void StompServerHandler::handleReceiptFrame(std::string receiptId) {
    if (receiptId == userData->getReceiptIdToDisconnect()) {
        connectionHandler->close();
        userData->logout();
        userData->setLogOutLock(false);
    } else if (userData->getCommandByReceiptId(receiptId) == "SUBSCRIBE") {
        std::cout<<"\tJoined channel "<<userData->getTopicByReceiptId(receiptId)<<std::endl;
    } else { 
       // case UNSUBSCRIBE
        std::string topic = userData->getTopicByReceiptId(receiptId);
        std::cout<<"\tExited channel "<<topic<<std::endl;
        userData->removeSubscribeFromTopic(topic);
        userData->removeTopicFromReceiptId(receiptId);
    }
}

std::string extractErrorDescriptionFromLines(std::vector<std::string> errorVector){
    int numOfSeperators=0;
    std::string output="";
    for(int i=0;i<(int) errorVector.size();i++){
        std::string line=errorVector[i];
        if(numOfSeperators==2){
            output+=line;  
        }
        if(line=="-----"){
                numOfSeperators+=1;
        }
        }
    return output;
}

void StompServerHandler::handleErrorFrame(std::vector<std::string> errorVector) {
    std::cout<<"\t"<<extractErrorDescriptionFromLines(errorVector)<<"\nstomp$ ";
    connectionHandler->close();
    userData->logout();
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
        std::cout << "\tFailed to send message, connection lost" << std::endl;
        connectionHandler->close();
        userData->logout();
        userData->setLogOutLock(false);
    }
}
