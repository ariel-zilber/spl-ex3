package bgu.spl.net.api.clientFrames;

import bgu.spl.net.api.Frame;
import bgu.spl.net.api.StompMessagingProtocolImp;
import bgu.spl.net.api.serverFrames.ErrorServerFrame;
import bgu.spl.net.api.serverFrames.MessageServerFrame;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ServerData;
import bgu.spl.net.srv.User;
import jdk.internal.net.http.common.Pair;

import java.util.Collections;
import java.util.Map;

public class SendClientFrame extends Frame {
    public SendClientFrame(String msg) {
        super(msg);
    }

    /**
     * @param connectionId
     * @param connections
     */
    @Override
    public void process(Integer connectionId, Connections<String> connections, StompMessagingProtocolImp protocol) {
        Map<String, String> headers = this.getHeaders();
        String headerDestination = headers.get("destination");
        String topicName;
        if (headerDestination == null) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("Id field must not be null")).process(connectionId, connections, protocol);
            return;
        }
        if (!headerDestination.startsWith("/topic/")) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("Id field must not be null")).process(connectionId, connections, protocol);
            return;
        }
        topicName = headerDestination.split("/topic/")[1];

        // top
        if (topicName.length() == 0) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("Topic name cannot be null")).process(connectionId, connections, protocol);
            return;
        }

        //
        User currUser = ServerData.getInstance().getUsers().getUserById(connectionId);
        Integer subscriptionId = currUser.getSubscriptionsId(topicName);
        MessageServerFrame messageFrame = MessageServerFrame.createFrame(
                "" + subscriptionId,
                "" + ServerData.getInstance().nextMessageId(),
                topicName,
                this.getBody()
        );
        //
        messageFrame.process(connectionId, connections, protocol);

    }

}
