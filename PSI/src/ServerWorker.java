import java.io.*;
import java.net.Socket;
import java.util.Scanner;

public class ServerWorker {
    private final Socket clientSocket;

    private final StateMachine stateMachine;

    private Response response;

    Scanner in;

    DataOutputStream out;

    String buff;

    public ServerWorker(Socket clientSocket) {
        this.clientSocket = clientSocket;
        this.stateMachine = new StateMachine();
        this.response = null;
        this.buff = new String("");
    }


    public void start() {
        new Thread(this::work).start();
    }

    protected void work() {
        try (Scanner in = new Scanner (new BufferedReader( new InputStreamReader(clientSocket.getInputStream()))).useDelimiter("\u0007\u0008");
             PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true)
        ) {
            while (true) {
                response = null;
                clientSocket.setSoTimeout(stateMachine.getTimeout());
                if (in.hasNext() && !in.hasNext(".*\\z") ) { //message until delimiter or end
                   response =  stateMachine.makeResponse(in.next());
                   buff = "";
                } else { //message not complete
                    buff = buff.concat(in.next());
                    if (!stateMachine.validMessage(buff)) {
                       response = new Response(Util.SERVER_SYNTAX_ERROR, true);
                    }
                }
                if (response != null) {
                    System.out.println(response.getResponse());
                    out.write(response.getResponse());
                    out.flush();

                    if (response.getClose()) {
                        //close();
                        break;
                    }
                }
            }
        } catch (Exception e) {
           e.printStackTrace();
            //close();
        }
        close();
    }

    private void close(){
        try {
            System.out.println("Close");
            clientSocket.close();
            //out.close();
            //in.close();
        }catch (Exception e){
            System.out.println("Can't close" + e);
        }
    }

}
