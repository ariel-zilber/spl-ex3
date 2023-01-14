package bgu.spl.net.srv;


import java.io.IOException;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

public class ConnectionsImpl  implements Connections<String> {
    private ConcurrentHashMap<Integer, ConnectionHandler<String>> connectionHandlers;

    public ConnectionsImpl() {
        connectionHandlers = new ConcurrentHashMap<>();
    }


    /**
     * @param connectionId
     * @param msg
     * @return
     */
    @Override
    public boolean send(int connectionId, String msg) {
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
    public void send(String channel, String msg) {
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

    /***
     *
     * @param connectionId
     * @param handler
     */
    public void addConnection(Integer connectionId, ConnectionHandler<String> handler) {
        connectionHandlers.put(connectionId, handler);
    }

    /**
     * @param connectionId
     */
    @Override
    public void disconnect(int connectionId) {
        try {
            ConnectionHandler<String> handler = connectionHandlers.get(connectionId);
            handler.close();
        } catch (IOException e) {
            System.out.println("Conception close failed :" + e.getMessage());
        }
        connectionHandlers.remove(connectionId);
    }


}
