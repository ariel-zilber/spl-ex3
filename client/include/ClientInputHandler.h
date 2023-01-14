#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "StompServerHandler.h"
#include "ConnectionHandler.h"
#include "UserData.h"
#include "GameSummary.h"

using namespace std;

/***
 * This class is used to handle client input
*/
class ClientInputHandler {
public:

    // constructors
    ClientInputHandler(); 
    ClientInputHandler(const ClientInputHandler& other); 
    ClientInputHandler& operator=(const ClientInputHandler& other);
 
    // Destructors
    virtual ~ClientInputHandler(); 

    // methods
    void run();

private: 

    // methods
    
    /**
     * Connects to server
     * @param  inputLine vector list of lines
    */
    bool connectToServer (vector<std::string> &args);

    /***
     * Send a message to connected server
     * @param  msg the message to send
    */
    void sendMessage (std::string msg);

    /**
     * Parses input arguments for login command
     * Login command is of the format:
     *  login {host:port} {username} {password}
     * @param args input arguments
    */
    void parseLogin(vector<std::string> & args);

    /**
     * Parses input arguments for join command
     * Join command is of the format:
     *  join {game_name}
     * @param args input arguments
    */
    void parseJoin (vector<std::string> & args);

    /**
     * Parses input arguments for exit command
     * exit command is of the format:
     *  exit {game_name}
     * @param args input arguments
    */
    void parseExit (vector<std::string> & args);

    /**
     * Parses input arguments for report command
     * report command is of the format:
     *  report {file}
     * @param args input arguments
    */
    void parseReport (vector<std::string> & args);

       
    /**
     * Parses input arguments for summary command
     * summary command is of the format:
     *  summary {game_name} {user} {file}
     * @param args input arguments
    */
    void parseSummary (vector<std::string> & args);


    /**
     * Parses input arguments for logout command
     * logout command is of the format:
     *  logout }
    */
    void parseLogout ();

    /***
     * Splits a string to list of words split by space
     * @param input 
    */
    std::vector<std::string> tokenize(std::string input);

    // data memebers
    UserData *userData;
    ConnectionHandler *connectionHandler;
    StompServerHandler *stompServerHandler;
    thread * stompServerHandlerThread;
 


};