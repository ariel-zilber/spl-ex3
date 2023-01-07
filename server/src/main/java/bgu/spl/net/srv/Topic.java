package bgu.spl.net.srv;

import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public class Topic {
    String name;


    private LinkedList<User> subscribedUsers;

    public Topic(String name) {
        name = name;
        subscribedUsers=new LinkedList<>();
    }
    public void addUser(User user){
        subscribedUsers.add(user);
    }
    public  void removeUser(User user){
        subscribedUsers.remove(user);
    }

    public LinkedList<User> getSubscribedUsers() {
        return subscribedUsers;
    }


}
