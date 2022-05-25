import java.lang.reflect.Array;
import java.util.HashMap;
import java.util.Map;

public class Util {
    public static final Integer SERVER_CONFIRMATION =   0;
    public static final String SERVER_MOVE =           "102 MOVE\u0007\u0008";
    public static final String SERVER_TURN_LEFT =      "103 TURN LEFT\u0007\u0008";
    public static final String SERVER_TURN_RIGHT =     "104 TURN RIGHT\u0007\u0008";
    public static final String SERVER_PICK_UP =        "105 GET MESSAGE\u0007\u0008";
    public static final String SERVER_LOGOUT =         "106 LOGOUT\u0007\u0008";
    public static final String SERVER_KEY_REQUEST =    "107 KEY REQUEST\u0007\u0008";
    public static final String SERVER_OK =             "200 OK\u0007\u0008";
    public static final String SERVER_LOGIN_FAILED =   "300 LOGIN FAILED\u0007\u0008";
    public static final String SERVER_SYNTAX_ERROR =   "301 SYNTAX ERROR\u0007\u0008";
    public static final String SERVER_LOGIC_ERROR =    "302 LOGIC ERROR\u0007\u0008";
    public static final String SERVER_KEY_OUT_OF_RANGE_ERROR =    "303 KEY OUT OF RANGE\u0007\u0008";


    public static final Integer TIMEOUT =               1;
    public static final Integer TIMEOUT_RECHARGING = 	5;

    public static final Position goal = new Position(0, 0);



    private static final String[][] keys = {{"23019", "32037"}, {"32037", "29295"}, {"18789", "13603"},
            {"16443", "29533"}, {"18189", "21952"}};




    public static boolean validName (String name) {
        if (name.contains("\u0007\u0008"))
            return false;
        return name.length() <= 18;
    }

    public static boolean validId (String id) {
        return id.length() <= 3;
    }

    public static boolean validConfirmation (String conf) {
        try {
            Integer i = Integer.parseInt(conf);
        } catch (Exception e) {
            return false;
        }
        return conf.length() <= 5;
    }

    public static boolean validOk (String ok) {
        return ok.length() <= 10;
    }

    public static boolean validRecharging (String rechar) {
        return rechar.length() <= 10;
    }

    public static boolean validFull (String full) {
        return full.length() <= 10;
    }

    public static boolean validMessage (String mess) {
        return mess.length() <= 98;
    }

    public static Integer getServerKey (String keyID) {
        int k = Integer.parseInt(keyID);
        if (k > 4 || k < 0) {
            return -1;
        }
        return Integer.parseInt(keys[k][0]);
    }

    public static Integer getClientKey (String keyID) {
        return Integer.parseInt(keys[Integer.parseInt(keyID)][1]);
    }

}