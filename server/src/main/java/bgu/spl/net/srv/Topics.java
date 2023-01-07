package bgu.spl.net.srv;


import java.util.concurrent.ConcurrentHashMap;

public class Topics {
    // todo: change to set
    private ConcurrentHashMap<String,Topic> nameToTopic;



    public Topics() {
        this.nameToTopic=new ConcurrentHashMap<>();
    }

    public  void addTopic(String topicName){
        if(nameToTopic.get(topicName)==null){
            nameToTopic.put(topicName,new Topic(topicName));
        }
    }
    public Topic getTopic(String topicName){
        return nameToTopic.get(topicName);
    }
    public boolean topicExists(String topicName){
        return nameToTopic.get(topicName)!=null;
    }
    //



}
