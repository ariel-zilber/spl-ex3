#include "UserData.h"
#include <string>
#include <unordered_map>
#include <chrono>
#include <ctime>
#include "GameSummary.h"

/// constructors ::::::::::::::::::::::

UserData::UserData(): username(),password(),topics(),topicNameToSubscriptionID(),receiptIdToTopic(),receiptIdToCommand(),usernameToGameSummary(),loggedIn(false),reciptIdCounter(1),loginLock(true),logOutLock(false),receiptIdToDisconnect("-1"){};
UserData::~UserData(){}


/// getters ::::::::::::::::::::::

std::unordered_map<std::string, std::string> & UserData::getTopicNameToSubscriptionID() { return topicNameToSubscriptionID;}
std::unordered_map<std::string, std::string> & UserData::getReceiptIdToTopic() { return receiptIdToTopic;}
std::unordered_map<std::string, std::string> & UserData::getReceiptIdToCommand() { return receiptIdToCommand;}


int UserData::nextSubscriptionId(){
    int  ms = std::time(nullptr);
    std::cout << ms << " seconds since the Epoch\n";
    
    return ms;
};

int UserData::nextReciptId(){
    reciptIdCounter++;
    return reciptIdCounter;
};

bool UserData::isSubscribed(std::string topic) {
    for(auto iterator= topicNameToSubscriptionID.begin();iterator!=topicNameToSubscriptionID.end();iterator++){
        if(iterator->first == topic){
            return true;
        }
    }
    return false;
}

void UserData::addTopic(std::string topic){
    topics.insert(topic);
};



//
void UserData::subscribeToTopic( std::string topic,std::string subscriptionId){
    topicNameToSubscriptionID.insert({topic,subscriptionId});
};

void UserData::removeSubscribeFromTopic(std::string topic){
    topicNameToSubscriptionID.erase(topic);
};

std::string UserData::getSubscribeIdByTopic(std::string topic){
    return topicNameToSubscriptionID.at(topic);
};
bool  UserData::userHasReportedEvents(std::string username){
    return usernameToGameSummary.find(username)!=usernameToGameSummary.end();

};

bool UserData::isSubscribedToTopic(std::string topic){
    return topicNameToSubscriptionID.find(topic)!=topicNameToSubscriptionID.end();
};

GameSummary* UserData::getGameSummary(std::string username){
    if(this->usernameToGameSummary.find(username)==this->usernameToGameSummary.end()){
        GameSummary* gameSummary=new GameSummary();
        this->usernameToGameSummary.insert({username,gameSummary});
    }
    return  this->usernameToGameSummary[username];
}
// recipt<-> topic relation    
void UserData::addReceiptIdToTopic(std::string receiptId, std::string topic){
       receiptIdToTopic.insert({receiptId,topic});
};

void UserData::removeTopicFromReceiptId(std::string receiptId){
       receiptIdToTopic.erase(receiptId);
};

std::string UserData::getTopicByReceiptId(std::string receiptId){
    return  receiptIdToTopic.at(receiptId);
};

// recipt<-> command relation    
void UserData::addReciptIdToCommand(std::string receiptId, std::string command){
    receiptIdToCommand.insert({receiptId,command});
};
std::string  UserData::getCommandByReceiptId(std::string receiptId){
    return receiptIdToCommand.at(receiptId);
};


/// setters ::::::::::::::::::::::
void UserData::setUserName(std::string name) { this->username=name;}
void UserData::setPassword(std::string password) { this->password=password;}
const std::string &UserData::getUserName() const {
    return username;
}

const std::string &UserData::getPassword() const {
    return password;
}

void UserData::login() {
    loggedIn = true;
}

void UserData::logout() {
    this->username="";
    this->password="";
    loggedIn = false;

    this->topicNameToSubscriptionID.clear();
}


//
bool UserData::isLoginLock() const {
    return loginLock;
}

//
void UserData::setLoginLock(bool loginLock) {
    UserData::loginLock = loginLock;
}

//
bool UserData::isLogOutLock() const {
    return loginLock;
}

//
void UserData::setLogOutLock(bool loginLock) {
    UserData::loginLock = loginLock;
}

bool UserData::isLoggedIn(){return this->loggedIn;}





//
void UserData::setReceiptIdToDisconnect(std::string updatedReceiptId){
    receiptIdToDisconnect=updatedReceiptId;
};

//
std::string UserData::getReceiptIdToDisconnect(){
    return receiptIdToDisconnect;
};