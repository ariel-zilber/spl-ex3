package bgu.spl.net.srv;

import java.util.LinkedList;

/***
 * represents a single topic
 */
public class Topic {
    private LinkedList<User> subscribedUsers;

    /**
     *
     */
    public Topic() {
        subscribedUsers=new LinkedList<>();
    }

    /**
     * Subscribe a user to the topic
     * @param user
     */
    public void addUser(User user){
        subscribedUsers.add(user);
    }

    /**
     * Unsubscribe a user from the topic
     * @param user
     */
    public  void removeUser(User user){
        subscribedUsers.remove(user);
    }

    /***
     * Returns list of all subscribed topics
     * @return
     */
    public LinkedList<User> getSubscribedUsers() {
        return subscribedUsers;
    }
}
