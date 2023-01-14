package bgu.spl.net.api.clientFrames;

import bgu.spl.net.api.Frame;
import bgu.spl.net.api.StompMessagingProtocolImp;
import bgu.spl.net.api.serverFrames.ErrorServerFrame;
import bgu.spl.net.api.serverFrames.MessageServerFrame;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ServerData;
import bgu.spl.net.srv.User;

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
        String topicName = headers.get("destination");
        if (topicName == null) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("Did not contain a destination header ,\n" +
                    "which is REQUIRED for message propagation.")).process(connectionId, connections, protocol);
            return;
        }
        if (topicName.length() == 0) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("Destination cannot be empty")).process(connectionId, connections, protocol);
            return;
        }

        if(! ServerData.getInstance().getTopics().topicExists(topicName)){
            ErrorServerFrame.createFrame(this, Collections.singletonList("Client cannot send message if not subscribed to destination")).process(connectionId, connections, protocol);
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
