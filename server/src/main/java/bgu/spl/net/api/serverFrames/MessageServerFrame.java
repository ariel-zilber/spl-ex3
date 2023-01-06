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
    private String msg;

    //
    public MessageServerFrame(String msg, String topicName) {
        super(msg);
    }

    //
    public MessageServerFrame(Map<String, String> headers, List<String> body, String topicName, String msg) {
        super("CONNECTED", headers, body);
    }


    public static MessageServerFrame createFrame(String subscription, String messageId, String topic, List<String> messageBody) {
        Map<String, String> header = new HashMap<>();
        header.put("subscription", subscription);
        header.put("message-id", messageId);
        header.put("destination", "/topic/" + topic);
        return null;
        //  return new MessageServerFrame(header, messageBody, topicName, messageBody);
    }

    /**
     * @param connectionId
     * @param connections
     */
    @Override
    public void process(Integer connectionId, Connections<String> connections, StompMessagingProtocolImp protocol) {
       // connections.send(topicName, msg);
        System.out.println("ReceiptServerFrame connectionId:"+connectionId);

    }

}
