package bgu.spl.net.api;

import bgu.spl.net.srv.Connections;

import java.util.*;

public abstract class Frame {
    private String command;
    private Map<String, String> headers;
    private List<String> body;

    public Frame(String command, Map<String, String> headers, List<String> body) {
        this.command=command;
        this.headers=headers;
        this.body=body;
    }

    public Frame(String msg) {
        String[] lines = msg.split("\\r?\\n");

        //
        command=lines[0];
        System.out.println("[Frame] command: "+command);
        //
        int startBody=lines.length;
        headers=new HashMap<>();
        body=new ArrayList<>();

        for(int i=1;i<lines.length;i++){
            String[] keyValPair=lines[i].split(":");
            if(keyValPair.length!=2){
                startBody=i;
                break;
            }
            headers.put(keyValPair[0],keyValPair[1]);
        }
        body.addAll(Arrays.asList(lines).subList(startBody , lines.length ));
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

    @Override
    public String toString() {
        System.out.println("[Frame] body length:"+body.size());
        StringBuilder stringBuilder=new StringBuilder();
        stringBuilder.append(command);
        stringBuilder.append('\n');
        stringBuilder.append('\n');

        for(String key:headers.keySet()){
            stringBuilder.append(key+":"+headers.get(key)+'\n');
        }

        for(String row:body){
            stringBuilder.append('\n');
            stringBuilder.append(row);
        }
        return stringBuilder.toString();
    }
}
