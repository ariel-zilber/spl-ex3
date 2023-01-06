#include "StompServerHandler.h"



StompServerHandler::StompServerHandler():connectionHandler(nullptr){
}

bool StompServerHandler::initConnection(std::string host,int port){
  connectionHandler=new ConnectionHandler(host,port);
  return connectionHandler->connect();
}

void StompServerHandler::stopConnection(){
    if(connectionHandler!=nullptr){
  delete connectionHandler;
  connectionHandler=nullptr;
    }
}


bool StompServerHandler::processCommandLogin(std::vector<std::string>  lines ){
  //
  std::string username=lines[0];
  std::string password=lines[1];

    
    std::string message="CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\nlogin:"+username+"\npasscode:"+password+"\n";
    std::string answer;
    connectionHandler->sendFrameAscii(message,'\0');
    if( !connectionHandler->getFrameAscii(answer,'\0')){return false;};

    // validate response

    // update userdata credentails


};
    