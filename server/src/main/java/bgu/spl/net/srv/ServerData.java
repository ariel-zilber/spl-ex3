package bgu.spl.net.srv;


import java.util.Objects;
import java.util.concurrent.atomic.AtomicInteger;

// Helper class
public class ServerData {
    private static ServerData instance = null;
    private Users users;

    private Topics topics;

    private AtomicInteger numberOfMessages;
    private ConnectionsImpl<String> connections;

    private ServerData() {
        numberOfMessages = new AtomicInteger(0);
        connections = new ConnectionsImpl<String>();
        topics = new Topics();
        users = new Users();
    }


    public static ServerData getInstance() {
        if (instance == null) {
            instance = new ServerData();
        }
        return instance;
    }

    //
    public Users getUsers() {
        return users;
    }

    public Topics getTopics() {
        return topics;
    }

    public ConnectionsImpl<String> getConnections() {
        return connections;
    }

    public Integer nextMessageId() {
        return numberOfMessages.incrementAndGet();
    }

    public boolean disconnectUser(Integer connectionId) {
        User user = users.getUserById(connectionId);
        if (user == null) {
            return false;
        }
        // remove the user from all topics
        for (String topicName : user.getAllTopicNames()) {
            topics.getTopic(topicName).removeUser(user);
        }

        //  logout the user
        users.logoutUser(connectionId);
        return true;
    }

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


    public LoginCodes loginUser(String username, String password, Integer connectionId) {
        Users users = ServerData.getInstance().getUsers();
        users.createUser(username, password);
        return users.loginUser(username, password, connectionId);
    }

    public  boolean doesTopicExists(String topicName){
        return  getTopics().getTopic(topicName)!=null;
    }
    public boolean isSubscriptionIdAvailable(String topicName, Integer subscriptionId) {
        Topic topic = getTopics().getTopic(topicName);
        for (User user : topic.getSubscribedUsers()) {
            if (Objects.equals(user.getSubscriptionsId(topicName), subscriptionId)) {
                return false;
            }
        }
        return true;
    }

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
