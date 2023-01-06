package bgu.spl.net.api.serverFrames;

import bgu.spl.net.api.Frame;
import bgu.spl.net.api.StompMessagingProtocolImp;
import bgu.spl.net.srv.Connections;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ReceiptServerFrame  extends Frame {
    public ReceiptServerFrame( Map<String, String> headers, List<String> body) {
        super("RECEIPT", headers, body);
    }

    public ReceiptServerFrame(String msg) {
        super(msg);
    }

    /**
     * @param connectionId
     * @param connections
     */
    @Override
    public void process(Integer connectionId, Connections<String> connections, StompMessagingProtocolImp protocol) {
        // todo
        System.out.println("ReceiptServerFrame connectionId:"+connectionId);
    }


    public static Frame createFrame(String receiptId){
        Map<String,String> header=new HashMap<>();
        header.put("receipt-Id",receiptId);
        return new ReceiptServerFrame(header,null);
    }
}