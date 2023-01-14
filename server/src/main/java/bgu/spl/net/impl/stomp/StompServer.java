package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocolImp;
import bgu.spl.net.srv.Server;

public class StompServer {
    public static void runReactor(int port){
        System.out.println("Reactor mode!!!!");
        Server.reactor(
                Runtime.getRuntime().availableProcessors(),
                port, //port
                StompMessagingProtocolImp::new, //protocol factory
                StompMessageEncoderDecoder::new //message encoder decoder factory
        ).serve();
    }
    public static void runThreadPerClient(int port){
        System.out.println("TPC mode!!!!");

        // you can use any server...
        Server.threadPerClient(
                port, //port
                StompMessagingProtocolImp::new, //protocol factory
                StompMessageEncoderDecoder::new //message encoder decoder factory
        ).serve();
//

    }

    public static void main(String[] args) {
        System.out.println("                                                                           __ __ \n" +
                " _____ _____ _____ _____ _____      _____ _____ _____ _____ _____ _____   |  |  |\n" +
                "|   __|_   _|     |     |  _  |    |   __|     |     |     |   __| __  |  |  |  |\n" +
                "|__   | | | |  |  | | | |   __|    |__   |  |  |   --|   --|   __|    -|  |__|__|\n" +
                "|_____| |_| |_____|_|_|_|__|       |_____|_____|_____|_____|_____|__|__|  |__|__|\n" +
                "                                                                                 \n" +
                "By Ariel and Barak\n");
        if(args.length==2){
            int port=Integer.parseInt(args[0]);
            String serverType=args[1];
            if (serverType.equals("tcp")){
                runThreadPerClient(port);
            }else if (serverType.equals("reactor")){
                runReactor(port);
            }
        }

    }
}
