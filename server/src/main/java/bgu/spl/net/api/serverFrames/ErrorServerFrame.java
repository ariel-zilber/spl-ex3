package bgu.spl.net.api.serverFrames;

import bgu.spl.net.api.Frame;
import bgu.spl.net.api.StompMessagingProtocolImp;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ServerData;
import bgu.spl.net.srv.User;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ErrorServerFrame extends Frame {

    public ErrorServerFrame(Map<String, String> headers,List<String> body) {
        super("ERROR",headers,body);
    }
    public ErrorServerFrame(String msg) {
        super(msg);
    }

    /**
     * @param connectionId
     * @param connections
     */
    @Override
    public void process(Integer connectionId, Connections<String> connections, StompMessagingProtocolImp protocol) {
        // todo
        System.out.println("ErrorServerFrame connectionId:"+connectionId);
        connections.send(connectionId,toString());

        ServerData.getInstance().getUsers().logoutUser(connectionId);
        protocol.terminate();
    }


    public static    ErrorServerFrame createFrame(Frame other,List<String> customMessage){
        Map<String,String> errorHeaders=new HashMap<>();
        List<String> errorBody=new ArrayList<>();
        if(other.getHeaders().get("receipt-id")!=null){
            errorHeaders.put("receipt-id",other.getHeaders().get("receipt-id"));
        }
        errorHeaders.put("message","malformedFrameReceived");
        errorBody.add("The message :");
        errorBody.add("-----");
        errorBody.add(other.toString());
        errorBody.add("-----");
        errorBody.addAll(customMessage);
        return new ErrorServerFrame(errorHeaders,errorBody);
    }

}
