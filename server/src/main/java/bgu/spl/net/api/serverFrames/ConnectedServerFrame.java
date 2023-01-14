package bgu.spl.net.api.serverFrames;

import bgu.spl.net.api.Frame;
import bgu.spl.net.api.StompMessagingProtocolImp;
import bgu.spl.net.srv.Connections;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ConnectedServerFrame extends Frame {
    public ConnectedServerFrame(String msg) {
        super(msg);
    }
    public ConnectedServerFrame(Map<String, String> headers, List<String> body) {
        super("CONNECTED",headers,body);
    }


    public static ConnectedServerFrame createFrame(){
        Map<String,String> header=new HashMap<>();
        header.put("version","1.2");
     return new ConnectedServerFrame(header,new ArrayList<>());
    }

    /**
     * @param connectionId
     * @param connections
     */
    @Override
    public void process(Integer connectionId, Connections<String> connections, StompMessagingProtocolImp protocol) {
        connections.send(connectionId,toString());
    }

}
