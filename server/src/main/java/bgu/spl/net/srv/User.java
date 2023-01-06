package bgu.spl.net.srv;

import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

public class User {


    //
    private  String userName;
    private  String userPassword;

    //
    private  int connectionId;
    private ServerData serverData;

    private ConcurrentHashMap<Integer,String> subscriptionsIdToTopic;
    private ConcurrentHashMap<String,Integer> topicToSubscriptionID;

    public User(String userName,String userPassword){
        this.userName = userName;
        this.userPassword = userPassword;
        connectionId = -1;
        subscriptionsIdToTopic = new ConcurrentHashMap<>();
        topicToSubscriptionID = new ConcurrentHashMap<>();
        serverData = ServerData.getInstance();
    }
    public void disconnect(){
        connectionId = -1;
        subscriptionsIdToTopic.clear();
        topicToSubscriptionID.clear();
    }

    public  void unsubscribe(String topicName,Integer subscriptionID){
        String topicName=subscriptionsIdToTopic.get(subscriptionID);
        subscriptionsIdToTopic.remove(subscriptionID);
        topicToSubscriptionID.remove(topicName);
    }

    public  void subscribe(String topicName,Integer subscriptionID){
        subscriptionsIdToTopic.putIfAbsent(subscriptionID,topicName);
        topicToSubscriptionID.putIfAbsent(topicName,subscriptionID);
    }

    public String getUserName() {
        return userName;
    }


    public  void setConnectionId(Integer connectionId){
        this.connectionId=connectionId;
    }

    public String getUserPassword() {
        return userPassword;
    }

    public int getConnectionId() {
        return connectionId;
    }
    public ConcurrentHashMap.KeySetView<String,Integer> getAllTopicNames(){
        return  topicToSubscriptionID.keySet();
    }
}
