#pragma once 

#include <vector>
#include <unordered_map>
#include "string"
#include <set>
#include "event.h"
#include "GameSummary.h"

/***
 * Represents the current client data
*/
class UserData {

public:
    // Constructors
    UserData();

    //Destructor
    ~UserData();

    // methods

    /***
     * Getter for the gameSummary by a given username
     * @param username
    */
    GameSummary* getGameSummary(std::string username);

    /**
     * Sets the receipt id to discomment
     * @param updatedReceiptI
    */
    void setReceiptIdToDisconnect(std::string updatedReceiptId);

    // getter

    //Getters
    /**
     * Return the receiptID used to disconnect the client
    */
    std::string getReceiptIdToDisconnect();

    /***
     * Getter for topic name to subscription id association dict
    */
    std::unordered_map<std::string,std::string> &getTopicNameToSubscriptionID();

    /***
     * Getter for Receipt Id to Topic association dict
    */
    std::unordered_map<std::string,std::string> &getReceiptIdToTopic();
    
    /***
     * Getter for Receipt Id to command association dict
    */
    std::unordered_map<std::string,std::string> &getReceiptIdToCommand();
    
    /***
     * Checks whenever user has reported to event  by username
     * @param username
    */
    bool userHasReportedEvents(std::string username);

    /**
     *  Subscribe to topic
     * @param topic
    */
    void addTopic(std::string topic);

    /***
     *   associate  subscribtion to a topic
     * @param topic
     * @param subscriptionId
    */
    void subscribeToTopic( std::string topic,std::string subscriptionId);

    /***
     * @param topic 
     * @return whenever subscribed to topic
    */
    bool isSubscribedToTopic(std::string topic);

    /***
     *   remove association of subscriptionID to a command
     * @param receiptId
     * @param topic
    */
    void removeSubscribeFromTopic(std::string topic);

    /***
     *  Return associated subscription to a given receiptId
     * @param receiptId
    */
    std::string getSubscribeIdByTopic(std::string topic);

    /***
     *   associate  receiptId to a command
     * @param receiptId
     * @param topic
    */
    void addReceiptIdToTopic(std::string receiptId, std::string topic);

    /***
     * Remove receiptID to topic association
     * @param receiptId
    */
    void removeTopicFromReceiptId(std::string receiptId);

    /***
     *  Return associated topic to a given receiptId
     * @param receiptId
    */
    std::string getTopicByReceiptId(std::string receiptId);

    /***
     *   associated command to a given receiptId
     * @param receiptId
     * @param command
    */
    void addReciptIdToCommand(std::string receiptId,std:: string command);
    
    
    /***
     *  Return associated command to a given receiptId
     * @param receiptId
    */
    std::string getCommandByReceiptId(std::string receiptId);


    /***
     * Getter for the username
    */
    const std::string & getUserName() const;

    /***
     * Setter for username
     * @param name
    */
    void setUserName(std::string name);
    
    
    /***
     * Getter for the password
    */
    const std::string & getPassword() const;

    /***
     * Setter for user password
     * @param password
    */
    void  setPassword(std::string password) ;

    /**
     *  Perform client login
    */
    void login();
    
    /**
     * Perform client logout
    */
    void logout();

    /***
     * getter for login lock
    */
    bool isLoginLock() const;

    /***
     * Sets the login lock 
     * @param logOutLock the new value of the lock
    */
    void setLoginLock(bool loginLock);

    /***
     * getter for logout lock
    */
    bool isLogOutLock() const;

    /***
     * Sets the logout lock 
     * @param logOutLock the new value of the lock
    */
    void setLogOutLock(bool logOutLock);


    /***
     * @return whenever clinet
    */
    bool isLoggedIn(); 


    /***
     * Checks whenever a client is subscribed to a given topic
     * @param topic
    */
    bool  isSubscribed(std::string topic);

    /***
     * Generates a unique subscription id.
    */
    int nextSubscriptionId();

    /***
     * Generates a unique recipt id.
    */
    int nextReciptId();


private:

    // data members:  
    std::string username;
    std::string password;
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
};