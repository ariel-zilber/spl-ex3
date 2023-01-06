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


        if(message.length() > 0){
            if(message.charAt(0) == '\n')
                message = message.substring(1);
            String[] msg = message.split("\\r?\\n"); // split the message by end line
            switch (msg[0]) {
                case "CONNECT":
                    ConnectFrame connect = new ConnectFrame();
                    connect.process(msg, connectionId, connections, this);
                    break;
                case "SUBSCRIBE":
                    SubscribeFrame subscribe = new SubscribeFrame();
                    subscribe.process(msg, connectionId, connections, this);
                    break;
                case "UNSUBSCRIBE":
                    UnsubscribeFrame unsubscribeFrame = new UnsubscribeFrame();
                    unsubscribeFrame.process(msg, connectionId, connections, this);
                    break;
                case "SEND":
                    SendFrame send = new SendFrame();
                    send.process(msg, connectionId, connections, this);
                    break;
                case "DISCONNECT":
                    DisconnectFrame disconnectFrame = new DisconnectFrame();
                    disconnectFrame.process(msg, connectionId, connections, this);
                    break;
                default:
                    System.out.println("Incorrect Message received");
                    break;
            }
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