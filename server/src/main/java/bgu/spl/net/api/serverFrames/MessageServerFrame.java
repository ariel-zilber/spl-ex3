package bgu.spl.net.api.serverFrames;

import bgu.spl.net.api.Frame;
import bgu.spl.net.api.StompMessagingProtocolImp;
import bgu.spl.net.srv.Connections;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MessageServerFrame extends Frame {

    //
    private String topicName;

    //
    public MessageServerFrame(Map<String, String> headers, List<String> body, String topicName) {
        super("CONNECTED", headers, body);
        topicName=topicName;
    }



    public static MessageServerFrame createFrame(String subscription, String messageId, String topicName, List<String> messageBody) {
        Map<String, String> header = new HashMap<>();
        header.put("subscription", subscription);
        header.put("message-id", messageId);
        header.put("destination", "/topic/" + topicName);
          return new MessageServerFrame(header, messageBody, topicName);
    }

    /**
     * @param connectionId
     * @param connections
     */
    @Override
    public void process(Integer connectionId, Connections<String> connections, StompMessagingProtocolImp protocol) {
        connections.send(topicName, toString());
    }

}
