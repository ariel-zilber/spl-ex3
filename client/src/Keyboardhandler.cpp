#include "../include/KeyboardHandler.h"
/* Tokenizing a string */
std::vector<std::string>  KeyboardHandler::tokenizer( const std::string& p_pcstStr, char delim )  {
    std::vector<std::string> tokens;
    std::stringstream   mySstream( p_pcstStr );
    std::string         temp;

    while( getline( mySstream, temp, delim ) ) {
        tokens.push_back( temp );
    }

    return tokens;
} 

KeyboardHandler::KeyboardHandler():handler(){
}

void KeyboardHandler::run(){
    while (1){
        
        // read line for user
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
		std::string line(buf);
        std::vector<std::string> lines=tokenizer(line,' ');
        std::string command=lines[0];
        if(handler.isLoggedIn()){
        // login {host:port} {username} {password}
        if (command.compare("login")){
            handler.processCommandLogin(lines);
        }
        // join {game_name}
        if (command.compare("join")){
            processCommandJoin(lines);

        }
        // exit {game_name}
        if (command.compare("exit")){
            // exitCommand(lines);
        }
        // report {file}
        if (command.compare("report")){
            // reportCommand(lines);
        }
        // summary {game_name} {user} {file}
        if (command.compare("summary")){
            // summaryCommand(lines);
        }
        // logout
        if (command.compare("logout")){
            // logoutCommand(lines);
        }

                
        }else{

        }

    }

}



// // todo
// void run(){
//     bool userLoggedIn=false;
//     ConnectionHandler *handler;
//     while (1) {
//         // read input from user

// 		int len=line.length();
//         std::cout<<line<<std::endl;
//         std::vector<std::string> tokens=tokenizer(line,' ');
//         std::cout<<"Command:"<<tokens[0]<<std::endl;

//         if(!userLoggedIn){
//             if(tokens[0].compare("login")){
//                 //
//                 handler= proccessLoginCommand(std::vector<std::string> tokens);
//             }
//             // 
//         }else{

//         }

//     }
// }
