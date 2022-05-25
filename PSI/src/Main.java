import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class Main {
    public static void main (String[] args) {
        StateMachine stateMachine = new StateMachine();

        ServerSocket serverSocket = null;
        try {
            serverSocket = new ServerSocket(1234);
        } catch (IOException e) {
            e.printStackTrace();
        }
        log("Server is listening on port 1234");
        ServerSocket finalServerSocket = serverSocket;
        new Thread(() -> {
            while (true) {
                try {
                    Socket clientSocket = finalServerSocket.accept();
                    log("New client joined");
                    new ServerWorker(clientSocket).start();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        ).start();
    }

    private static void log(String message) {
        System.out.println("[SERVER]: " + message);
    }

}
