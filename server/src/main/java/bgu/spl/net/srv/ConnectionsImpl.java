package bgu.spl.net.srv;

import jdk.internal.net.http.common.Pair;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ConnectionsImpl<T> implements Connections<T> {
    private Map<Integer, ConnectionHandler<T>> connectionsMap;

    //

    private Topics topics;
    private  Users users;
    public ConnectionsImpl() {
        connectionsMap = new HashMap<>();
        topics = new Topics();
        users=new Users();
    }

    public void addConnection(Integer connectionId,ConnectionHandler<T> handler){
        connectionsMap.put(connectionId,handler);
    }
    public ConnectionHandler<T> getConnection(int connectionId) {
        return connectionsMap.get(connectionId);
    }


    /**
     * @param connectionId
     * @param msg
     * @return
     */
    @Override
    public boolean send(int connectionId, T msg) {

        //
        if (getConnection(connectionId) == null) {
            return false;
        }

        //
        getConnection(connectionId).send(msg);
        return true;
    }

    /**
     * @param channel
     * @param msg
     */
    @Override
    public void send(String channel, T msg) {
        // todo
        List<Pair<Integer,Integer>> listOfIdsPairs=topics.getTopicIds(channel);
        for(Pair<Integer,Integer> pair :listOfIdsPairs){
            send(pair.first,msg);
        }
    }

    /**
     * @param connectionId
     */
    @Override
    public void disconnect(int connectionId) {
        topics.disconnectUser(connectionId);
        connectionsMap.remove(connectionId);
        //todo

    }


}
