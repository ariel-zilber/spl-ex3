package bgu.spl.net.api.clientFrames;

import bgu.spl.net.api.Frame;
import bgu.spl.net.api.StompMessagingProtocolImp;
import bgu.spl.net.api.serverFrames.ErrorServerFrame;
import bgu.spl.net.api.serverFrames.ReceiptServerFrame;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ServerData;

import java.util.Collections;
import java.util.Map;

public class UnsubscribeClientFrame extends Frame {
    public UnsubscribeClientFrame(String msg) {
        super(msg);
    }

    /**
     * @param connectionId
     * @param connections
     */
    @Override
    public void process(Integer connectionId, Connections<String> connections, StompMessagingProtocolImp protocol) {
        Map<String, String> headers = this.getHeaders();

        String headerId = headers.get("id");
        // check whenever there are format errors
        if (headerId == null) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("Id field must not be null")).process(connectionId, connections,protocol);
            return;
        }

        // unsubscribe user
        if(!ServerData.getInstance().unsubscribeUser(connectionId,Integer.parseInt(headerId))){
            ErrorServerFrame.createFrame(this, Collections.singletonList("Id field must not be null")).process(connectionId, connections,protocol);
            return;
        }
        if (headers.containsKey("receipt")) {
            ReceiptServerFrame.createFrame(headers.get("receipt")).process(connectionId, connections,protocol);
        }
    }

}
