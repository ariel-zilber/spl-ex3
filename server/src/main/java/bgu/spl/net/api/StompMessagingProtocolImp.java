package bgu.spl.net.api;

import bgu.spl.net.api.clientFrames.ClientFrameFactory;
import bgu.spl.net.srv.Connections;


public class StompMessagingProtocolImp  implements  StompMessagingProtocol<String>{
    private int connectionId;
    private Connections<String> connections;
    private boolean shouldTerminate;

    @Override
    public void start(int connectionId, Connections<String> connections) {
        this.connectionId = connectionId;
        this.connections = connections;
        this.shouldTerminate = false;
    }

    @Override
    public void process(String message) {
        System.out.println("[StompMessagingProtocolImp] [message] ");
        System.out.println(message);
        System.out.println("---------------------");

        if(message.charAt(0)=='\n'){
            message=message.substring(1);
        }
        if(Frame.validFrame(message)){
            System.out.println("Frame is valid!!!");

            Frame frame=new ClientFrameFactory().getFrame(message);
            frame.process(connectionId,connections,this);
        }else{
            System.out.println("Bad formatted message was given!!");
        }
    }


    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }


    public void terminate(){
        shouldTerminate = true;
    }

}