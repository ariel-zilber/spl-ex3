package bgu.spl.net.srv;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import static bgu.spl.net.srv.LoginCodes.*;

public class Users {

    private ConcurrentHashMap<String, User> registeredUsers; // key: userName value: User
    private ConcurrentHashMap<Integer, User> activeUsers; // key: connectionId value: User

    /**
     *
     */
    public Users() {
        registeredUsers = new ConcurrentHashMap<>();
        activeUsers = new ConcurrentHashMap<>();
    }


    /**
     * Creates a user
     * @param username
     * @param password
     */
    public void createUser(String username, String password) {
        // new user
        if (registeredUsers.get(username) == null) {
            registeredUsers.put(username, new User(username, password));
        }
    }

    /**
     *  Performs login for a given user
     * @param username
     * @param password
     * @param connectionId
     * @return
     */
    public LoginCodes loginUser(String username, String password, Integer connectionId) {
        // cannot login not existing user
        if (registeredUsers.get(username) == null) {
            return USER_NOT_EXISTS;
        }
        boolean someUserIsConnected = registeredUsers.get(username).getConnectionId() != -1;
        boolean currentUserIsConnected = registeredUsers.get(username).getConnectionId() == connectionId;
        boolean samePassword = registeredUsers.get(username).getUserPassword().equals(password);

        // check if user connected
        if (activeUsers.get(connectionId) != null) {
            return CLIENT_LOGGED_IN;
        }

        // check if  another user is  connected
        if (someUserIsConnected && !currentUserIsConnected) {
            return USER_LOGGED_IN;
        }

        // check password validity
        if (!samePassword) {
            return WRONG_PASSWORD;
        }

        // case user is not connected update the connection id of the registered user
        registeredUsers.get(username).setConnectionId(connectionId);

        // add the registered users to the list of active users
        activeUsers.put(connectionId, registeredUsers.get(username));
        return NEW_USER;
    }

    /**
     * Logges out a user
     *
     * @param connectionId
     */
    public void logoutUser(Integer connectionId) {
        if (activeUsers.get(connectionId) != null) {

            // disconnect the user
            activeUsers.get(connectionId).disconnect();

            // remove from active users list
            activeUsers.remove(connectionId);
        }
    }

    /***
     * Get user by associated connectionID
     * @param connectionID
     * @return
     */
    public User getUserById(Integer connectionID) {
        return activeUsers.get(connectionID);
    }

}
