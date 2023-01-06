package bgu.spl.net.srv;

public enum LogoutCodes {

    /**
     ** case attempting to disconnect a client which is not connected
     *
     */
    CLIENT_NOT_CONNECTED,

    /***
     *  case client was disconnected successfully
     */
    CLIENT_DISCONNECTED,
}
