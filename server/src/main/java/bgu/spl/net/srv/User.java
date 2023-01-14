package bgu.spl.net.srv;

import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

/***
 * Represents a single user
 */
public class User {

    private  String userName;
    private  String userPassword;
    //
    private  int connectionId;
    private ServerData serverData;

    private ConcurrentHashMap<Integer,String> subscriptionsIdToTopic;
    private ConcurrentHashMap<String,Integer> topicToSubscriptionID;

    /***
     *
     * @param userName
     * @param userPassword
     */
    public User(String userName,String userPassword){
        this.userName = userName;
        this.userPassword = userPassword;
        connectionId = -1;
        subscriptionsIdToTopic = new ConcurrentHashMap<>();
        topicToSubscriptionID = new ConcurrentHashMap<>();
        serverData = ServerData.getInstance();
    }

    /***
     *
     */
    public void disconnect(){
        connectionId = -1;
        subscriptionsIdToTopic.clear();
        topicToSubscriptionID.clear();
    }

    /***
     *
     * @param subscriptionID
     * @return
     */
    String getTopicName(Integer subscriptionID){
        return subscriptionsIdToTopic.get(subscriptionID);
    }

    /***
     *
     * @param subscriptionID
     */
    public  void unsubscribe( Integer subscriptionID){
        String topicName=subscriptionsIdToTopic.get(subscriptionID);
        subscriptionsIdToTopic.remove(subscriptionID);
        topicToSubscriptionID.remove(topicName);
    }

    /***
     *
     * @param topicName
     * @param subscriptionID
     */
    public  void subscribe(String topicName,Integer subscriptionID){
        subscriptionsIdToTopic.putIfAbsent(subscriptionID,topicName);
        topicToSubscriptionID.putIfAbsent(topicName,subscriptionID);
    }

    /***
     *
     * @param topicName
     * @return
     */
    public Integer getSubscriptionsId(String topicName) {
        return topicToSubscriptionID.get(topicName);
    }


    /**
     * Setter
     * @param connectionId
     */
    public  void setConnectionId(Integer connectionId){
        this.connectionId=connectionId;
    }

    /**
     *  Getter for password
     * @return
     */
    public String getUserPassword() {
        return userPassword;
    }

    /***
     * Getter for connectionID
     * @return
     */
    public int getConnectionId() {
        return connectionId;
    }

    /***
     *
     * @return all the topics the user is subscribed to
     */
    public ConcurrentHashMap.KeySetView<String,Integer> getAllTopicNames(){
        return  topicToSubscriptionID.keySet();
    }
}
