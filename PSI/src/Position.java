import java.security.InvalidParameterException;

public class Position {
    int x;
    int y;

    public Position (Integer x, Integer y) {
        this.x = x;
        this.y = y;
    }

    public Position(String position) throws Exception {
        String delims = " ";
        String [] tokens = position.split(delims);
        if (tokens.length != 3 || !tokens[0].equals("OK"))
            throw new InvalidParameterException(position);
        try {
            x = Integer.parseInt(tokens[1]);
            y = Integer.parseInt(tokens[2]);
        } catch (NumberFormatException nfe) {
            throw new InvalidParameterException(position);
        }
        if (!position.endsWith(tokens[2]))
            throw new InvalidParameterException(position);
    }


    boolean isEqual (Position pos) {
        return pos.x == x && pos.y == y;
    }
}
