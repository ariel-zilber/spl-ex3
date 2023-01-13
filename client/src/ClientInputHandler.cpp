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
    //
    if(connectionHandler!=nullptr){
        delete this->connectionHandler;
    }
    //
    if(userData!=nullptr){
        delete this->userData;
    }
}

vector<string> ClientInputHandler::tokenize(string lastUserInput) {
    std::istringstream iss(lastUserInput);
    vector<string> results(istream_iterator<string>{iss},
                           istream_iterator<string>());
    return results;
};


void ClientInputHandler::run() {
    std::cout<< "Welcome to stomp client 1.2"<<std::endl;
    std::cout<< "\nCommands:"<<std::endl;
    std::cout<< "\tlogin {host:port} {username} {password}  "<<std::endl;
    std::cout<< "\tjoin {game_name}                         "<<std::endl;
    std::cout<< "\texit {game_name}                         "<<std::endl;
    std::cout<< "\treport {file}                            "<<std::endl;
    std::cout<< "\tsummary {game_name} {user} {file}        "<<std::endl;
    std::cout<< "\tlogout                                   "<<std::endl;
    std::cout<< "Enter commands:"<<std::endl;
    std::cout<< ""<<std::endl;
    vector<std::string> enteredUserCommand;
    std::string rawUserInput;

    bool exit=false;
    while(!exit){

        // should run until client has loggedin
        while(!userData->isLoggedIn() && !exit ){
            userData->setLoginLock(true);
            
            //  xx
            getline(cin, rawUserInput);
            enteredUserCommand = tokenize(rawUserInput);
            if(enteredUserCommand.size()>0 ){

            if(enteredUserCommand[0]=="login"){
                if(connectToServer(enteredUserCommand)){
                    parseLogin(enteredUserCommand);
                    std::cout<<"[DEBUG] YES!!! 1"<<std::endl;
                    while(userData->isLoginLock()){}
                    std::cout<<"[DEBUG] YES!!! 2"<<std::endl;
                }else{
                    std::cout<<"Could not connect to server.Please try again"<<std::endl;
                }
            } else if(enteredUserCommand[0]=="logout"){
                exit=true;
            }
            else{
                    std::cout<<"Pleas enter login command of the format: login {host:port} {username} {password}  "<<std::endl;
            }
        
            }else{
                    std::cout<<"Pleas enter login command of the format: login {host:port} {username} {password}  "<<std::endl;
            }
        
        
        }

    std::cout<<"[DEBUG] loggedin"<<std::endl;

    while(userData->isLoggedIn()){
        getline(cin, rawUserInput);
        enteredUserCommand = tokenize(rawUserInput);
        if(enteredUserCommand.size()>0){
        if(enteredUserCommand[0]=="login"){
            std::cout<<"The client is already logged in, log out before trying again"<<std::endl;
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
            std::cout<<"Please enter a valid command"<<std::endl;
        }
        while (userData->isLogOutLock()) {}
        }else{
            std::cout<<"Pleas enter command"<<std::endl;
        }
    }


    }
    std::cout<<"Will now exit the program"<<std::endl;


}

void ClientInputHandler::sendMessage(string msg) {
    if (!connectionHandler->sendLine(msg)) {
        connectionHandler->close();
        userData->logout();
        userData->setLogOutLock(false);
        std::cout << "Failed to send message, connection lost" << std::endl;
    }
}
void ClientInputHandler::parseLogin(vector<std::string> & inputLine){
    userData->setUserName(inputLine[2]);
    userData->setPassword(inputLine[3]);
    // decode msg
    string output = string("CONNECT") + '\n'
                    + string("accept-version:1.2") + '\n'
                    + string("host:stomp.cs.bgu.ac.il") + '\n'
                    + string("login:") + userData->getUserName() + '\n'
                    + string("passcode:") + userData->getPassword() + '\n'+'\0';
    sendMessage(output);
};
void ClientInputHandler::parseJoin (vector<std::string> & inputLine){
    string topicName = inputLine[1];
    if(userData->isSubscribed(topicName)){
        std::cout << "Client is already subscribed to the topic!!" << std::endl;
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
    
    //
    sendMessage(output);
};
void ClientInputHandler::parseExit (vector<std::string> & inputLine){

    //

    std::string topicName = inputLine[1];

    // get the subscription id 
    if(userData->isSubscribedToTopic(topicName)){

    // get the receipt Id 
    std::string receiptId = std::to_string(userData->nextReciptId());
    
    std::string subscriptionId = userData->getSubscribeIdByTopic(topicName);
    std::cout<<"subscriptionId:"<<subscriptionId<<std::endl;
    //
    userData->addReceiptIdToTopic(receiptId, topicName);
    
    //
    userData->addReciptIdToCommand(receiptId, "UNSUBSCRIBE");

    //
    std::string output = string("UNSUBSCRIBE") + '\n'
                    + string("id:") + subscriptionId + '\n'
                    + string("receipt:") + receiptId + '\n' + '\0';
    
    //
    sendMessage(output);
    }else{
        std::cout<<"Not subscribed to the game"<<std::endl;
        
    }
};
void ClientInputHandler::parseReport (vector<std::string> & inputLine){
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
    std::cout<<"[ClientInputHandler::parseReport] game:"<<gameName<<std::endl;
    std::cout<<"[ClientInputHandler::parseReport] username:"<<username<<std::endl;
    
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
         output="SEND\ndestination:/"+gameName+
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
                output+="description:\n"+event.get_discription();
                output+='\n';
                output+='\0';
                std::cout<<output<<std::endl;
                sendMessage(output);
    }    

};
void ClientInputHandler::parseSummary (vector<std::string> & inputLine){
        // todo
    std::string gameName = inputLine[1];
    std::string user = inputLine[2];
    std::string file = inputLine[3];
    std::cout<<"[ClientInputHandler::parseSummary]"<<gameName<<std::endl;
    std::cout<<"[ClientInputHandler::parseSummary]"<<user<<std::endl;
    std::cout<<"[ClientInputHandler::parseSummary]"<<file<<std::endl;
    
    if(!userData->userHasReportedEvents(user)){
        std::cout<<"User has not reported any game events!!"<<std::endl;
        return;
    }
    if(!userData->getGameSummary(user)->hasSummary(gameName)){
        std::cout<<"Not subscribed to game!!"<<std::endl;
        return;
    }

    //
    std::string output;
    std::cout<<"------------[Report]--------------------"<<std::endl;
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
    std::cout<<"---------------[result]-------------------"<<std::endl;
    std::cout<<output<<std::endl;
    std::cout<<"---------------[ file]"<<file<<std::endl;

    ofstream myfile(file);
    myfile << output;
    myfile.close();

};

void ClientInputHandler::parseLogout(){
    // get the receipt Id 
    std::string receiptId = std::to_string(userData->nextReciptId());
    
    // 
    userData->setReceiptIdToDisconnect(receiptId);
    
    //
    std::string output = string("DISCONNECT") + '\n'
                    + string("receipt:") + receiptId + '\n' + '\0';
    
    //
    sendMessage(output);
};

bool ClientInputHandler::connectToServer (vector<std::string> &inputLine){
    string hostPortStr = inputLine[1];

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