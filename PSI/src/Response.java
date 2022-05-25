import java.io.Serializable;

public class Response implements Serializable {
    private String response;
    private Boolean close = false;

    public Response(String response, boolean close) {
        this.response = response;
        this.close = close;
    }
    public Response(String response) {
        this.response = response;
    } // add close

    public String getResponse() {
        return response;
    }

    public Boolean getClose() {
        return close;
    }

    public void setClose(Boolean close) {
        this.close = close;
    }
}

