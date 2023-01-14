package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;

public class BlockingConnectionHandler  implements Runnable, ConnectionHandler<String> {

    private final StompMessagingProtocol<String> protocol;
    private final MessageEncoderDecoder<String>  encdec;
    private final Socket sock;
    private BufferedInputStream in;
    private BufferedOutputStream out;
    private volatile boolean connected = true;
    private Connections<String> connections;
    private int connectionHandlerId;

    public BlockingConnectionHandler(Socket sock,
                                     MessageEncoderDecoder<String>  reader,
                                     StompMessagingProtocol<String> protocol,
                                     Connections<String> connections,
                                     int connectionHandlerId ) {
        this.sock = sock;
        this.encdec = reader;
        this.protocol = protocol;
        this.connections = connections;
        this.connectionHandlerId = connectionHandlerId;
    }

    @Override
    public void run() {
        try (Socket sock = this.sock) { //just for automatic closing
            int read;

            in = new BufferedInputStream(sock.getInputStream());
            out = new BufferedOutputStream(sock.getOutputStream());
            protocol.start(connectionHandlerId,connections);

            while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
                String nextMessage = encdec.decodeNextByte((byte) read);

                if (nextMessage != null) {
                    protocol.process(nextMessage);
                }
            }
            connections.disconnect(connectionHandlerId);

        } catch (IOException ex) {
            ex.printStackTrace();
        }

    }

    @Override
    public void close() throws IOException {
        connected = false;
        sock.close();
    }

    @Override
    public void send(String msg) {
        if (msg != null){
            try {
                out.write(encdec.encode(msg));
                out.flush();
            } catch (IOException e) {
                System.out.println("IOException: " + e.getMessage());
            }
        }
    }
}
