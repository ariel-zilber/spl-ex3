package bgu.spl.net.srv;


/****
 * Represents login codes
 */
public enum LoginCodes {
    SOCKET_ERROR,

    /**
     * f the client has already logged into a
     * server you should not attempt to log in again. The client should
     * simply print "The client is already logged in, log out before trying
     * again".
     */
    CLIENT_LOGGED_IN,

    /**
     * f the server connection was successful and the server
     * doesn’t find the username, then a new user is created, and the
     * password is saved for that user. Then the server sends a
     * CONNECTED frame to the client and the client will print "Login
     * successful"
     */
    NEW_USER,

    /**
     * User is already logged in: If the user is already logged in, then
     * the server will respond with a STOMP error frame indicating
     * the reason – the output, in this case, should be "User already
     * logged in".
     */
    USER_LOGGED_IN,

    /***
     * Wrong password: If the user exists and the password doesn’t
     * match the saved password, the server will send back an appro-
     * priate ERROR frame indicating the reason - the output, in this
     * case, should be "Wrong password".
     */
    WRONG_PASSWORD,

    /**
     *  User exists: If the server connection was successful, the server
     * will check if the user exists in the users’ list and if the password
     * matches, also the server will check that the user does not have an
     * active connection already. In case these tests are OK, the server
     * sends back a CONNECTED frame and the client will print to
     * the screen "Login successful"
     */
    USER_EXISTS,
    USER_NOT_EXISTS,
}
