package bgu.spl.net.srv;


import java.util.Objects;
import java.util.concurrent.atomic.AtomicInteger;

// Helper class

/***
 * Singleton represents the server data
 */
public class ServerData {
    private static ServerData instance = null;
    private final Users users;
    private final Topics topics;

    private final AtomicInteger numberOfMessages;
    private final ConnectionsImpl connections;

    /***
     *
     */
    private ServerData() {
        numberOfMessages = new AtomicInteger(0);
        connections = new ConnectionsImpl();
        topics = new Topics();
        users = new Users();
    }


    /***
     *
     * @return
     */
    public static ServerData getInstance() {
        if (instance == null) {
            instance = new ServerData();
        }
        return instance;
    }

    //

    /***
     *
     * @return
     */
    public Users getUsers() {
        return users;
    }

    /***
     *
     * @return
     */
    public Topics getTopics() {
        return topics;
    }

    /***
     *
     * @return
     */
    public ConnectionsImpl getConnections() {
        return connections;
    }

    /***
     *
     * @return
     */
    public Integer nextMessageId() {
        return numberOfMessages.incrementAndGet();
    }

    /***
     *
     * @param connectionId
     * @return
     */
    public boolean canDisconnectUser(Integer connectionId) {
        return ( users.getUserById(connectionId)!=null) ;
    }


    /***
     *
     * @param connectionId
     */
    public void disconnectUser(Integer connectionId) {
        User user = users.getUserById(connectionId);
        if (user == null) {
            return  ;
        }
        // remove the user from all topics
        for (String topicName : user.getAllTopicNames()) {
            topics.getTopic(topicName).removeUser(user);
        }
        //  logout the user
        users.logoutUser(connectionId);
    }

    /***
     *
     * @param connectionId
     * @param subscriptionID
     * @return
     */
    public boolean unsubscribeUser(Integer connectionId, Integer subscriptionID) {
        User user = getUsers().getUserById(connectionId);
        if (user == null) {
            return false;
        }
        String topicName = user.getTopicName(subscriptionID);
        if (topicName == null) {
            return false;
        }
        getTopics().getTopic(topicName).removeUser(user);
        user.unsubscribe(subscriptionID);
        return true;
    }


    /**
     *
     * @param topicName
     * @param connectionId
     * @param subscriptionID
     * @return
     */
    public boolean subscribeUser(String topicName, Integer connectionId, Integer subscriptionID) {
        // get the user associated with the connection
        User user = getUsers().getUserById(connectionId);

        // case atempting to subscribe a non connected user
        if (user == null) {
            return false;
        }
        // create topic if not exists
        topics.addTopic(topicName);
        user.subscribe(topicName, subscriptionID);
        topics.getTopic(topicName).addUser(user);
        return true;
    }

    /**
     *
     * @param username
     * @param password
     * @param connectionId
     * @return
     */
    public LoginCodes loginUser(String username, String password, Integer connectionId) {
        Users users = ServerData.getInstance().getUsers();
        users.createUser(username, password);
        return users.loginUser(username, password, connectionId);
    }

    /***
     *
     * @param topicName
     * @param subscriptionId
     * @return
     */
    public boolean isSubscriptionIdAvailable(String topicName, Integer subscriptionId) {
        Topic topic = getTopics().getTopic(topicName);
        for (User user : topic.getSubscribedUsers()) {
            if (Objects.equals(user.getSubscriptionsId(topicName), subscriptionId)) {
                return false;
            }
        }
        return true;
    }

    /***
     *  Checks wheneve a given user is registerd
     * @param topicName
     * @param connectionId
     * @return
     */
    public boolean isUserRegistered(String topicName, Integer connectionId) {
        Topic topic = getTopics().getTopic(topicName);
        for (User user : topic.getSubscribedUsers()) {
            if (Objects.equals(user.getConnectionId(), connectionId)) {
                return true;
            }
        }
        return false;
    }
}
