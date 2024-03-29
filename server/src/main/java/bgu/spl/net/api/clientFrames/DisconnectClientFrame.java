package bgu.spl.net.api.clientFrames;

import bgu.spl.net.api.Frame;
import bgu.spl.net.api.StompMessagingProtocolImp;
import bgu.spl.net.api.serverFrames.ErrorServerFrame;
import bgu.spl.net.api.serverFrames.ReceiptServerFrame;
import bgu.spl.net.srv.*;

import java.util.Collections;
import java.util.Map;

public class DisconnectClientFrame extends Frame {
    public DisconnectClientFrame(String msg) {
        super(msg);
    }

    /**
     * @param connectionId
     * @param connections
     */
    @Override
    public void process(Integer connectionId, Connections<String> connections, StompMessagingProtocolImp protocol) {
        Map<String, String> headers = this.getHeaders();
        if(ServerData.getInstance().canDisconnectUser(connectionId)){
            ReceiptServerFrame.createFrame(headers.get("receipt")).process(connectionId, connections, protocol);
            ServerData.getInstance().disconnectUser(connectionId);
            protocol.terminate();

        } else {
            ErrorServerFrame.createFrame(this, Collections.singletonList("Cannot disconnecte unless connected already!")).process(connectionId, connections, protocol);
        }
    }
}
