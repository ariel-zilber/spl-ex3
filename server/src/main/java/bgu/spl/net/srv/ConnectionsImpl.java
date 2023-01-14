package bgu.spl.net.srv;


import java.io.IOException;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

public class ConnectionsImpl<T> implements Connections<T> {
    private ConcurrentHashMap<Integer, ConnectionHandler<T>> connectionHandlers;

    //

    private Topics topics;
    private Users users;

    public ConnectionsImpl() {
        connectionHandlers = new ConcurrentHashMap<>();
        topics = new Topics();
        users = new Users();
    }


    /**
     * @param connectionId
     * @param msg
     * @return
     */
    @Override
    public boolean send(int connectionId, T msg) {
        if (msg == null) {
            return false;
        }
        if (!connectionHandlers.containsKey(connectionId)) {
            return false;
        }
        connectionHandlers.get(connectionId).send(msg);
        return true;
    }

    /**
     * @param channel
     * @param msg
     */
    @Override
    public void send(String channel, T msg) {
        if(msg==null){
            return;
        }

        List<User> subscribedUsers=ServerData.getInstance().getTopics().getTopic(channel).getSubscribedUsers();
        if(subscribedUsers==null){
            return;
        }

        for(User user:subscribedUsers){
            send(user.getConnectionId(),msg);
        }
    }

    public void addConnection(Integer connectionId, ConnectionHandler<T> handler) {
        connectionHandlers.put(connectionId, handler);
    }

    public ConnectionHandler<T> getConnection(int connectionId) {
        return connectionHandlers.get(connectionId);
    }

    /**
     * @param connectionId
     */
    @Override
    public void disconnect(int connectionId) {
        try {
            ConnectionHandler<T> handler = connectionHandlers.get(connectionId);
            handler.close();
        } catch (IOException e) {
            System.out.println("Conception close failed :" + e.getMessage());
        }
        connectionHandlers.remove(connectionId);
    }


}
