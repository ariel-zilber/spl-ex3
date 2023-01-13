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


class ClientInputHandler {
public:
    ClientInputHandler(); 
    ClientInputHandler(const ClientInputHandler& other); 
    ClientInputHandler& operator=(const ClientInputHandler& other);
    virtual ~ClientInputHandler(); 

    void run();

private:
    UserData *userData;
    ConnectionHandler *connectionHandler;
    StompServerHandler *stompServerHandler;
    thread * stompServerHandlerThread;
   bool connectToServer (vector<std::string> &inputLine);
   void sendMessage (std::string msg);
    void parseLogin(vector<std::string> & inputLine);
    void parseJoin (vector<std::string> & inputLine);
    void parseExit (vector<std::string> & inputLine);
    void parseReport (vector<std::string> & inputLine);
    void parseSummary (vector<std::string> & inputLine);
    void parseLogout ();
    std::vector<std::string> tokenize(std::string lastUserInput);



};