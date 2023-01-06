#include <stdlib.h>
#include "../include/ConnectionHandler.h"




void loginCommand(int argc, char *argv[]){
    if(argc!=4){
        return;
    }
    std::string hostPort=argv[1];
    std::string username=argv[2];
    std::string password=argv[3];
    int idx = hostPort.find(':');
    std::string host=hostPort.substr(0, idx);;
    std::string port=hostPort.substr( idx,hostPort.length());

}
void joinCommand(int argc, char *argv[]){
    if(argc!=2){
        return;
    }
    std::string gameName=argv[1];
    
}
void exitCommand(int argc, char *argv[]){
    if(argc!=2){
        return;
    }
    std::string gameName=argv[1];

}
void reportCommand(int argc, char *argv[]){
    if(argc!=2){
        return;
    }
    std::string file=argv[1];

}
void summaryCommand(int argc, char *argv[]){
    if(argc!=4){
        return;
    }
 std::string gameName=argv[1];
    std::string username=argv[2];
    std::string password=argv[3];
}
void logoutCommand(int argc, char *argv[]){
    if(argc!=1){
        return;
    }
    
}


void handleCommand(int argc, char *argv[]){
    // login {host:port} {username} {password}
    if (strcmp(argv[0],"login")){
        loginCommand(argc,argv);
    }
    // join {game_name}
    if (strcmp(argv[0],"join")){
        joinCommand(argc,argv);

    }
    // exit {game_name}
    if (strcmp(argv[0],"exit")){
        exitCommand(argc,argv);
    }
    // report {file}
    if (strcmp(argv[0],"report")){
        reportCommand(argc,argv);
    }
    // summary {game_name} {user} {file}
    if (strcmp(argv[0],"summary")){
        summaryCommand(argc,argv);
    }
    // logout
    if (strcmp(argv[0],"logout")){
        logoutCommand(argc,argv);
    }
}


int main(int argc, char *argv[]) {

    std::cout<<"argc:"<<argc<<std::endl;

   if(argc!=5){
        return 0;
    }

    // 
    std::string hostPort=argv[2];
    int idx = hostPort.find(':');
    std::string host=hostPort.substr(0, idx);
    std::string portStr=hostPort.substr( idx+1,hostPort.length());
    short port = stoi(portStr);
    
    //username
    std::string username=argv[2];
    
    // password
    std::string password=argv[3];
    ConnectionHandler connectionHandler(host, port);

    
    while (1) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
		std::string line(buf);
        std::cout<<"My command is"<<line<<std::endl;





        // if (!connectionHandler.sendLine(line)) {
        //     std::cout << "Disconnected. Exiting...\n" << std::endl;
        //     break;
        // }
		// // connectionHandler.sendLine(line) appends '\n' to the message. Therefor we send len+1 bytes.
        // std::cout << "Sent " << len+1 << " bytes to server" << std::endl;

 
        // // We can use one of three options to read data from the server:
        // // 1. Read a fixed number of characters
        // // 2. Read a line (up to the newline character using the getline() buffered reader
        // // 3. Read up to the null character
        // std::string answer;
        // // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
        // // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
        // if (!connectionHandler.getLine(answer)) {
        //     std::cout << "Disconnected. Exiting...\n" << std::endl;
        //     break;
        // }
        
		// len=answer.length();
		// // A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
		// // we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
        // answer.resize(len-1);
        // std::cout << "Reply: " << answer << " " << len << " bytes " << std::endl << std::endl;
        // if (answer == "bye") {
        //     std::cout << "Exiting...\n" << std::endl;
        //     break;
        // }
    }
    return 0;




	// TODO: implement the STOMP client
	return 0;
}