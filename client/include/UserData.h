#pragma once 

#include <vector>
#include <unordered_map>
#include "string"
#include <set>
#include "event.h"
#include "GameSummary.h"

//
class UserData {
private:
    
    std::string username;
    std::string password;

    //
    std::set<std::string> topics;
    std::unordered_map<std::string,std::string> topicNameToSubscriptionID; 
    std::unordered_map<std::string,std::string> receiptIdToTopic; 
    std::unordered_map<std::string,std::string> receiptIdToCommand; 
    std::unordered_map<std::string,GameSummary*> usernameToGameSummary; 

    bool loggedIn;

    int reciptIdCounter;
    bool loginLock;
    bool logOutLock;

    std::string receiptIdToDisconnect;

public:
    // Constructors
    UserData();

    //Destructor
    ~UserData();

    //
    GameSummary* getGameSummary(std::string username);
    //
    void setReceiptIdToDisconnect(std::string updatedReceiptId);

    // getter

    //Getters
    std::string getReceiptIdToDisconnect();
    std::unordered_map<std::string,std::string> &getTopicNameToSubscriptionID();
    std::unordered_map<std::string,std::string> &getReceiptIdToTopic();
    std::unordered_map<std::string,std::string> &getReceiptIdToCommand();
    
    bool userHasReportedEvents(std::string username);

    void addTopic(std::string topic);

    // subscribtion<-> topic relation  
    void subscribeToTopic( std::string topic,std::string subscriptionId);
    bool isSubscribedToTopic(std::string topic);
    void removeSubscribeFromTopic(std::string topic);
    std::string getSubscribeIdByTopic(std::string topic);

    // recipt<-> topic relation    
    void addReceiptIdToTopic(std::string receiptId, std::string topic);
    void removeTopicFromReceiptId(std::string receiptId);
    std::string getTopicByReceiptId(std::string receiptId);

    // recipt<-> command relation    
    void addReciptIdToCommand(std::string receiptId,std:: string command);
    std::string getCommandByReceiptId(std::string receiptId);


    // name related
    const std::string & getUserName() const;

    void setUserName(std::string name);
    
    
    // password related
    const std::string & getPassword() const;
    void  setPassword(std::string password) ;


    //
    void login();
    void logout();

    // login lock related
    bool isLoginLock() const;
    void setLoginLock(bool loginLock);

    // logout lock related
    bool isLogOutLock() const;
    void setLogOutLock(bool logOutLock);

    bool isLoggedIn(); 
    bool  isSubscribed(std::string topic);

    int nextSubscriptionId();
    int nextReciptId();




};