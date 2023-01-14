#include <string>
#include <iostream>
#include <vector>
#include "ClientInputHandler.h"
#include <thread>
#include <sstream>
#include "event.cpp"
#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>

// constructors
ClientInputHandler::ClientInputHandler(): userData(nullptr),connectionHandler(nullptr),stompServerHandler(nullptr),stompServerHandlerThread(nullptr)   {
    userData = new UserData;
    connectionHandler = new ConnectionHandler();
    stompServerHandler=new StompServerHandler(*connectionHandler,*userData);
    stompServerHandlerThread = new thread(&StompServerHandler::run, *stompServerHandler);
}

// destructors
ClientInputHandler::~ClientInputHandler() {
    stompServerHandlerThread->join();
    delete stompServerHandlerThread;
    stompServerHandlerThread=nullptr;
    //
    if(stompServerHandler!=nullptr){
        delete this->stompServerHandler;
        this->stompServerHandler=nullptr;
    }
    //
    if(connectionHandler!=nullptr){
        delete this->connectionHandler;
        this->connectionHandler=nullptr;
    }
    //
    if(userData!=nullptr){
        delete this->userData;
        this->userData=nullptr;
    }

}

// methods
vector<string> ClientInputHandler::tokenize(string input) {
    std::istringstream iss(input);
    vector<string> results(istream_iterator<string>{iss},
                           istream_iterator<string>());
    return results;
};
void ClientInputHandler::run() {
    vector<std::string> enteredUserCommand;
    std::string rawUserInput;
    bool exit=false;
    
    // print welcome message
    std::cout<< "###########################################################"<<std::endl;
    std::cout<< "#                    Welcome                              #"<<std::endl;
    std::cout<< "# --------------------------------------------------------#"<<std::endl;
    std::cout<< "# This is a stomp client for STOMP protocol v1.2          #"<<std::endl;
    std::cout<< "# --------------------------------------------------------#"<<std::endl;
    std::cout<< "# Commands:                                               #"<<std::endl;
    std::cout<< "#    login {host:port} {username} {password}              #"<<std::endl;
    std::cout<< "#    join {game_name}                                     #"<<std::endl;
    std::cout<< "#    exit {game_name}                                     #"<<std::endl;
    std::cout<< "#    report {file}                                        #"<<std::endl;
    std::cout<< "#    summary {game_name} {user} {file}                    #"<<std::endl;
    std::cout<< "#    logout                                               #"<<std::endl;
    std::cout<< "###########################################################"<<std::endl;
    std::cout<< "\tEnter your commands:"<<std::endl;


    // take input from user
    while(!exit){
    std::cout<<"stomp$ ";

        // should run until client has loggedin
        while(!userData->isLoggedIn() && !exit ){
            userData->setLoginLock(true);
            getline(cin, rawUserInput);
            enteredUserCommand = tokenize(rawUserInput);
            if(enteredUserCommand.size()>0 ){
            if(enteredUserCommand[0]=="login" &&enteredUserCommand.size()==4){
                if(connectToServer(enteredUserCommand)){
                    parseLogin(enteredUserCommand);
                    while(userData->isLoginLock()){}
                }else{
                    std::cout<<"\tCould not connect to server.Please try again"<<std::endl;
                }
            } else if(enteredUserCommand[0]=="logout"){
                exit=true;
            }
            else{
                std::cout<<"\tPlease enter login command\nstomp$ ";
            }
            }else{
                std::cout<<"\tPlease enter login command\nstomp$ ";
            }
        }

    while(userData->isLoggedIn()){
        
        // print prompt
        std::cout<<"stomp$ ";

        // take user input
        getline(cin, rawUserInput);
        enteredUserCommand = tokenize(rawUserInput);
        if(enteredUserCommand.size()>0){
        if(enteredUserCommand[0]=="login"){
            std::cout<<"\tThe client is already logged in, log out before trying again"<<std::endl;
        }
        else if(enteredUserCommand[0]=="join"){
            parseJoin(enteredUserCommand);
        }
        else if(enteredUserCommand[0]=="exit"){
            parseExit(enteredUserCommand);
        }
        else if(enteredUserCommand[0]=="report"){
            parseReport(enteredUserCommand);
        }
        else if(enteredUserCommand[0]=="logout"){
            parseLogout();
        }
        else if(enteredUserCommand[0]=="summary"){
            parseSummary(enteredUserCommand);
        }
        else if(enteredUserCommand[0]=="join"){
            parseJoin(enteredUserCommand);
        }else{
            std::cout<<"\tPlease enter a valid command"<<std::endl;
        }
        
        while (userData->isLogOutLock()) {}
        } 
    }


    }

    std::cout<<"\tWill now exit the program"<<std::endl;
}
void ClientInputHandler::sendMessage(string msg) {
    if (!connectionHandler->sendLine(msg)) {
        connectionHandler->close();
        userData->logout();
        userData->setLogOutLock(false);
        std::cout << "\tFailed to send message, connection lost" << std::endl;
    }
}
void ClientInputHandler::parseLogin(vector<std::string> & args){
    
    // valid argument count check
    if(args.size()!=4){
        std::cout<<"\tWrong number of agruments were given!."<<std::endl;
        return;
    }

    // set login creds
    userData->setUserName(args[2]);
    userData->setPassword(args[3]);


    // build stomp message string
    string output = string("CONNECT") + '\n'
                    + string("accept-version:1.2") + '\n'
                    + string("host:stomp.cs.bgu.ac.il") + '\n'
                    + string("login:") + userData->getUserName() + '\n'
                    + string("passcode:") + userData->getPassword() + '\n'+'\0';
    
    // send the message
    sendMessage(output);
};
void ClientInputHandler::parseJoin (vector<std::string> & args){

    // valid argument count check
    if(args.size()!=2){
        std::cout<<"\tWrong number of agruments were given!."<<std::endl;
        return;
    }

    // extract input args
    string topicName = args[1];

    // check input validity
    if(userData->isSubscribed(topicName)){
        std::cout << "\tClient is already subscribed to the topic!!" << std::endl;
        return;
    }

    // get the receipt Id 
    string receiptId = std::to_string(userData->nextReciptId());
    
    // get the subscription id 
    string subscriptionId = std::to_string(userData->nextSubscriptionId());
    
    // save receipt id
    userData->addReceiptIdToTopic(receiptId, topicName);

    // save the name of the topic when not exists
    userData->addTopic(topicName);

    // subscribe the user to the topic
    userData->subscribeToTopic(topicName, subscriptionId);

    // save the command
    userData->addReciptIdToCommand(receiptId, "SUBSCRIBE");
    
    // decode msg
    string output = string("SUBSCRIBE\ndestination:") + topicName + '\n'
                    + string("id:") + subscriptionId + '\n'
                    + string("receipt:") + receiptId + '\n' + '\0';
    
    // send the message
    sendMessage(output);
};
void ClientInputHandler::parseExit (vector<std::string> & inputLine){
 // valid argument count check
    if(inputLine.size()!=2){
        std::cout<<"\tWrong number of agruments were given!."<<std::endl;
        return;
    }
    std::string topicName = inputLine[1];

    // get the subscription id 
    if(userData->isSubscribedToTopic(topicName)){

    // get the receipt Id 
    std::string receiptId = std::to_string(userData->nextReciptId());
    
    std::string subscriptionId = userData->getSubscribeIdByTopic(topicName);

    //
    userData->addReceiptIdToTopic(receiptId, topicName);
    userData->addReciptIdToCommand(receiptId, "UNSUBSCRIBE");

    //
    std::string output = string("UNSUBSCRIBE") + '\n'
                    + string("id:") + subscriptionId + '\n'
                    + string("receipt:") + receiptId + '\n' + '\0';
    
    //
    sendMessage(output);
    }else{
        std::cout<<"\tNot subscribed to the game"<<std::endl;
        
    }
};
void ClientInputHandler::parseReport (vector<std::string> & inputLine){
     // valid argument count check
    if(inputLine.size()!=2){
        std::cout<<"\tWrong number of agruments were given!."<<std::endl;
        return;
    }
    std::string file = inputLine[1];
    names_and_events nne = parseEventsFile(file);
    std::string teamA=nne.team_a_name;
    std::string teamB=nne.team_b_name;

    // //
    transform(teamA.begin(), teamA.end(), teamA.begin(), ::tolower);
    transform(teamB.begin(), teamB.end(), teamB.begin(), ::tolower);
    std::string gameName=teamA+"_"+teamB;
    std::string username=userData->getUserName();
    // // 

    //  
    if(!userData->isSubscribedToTopic(gameName)){
        std::cout<<"\tNot subscribe to game!!"<<std::endl;
        return;
    }

    // save the events in the corrent location
    for(int i=0;i<(int )nne.events.size();i++){
        this->userData->getGameSummary(username)->addEventToGame(gameName,nne.events[i]);
    }    
    std::string output;
    // send all the requests
    for(int i=0;i<(int )nne.events.size();i++){
        Event event=nne.events[i];
        const std::map<std::string, std::string> & gameUpdates=event.get_game_updates();
        const std::map<std::string, std::string> & teamAUpdates=event.get_team_a_updates();
        const std::map<std::string, std::string> & teamBUpdates=event.get_team_b_updates();
         output="SEND\ndestination:"+gameName+
                "\n"+"\nuser: "+username+
                "\nteam a: "+teamA+
                "\nteam b: "+teamB+"\nevent name: "+event.get_name()+
                "\ntime: "+ std::to_string(event.get_time())+"\ngeneral game updates:\n";
                for(auto iter = gameUpdates.begin(); iter !=gameUpdates.end(); ++iter)
                {
                    std::string category =  iter->first;
                    std::string updatedValue =  iter->second;
                    output+="    "+category+": "+updatedValue+"\n";
                }

                //
                output+="team a updates:\n";
                for(auto iter = teamAUpdates.begin(); iter != teamAUpdates.end(); ++iter)
                {
                    std::string category =  iter->first;
                    std::string updatedValue =  iter->second;
                    output+="    "+category+": "+updatedValue+"\n";
                }
                
                //
                output+="team b updates:\n";
                for(auto  iter = teamBUpdates.begin(); iter != teamBUpdates.end(); ++iter)
                {
                    std::string category =  iter->first;
                    std::string updatedValue =  iter->second;
                    output+="    "+category+": "+updatedValue+"\n";
                }

                //
                output+="description:\n";
                output+=event.get_discription();
                output+='\n';
                output+='\0';
                sendMessage(output);
    }    

};
void ClientInputHandler::parseSummary (vector<std::string> & args){
     // valid argument count check
    if(args.size()!=4){
        std::cout<<"\tWrong number of agruments were given!."<<std::endl;
        return;
    }

    // parse user arguemnts
    std::string gameName =args[1];
    std::string user = args[2];
    std::string file = args[3];
    
    // checks of validity
    if(!userData->userHasReportedEvents(user)){
        std::cout<<"\tUser has not reported any game events!!"<<std::endl;
        return;
    }
    if(!userData->getGameSummary(user)->hasSummary(gameName)){
        std::cout<<"\tNot subscribed to game!!"<<std::endl;
        return;
    }

    // create string command
    std::string output;
    GameSummary* userGameSummary=userData->getGameSummary(user);
    std::vector<Event> events=userGameSummary->getEvents(gameName);
    output+=userGameSummary->firstTeam(gameName)+" vs "+userGameSummary->secondTeam( gameName)+"\n";
    output+="Game stats:\n";
    output+="General stats:\n";
    output+=userGameSummary->getGeneralStats(gameName);
    output+=userGameSummary->firstTeam(gameName)+" stats:\n";
    output+=userGameSummary->getTeamAStats(gameName);
    output+=userGameSummary->secondTeam(gameName)+" stats:\n";
    output+=userGameSummary->getTeamBStats(gameName);
    output+="Game event reports:\n";
    output+=userGameSummary->getEventsReport(gameName);

    // save output to file
    ofstream myfile(file);
    myfile << output;
    myfile.close();
};
void ClientInputHandler::parseLogout(){
    
    // get the receipt Id 
    std::string receiptId = std::to_string(userData->nextReciptId());
    userData->setReceiptIdToDisconnect(receiptId);
    
    //
    std::string output = string("DISCONNECT") + '\n'
                    + string("receipt:") + receiptId + '\n' + '\0';
    std::cout<<"\tlogged out"<<std::endl;
    
    //
    sendMessage(output);
};
bool ClientInputHandler::connectToServer (vector<std::string> &args){
    string hostPortStr = args[1];

    // given host from client
    string host = hostPortStr.substr(0, hostPortStr.find(':'));
    
    // given port from client
    short port = short(stoi(hostPortStr.substr(hostPortStr.find(':') + 1, hostPortStr.size())));

    //
    if (connectionHandler->connect(host, port)) {
        stompServerHandlerThread->join();
        delete stompServerHandlerThread;
        stompServerHandlerThread = new thread(&StompServerHandler::run, *stompServerHandler);
        return true;
    }
    return false;
}