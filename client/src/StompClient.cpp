#include <stdlib.h>
#include "../include/ConnectionHandler.h"

// login {host:port} {username} {password}
// join {game_name}
// exit {game_name}
// report {file}
// summary {game_name} {user} {file}
// logout

int main(int argc, char *argv[]) {


    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }




	// TODO: implement the STOMP client
	return 0;
}