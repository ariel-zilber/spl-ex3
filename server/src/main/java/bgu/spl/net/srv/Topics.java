package bgu.spl.net.srv;

import jdk.internal.net.http.common.Pair;

import java.util.*;

public class Topics {
    // todo: change to set

    // topic->[subscriptionId,connectionId]
    private final Map<String, List<Pair<Integer,Integer>>> topicsToIds;
    private final Map<Integer, List<String>> subscriptionIdToTopic;

    private final Map< Integer, List<Pair<Integer,String>>> connectionIdToSubscriptionId;


    //
    private Map<String,Topic> nameToTopic;



    public Topics() {
        this.topicsToIds = new HashMap<>();
        this.subscriptionIdToTopic = new HashMap<>();
        this.connectionIdToSubscriptionId=new HashMap<>();
    }

    public  void addTopic(String topicName){
        if(nameToTopic.get(topicName)==null){
            nameToTopic.put(topicName,new Topic(topicName));
        }
    }
    public Topic getTopic(String topicName){
        return nameToTopic.get(topicName);
    }
    public List<Pair<Integer,Integer>> getTopicIds(String topic) {
        return this.topicsToIds.get(topic);
    }

    //
    public  boolean isSubscriptionIdAvailable(String topicName,Integer subscriptionId){
        for(Pair<Integer,Integer> pair:topicsToIds.get(topicName)){
            if(Objects.equals(subscriptionId, pair.first)){
                return true;
            }
        }
        return  false;
    }

    public void subscribe(Integer connectionId,Integer subscriptionId, String topic) {
        // todo handle case entrying duplicate data!!!

        //
        topicsToIds.computeIfAbsent(topic, k -> new ArrayList<>());
        topicsToIds.get(topic).add(new Pair<Integer,Integer>(subscriptionId,connectionId));

        //
        subscriptionIdToTopic.computeIfAbsent(subscriptionId, k -> new ArrayList<>());
        subscriptionIdToTopic.get(subscriptionId).add(topic);

        // link connectionid to subscriptionID+topic
        connectionIdToSubscriptionId.computeIfAbsent(connectionId, k -> new ArrayList<>());
        connectionIdToSubscriptionId.get(connectionId).add(new Pair<>(subscriptionId,topic));


    }

    public void disconnectUser(Integer subscriptionId) {
        for (String topic:subscriptionIdToTopic.get(subscriptionId)) {
            unsubscribe(subscriptionId,topic);
        }
    }
    public boolean unsubscribe(Integer connectionID,Integer subscriptionId) {
        for (Pair<Integer,String> subscription:connectionIdToSubscriptionId.get(connectionID)){
            Integer currSubscriptionId=subscription.first;
            String topic=subscription.second;
            if(connectionID.equals(currSubscriptionId)){
                unsubscribe(subscriptionId,topic);
                return true;
            }
        }
        return false;
    }

    public void unsubscribe(Integer subscriptionId,String topic) {
        // todo handle case entrying duplicate data!!!
        topicsToIds.get(topic).removeIf(id-> Objects.equals(id, subscriptionId)); // removes all instances, not just the 1st one
    }

}
