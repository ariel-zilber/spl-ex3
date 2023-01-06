package bgu.spl.net.api;

import bgu.spl.net.srv.Connections;

import java.util.*;

public abstract class Frame {
    private String command;
    private Map<String, String> headers;
    private List<String> body;

    public Frame(String command, Map<String, String> headers, List<String> body) {
        command=command;
        headers=headers;
        body=body;
    }

    public Frame(String msg) {
        String[] lines = msg.split("\\r?\\n");

        //
        command=lines[0];

        //
        int startBody=lines.length-1;
        headers=new HashMap<>();
        for(int i=1;i<lines.length-1;i++){
            String[] keyValPair=lines[i].split(":");
            if(keyValPair.length!=2){
                startBody=i;
                break;
            }
            headers.put(keyValPair[0],keyValPair[1]);
        }

        body=new ArrayList<>();
        body.addAll(Arrays.asList(lines).subList(startBody + 1, lines.length - 1));

    }

    public static boolean validFrame(String msg) {
        String[] lines = msg.split("\\r?\\n");
        if (lines.length < 2) {
            return false;
        }
        if (lines[0].contains(":")) {
            return false;
        }
        int startBody=lines.length;

        for (int i = 1; i < lines.length-1; i++) {
            if (!lines[i].contains(":") && lines[i].length() > 1) {
                return false;
            } else if (!lines[i].contains(":") && lines[i].length() <= 1) {
                startBody = i;
                break;
            } else {
                String keyValPair[] = lines[i].split(":");
                if (keyValPair.length != 2) {
                    return false;
                }
                if (keyValPair[0].length() == 0) {
                    return false;
                }
                if (keyValPair[1].length() == 0) {
                    return false;
                }

            }
        };
        for(int i=startBody+1;i<lines.length-1;i++){
            if(lines[i].length()==1){
                return false;
            }
        }

        return  true;
    }

    public abstract void process( Integer connectionId, Connections<String> connections,StompMessagingProtocolImp protocol);

    public String getCommand(){
        return  command;
    }

    public Map<String,String > getHeaders(){
        return  headers;
    }

    public List<String> getBody(){
        return  body;
    }
}
