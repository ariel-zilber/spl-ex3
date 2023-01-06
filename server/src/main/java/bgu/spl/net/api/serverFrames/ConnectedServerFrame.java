package bgu.spl.net.api.serverFrames;

import bgu.spl.net.api.Frame;
import bgu.spl.net.api.StompMessagingProtocolImp;
import bgu.spl.net.srv.Connections;

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
     return new ConnectedServerFrame(header,null);
    }

    /**
     * @param connectionId
     * @param connections
     */
    @Override
    public void process(Integer connectionId, Connections<String> connections, StompMessagingProtocolImp protocol) {
        // todo
        System.out.println("ConnectedServerFrame connectionId:"+connectionId);

    }

}
