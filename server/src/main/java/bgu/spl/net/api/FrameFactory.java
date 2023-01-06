package bgu.spl.net.api;

import bgu.spl.net.api.clientFrames.*;
import bgu.spl.net.api.serverFrames.ErrorServerFrame;

public class FrameFactory {

    Frame getFrame(String message){
        String  command=message.substring(0,message.indexOf("\n"));
        switch (command){
            case "CONNECT":
               return  new ConnectClientFrame(message);
            case "SEND":
                return new SendClientFrame(message);
            case "SUBSCRIBE":
                return new SubscribeClientFrame(message);
            case "UNSUBSCRIBE":
               return  new UnsubscribeClientFrame(message);
            case "DISCONNECT":
                return new DisconnectClientFrame(message);
            default:
                return new ErrorServerFrame(message);
        }
    }


}
