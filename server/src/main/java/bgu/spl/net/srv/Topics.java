package bgu.spl.net.srv;


import java.util.concurrent.ConcurrentHashMap;

/***
 *
 */
public class Topics {

    private ConcurrentHashMap<String,Topic> nameToTopic;

    /**
     *
     */
    public Topics() {
        this.nameToTopic=new ConcurrentHashMap<>();
    }

    /***
     *
     * @param topicName
     */
    public  void addTopic(String topicName){
        if(nameToTopic.get(topicName)==null){
            nameToTopic.put(topicName,new Topic());
        }
    }

    /**
     * Getter
     * @param topicName
     * @return
     */
    public Topic getTopic(String topicName){
        return nameToTopic.get(topicName);
    }

    /**
     * Check whenever topic exists!!
     * @param topicName
     * @return
     */
    public boolean topicExists(String topicName){
        return nameToTopic.get(topicName)!=null;
    }
}
