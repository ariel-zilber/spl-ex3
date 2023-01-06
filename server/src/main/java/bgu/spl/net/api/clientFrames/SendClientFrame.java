package bgu.spl.net.api.clientFrames;

import bgu.spl.net.api.Frame;
import bgu.spl.net.api.StompMessagingProtocolImp;
import bgu.spl.net.api.serverFrames.ErrorServerFrame;
import bgu.spl.net.api.serverFrames.MessageServerFrame;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ServerData;
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
            ErrorServerFrame.createFrame(this, Collections.singletonList("Id field must not be null")).process(connectionId, connections,protocol);
            return;
        }
        if (!headerDestination.startsWith("/topic/")) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("Id field must not be null")).process(connectionId, connections,protocol);
            return;
        }
        topicName = headerDestination.split("/topic/")[1];

        // top
        if(topicName.length()==0){
            ErrorServerFrame.createFrame(this, Collections.singletonList("Topicname cannot be null")).process(connectionId, connections,protocol);
            return;
        }

        //
        for(Pair<Integer,Integer> currPair:ServerData.getInstance().getTopics().getTopicIds(topicName)){
            MessageServerFrame.createFrame(currPair.first.toString(),ServerData.getInstance().nextMessageId().toString(),topicName,getBody()).process(connectionId,connections,protocol);
        }

    }

}
