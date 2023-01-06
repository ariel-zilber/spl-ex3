package bgu.spl.net.api.serverFrames;

import bgu.spl.net.api.Frame;
import bgu.spl.net.api.StompMessagingProtocolImp;
import bgu.spl.net.srv.Connections;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class DisconnectedServerFrame extends Frame {
    public DisconnectedServerFrame(String msg) {
        super(msg);
    }
    public DisconnectedServerFrame(Map<String, String> headers, List<String> body) {
        super("CONNECTED",headers,body);
    }


    public static DisconnectedServerFrame createFrame(){
        Map<String,String> header=new HashMap<>();
        header.put("version","1.2");
     return new DisconnectedServerFrame(header,null);
    }

    /**
     * @param connectionId
     * @param connections
     */
    @Override
    public void process(Integer connectionId, Connections<String> connections, StompMessagingProtocolImp protocol) {
        // todo
        System.out.println("DisconnectedServerFrame connectionId:"+connectionId);

    }

}
