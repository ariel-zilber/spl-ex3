package bgu.spl.net.api;

import bgu.spl.net.api.clientFrames.*;
import bgu.spl.net.srv.Connections;


public class StompMessagingProtocolImp  implements  StompMessagingProtocol<String>{
    private int connectionId;
    private Connections<String> connections;
    private boolean shouldTerminate;

    @Override
    public void start(int connectionId, Connections<String> connections) {
        this.connectionId = connectionId;
        this.connections = connections;
        shouldTerminate = false;
    }

    @Override
    public void process(String message) {

        //
        if(Frame.validFrame(message)){
            Frame frame=new FrameFactory().getFrame(message);
            frame.process(connectionId,connections,this);
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