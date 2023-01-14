#pragma once
#include "ConnectionHandler.h"
#include "UserData.h"
#include "GameSummary.h"


/***
 * Handes interaction with STOMP server
 * 
*/
class StompServerHandler {
public:
    // constructors:

    /**
     * @param connectionHandler
     * @param userData
    */
    StompServerHandler(ConnectionHandler &connectionHandler, UserData &userData); 



    /**
     * @param other
    */
    StompServerHandler(const StompServerHandler& other);

    // Destructors:
    virtual ~StompServerHandler(); 

    // opeator overloads
    StompServerHandler& operator=(const StompServerHandler& other); 

    //methods
    static std::vector<std::string> parseByLine(std::string message);

    /**
     * Starts the handler
    */
    void run();

private:

    // methods:

    /***
     * Handles response frame for STOMP server
     * Message response of type message
     * @param username
     * @param gameName
     * @param messageBody the frame message body as a string
     * 
    */
    void handleMessageFrame(std::string username,std::string gamename,const std::string messageBody);


    /***
     * Handles response frame for STOMP server
     * Message response of type CONNECTED
     * 
    */
    void handleConnectedFrame();

    /***
     * Handles response frame for STOMP server
     * Message response of type Receipt
     * @param receiptId the receiptId given by the STOMP server
     * 
    */
    void handleReceiptFrame(std::string receiptId);

    /***
     * Handles response frame for STOMP server
     * Message response of type error
     * @param errorMessage the STOMP frame given as list of the lines of the message
     * 
    */
    void handleErrorFrame(std::vector<std::string> errorMessage);

    /***
     *  Sends a given message to STOMP server
     * @param msg message to send
    */
    void sendMessage (std::string msg);

    // data memebers:
    ConnectionHandler *connectionHandler;
    UserData *userData;

};

