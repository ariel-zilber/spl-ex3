package bgu.spl.net.api.clientFrames;

import bgu.spl.net.api.Frame;
import bgu.spl.net.api.StompMessagingProtocolImp;
import bgu.spl.net.api.serverFrames.ErrorServerFrame;
import bgu.spl.net.api.serverFrames.ReceiptServerFrame;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ServerData;

import java.util.Collections;
import java.util.Map;

/***
 *  Reprensets a client subscription frame
 */
public class SubscribeClientFrame extends Frame {
    public SubscribeClientFrame(String msg) {
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
        String topicName = headers.get("destination");
        System.out.println("[SubscribeClientFrame] 0");

        // check whenever there are format errors
        if (headerId == null) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("Id field must not be null")).process(connectionId, connections, protocol);
            return;
        }
        System.out.println("[SubscribeClientFrame] 1");
        // check whenever there are format errors
        if (topicName == null) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("destination field must not be null")).process(connectionId, connections, protocol);
            return;
        }
        System.out.println("[SubscribeClientFrame] 2");

        // top
        if (topicName.length() == 0) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("Topic name cannot be null")).process(connectionId, connections, protocol);
            return;
        }


        if (ServerData.getInstance().getTopics().topicExists(topicName)) {
            if (!ServerData.getInstance().isSubscriptionIdAvailable(topicName, Integer.parseInt(headerId))) {
                ErrorServerFrame.createFrame(this, Collections.singletonList("Subscription id is not free")).process(connectionId, connections, protocol);
                return;
            }
            if (ServerData.getInstance().isUserRegistered(topicName, connectionId)) {
                ErrorServerFrame.createFrame(this, Collections.singletonList("Client is already registered to this topic")).process(connectionId, connections, protocol);
                return;
            }


        }

        // add the topic if not exists
        boolean success = ServerData.getInstance().subscribeUser(topicName, connectionId, Integer.parseInt(headerId));
        if (success) {
            if (headers.containsKey("receipt")) {
                ReceiptServerFrame.createFrame(headers.get("receipt")).process(connectionId, connections, protocol);
            }
        } else {
            ErrorServerFrame.createFrame(this, Collections.singletonList("User is not connected")).process(connectionId, connections, protocol);

        }
    }

}
