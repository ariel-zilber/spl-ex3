package bgu.spl.net.api.clientFrames;

import bgu.spl.net.api.Frame;
import bgu.spl.net.api.StompMessagingProtocolImp;
import bgu.spl.net.api.serverFrames.DisconnectedServerFrame;
import bgu.spl.net.api.serverFrames.ErrorServerFrame;
import bgu.spl.net.srv.*;

import java.util.Collections;
import java.util.Map;

public class DisconnectClientFrame extends Frame {
    public DisconnectClientFrame(String msg) {
        super(msg);
    }

    /**
     * @param connectionId
     * @param connections
     */
    @Override
    public void process(Integer connectionId, Connections<String> connections, StompMessagingProtocolImp protocol) {
        Map<String, String> headers = this.getHeaders();

        // check whenever there are format errors
        if (!headers.get("accept-version").equals("1.2")) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("The only accepted version is 1.2")).process(connectionId, connections, protocol);
            return;
        }
        if (!headers.get("host").equals("stomp.cs.bgu.ac.il")) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("The hostname must be stomp.cs.bgu.ac.il")).process(connectionId, connections, protocol);
            return;
        }
        if (headers.get("login") == null) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("Login username was not provided")).process(connectionId, connections, protocol);
            return;
        }
        if (headers.get("passcode") == null) {
            ErrorServerFrame.createFrame(this, Collections.singletonList("Login password was not provided")).process(connectionId, connections, protocol);
            return;
        }

        boolean success = ServerData.getInstance().disconnectUser(connectionId);
        if (success) {
            DisconnectedServerFrame.createFrame().process(connectionId, connections, protocol);
        } else {
            ErrorServerFrame.createFrame(this, Collections.singletonList("Cannot disconnecte unless connected already!")).process(connectionId, connections, protocol);
        }
    }
}
