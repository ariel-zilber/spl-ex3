package bgu.spl.net.api.clientFrames;

import bgu.spl.net.api.Frame;
import bgu.spl.net.api.StompMessagingProtocolImp;
import bgu.spl.net.api.serverFrames.ConnectedServerFrame;
import bgu.spl.net.api.serverFrames.ErrorServerFrame;
import bgu.spl.net.api.serverFrames.ReceiptServerFrame;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.LoginCodes;
import bgu.spl.net.srv.ServerData;

import java.util.Collections;
import java.util.Map;

public class ConnectClientFrame extends Frame {
    public ConnectClientFrame(String msg) {
        super(msg);
    }

    /**
     * @param connectionId
     * @param connections
     */
    @Override
    public void process(Integer connectionId, Connections<String> connections, StompMessagingProtocolImp protocol) {
        Map<String, String> headers = this.getHeaders();
        String login = headers.get("login");
        String passcode = headers.get("passcode");
        // check whenever there are format errors
        // todo
        if (!headers.get("accept-version").equals("1.0")) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("The only accepted version is 1.2")).process(connectionId, connections, protocol);
            return;
        }

        if (!headers.get("host").equals("stomp.cs.bgu.ac.il")) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("The hostname must be stomp.cs.bgu.ac.il")).process(connectionId, connections, protocol);
            return;
        }

        if (login == null) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("Login username was not provided")).process(connectionId, connections, protocol);
            return;
        }
        if (passcode == null) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("Login password was not provided")).process(connectionId, connections, protocol);
            return;
        }

        // attempt login
        LoginCodes loginCodes = ServerData.getInstance().loginUser(login, passcode, connectionId);
        System.out.println("[ConnectClientFrame] ");

        // respond accordingly
        switch (loginCodes) {
            case CLIENT_LOGGED_IN:
                ErrorServerFrame.createFrame(this, Collections.singletonList("Client has already logged in")).process(connectionId, connections, protocol);
                break;
            case SOCKET_ERROR:
                ErrorServerFrame.createFrame(this, Collections.singletonList("Socket error")).process(connectionId, connections, protocol);
                break;
            case USER_EXISTS:
            case NEW_USER:
                ConnectedServerFrame.createFrame().process(connectionId, connections, protocol);
                if (headers.containsKey("receipt-id")) {
                    ReceiptServerFrame.createFrame(headers.get("receipt-id")).process(connectionId, connections, protocol);
                }
                break;
            case USER_LOGGED_IN:
                ErrorServerFrame.createFrame(this, Collections.singletonList("User is already logged in")).process(connectionId, connections, protocol);
                break;
            case WRONG_PASSWORD:
                ErrorServerFrame.createFrame(this, Collections.singletonList("Login password was invalid")).process(connectionId, connections, protocol);

        }
    }
}
