package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.api.StompMessagingProtocolImp;
import bgu.spl.net.impl.echo.EchoProtocol;
import bgu.spl.net.impl.echo.LineMessageEncoderDecoder;
import bgu.spl.net.srv.Server;

public class StompServer {
    public static void runReactor(int port){
        Server.reactor(
                Runtime.getRuntime().availableProcessors(),
                port, //port
                StompMessagingProtocolImp::new, //protocol factory
                StompMessageEncoderDecoder::new //message encoder decoder factory
        ).serve();
    }
    public static void runThreadPerClient(int port){

        // you can use any server...
        Server.threadPerClient(
                port, //port
                StompMessagingProtocolImp::new, //protocol factory
                StompMessageEncoderDecoder::new //message encoder decoder factory
        ).serve();
//

    }

    public static void main(String[] args) {
        // TODO:: ??
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
