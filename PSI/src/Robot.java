public class Robot {
    public Position oldPosition;
    public Direction direction;
    private String keyId;
    private Integer hash;
    private String name;
    Boolean recharging;
    Boolean hasTurned = false;
    int obstacleCase = 0;

    Robot (String name) {
        this.name = name;
        this.direction = Direction.UNKNOWN;
        this.oldPosition = null;
        this.hash = -1;
        this.keyId = null;
        this.recharging = false;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer calculateHash () {
        hash = 0;
        for (int i = 0; i < name.length(); i++) {
            hash += name.charAt(i);
        }
        hash = (hash * 1000) % 65536;
        System.out.println(hash);
        return hash;
    }

    public Integer calculateCode (String keyId) {
        this.keyId = keyId;
        try {
            double d = Integer.parseInt(keyId);
        } catch (NumberFormatException nfe) {
            return -2;
        }
        Integer serverCode = Util.getServerKey(keyId);
        if (serverCode == -1)
            return -1;
        return (serverCode + calculateHash()) % 65536;
    }


    public boolean compareCode (String clientCode) {
        Integer clientCod = Integer.parseInt(clientCode);
        clientCod = Math.floorMod(clientCod - Util.getClientKey(keyId), 65536);
        return clientCod.equals(hash);
    }

    public String move1 (Position newPosition) {
        System.out.println(newPosition.x + " " + newPosition.y);
        if (oldPosition.isEqual(newPosition) && !hasTurned) {
            obstacleCase = 0;
            hasTurned = false;
            return new String("OBSTACLE");
        } else {
            oldPosition = newPosition;
            hasTurned = false;
            return Util.SERVER_MOVE;
        }
    }

    public String turn (Position newPosition) {
        System.out.println(direction);

        if (direction == Direction.UNKNOWN) {
            obstacleCase = 0;
            return "OBSTACLE";
        }
        direction = turnRight();
        hasTurned = true;
        return Util.SERVER_TURN_RIGHT;
    }

    public String move (String position){
        Position newPosition;
        try {
            newPosition = new Position(position);
        } catch (Exception e) {
            return null;
        }

        if (newPosition.isEqual(Util.goal))
            return new String("GOAL");
        if (oldPosition == null) {
            oldPosition = newPosition;
            return Util.SERVER_MOVE;
        } else if (direction == Direction.UNKNOWN) {
            direction = determineDirection(newPosition);
        }
        String res;
        if (checkDirection(newPosition)) {
            System.out.println(direction);
            res = turn(newPosition);
        } else {
            res = move1(newPosition);
        }
        return res;
    }


    public String moveObstacleR(String position) {
        Position newPosition;
        try {
            newPosition = new Position(position);
        } catch (Exception e) {
            return Util.SERVER_SYNTAX_ERROR;
        }
        System.out.println(newPosition.x + " " + newPosition.y);
        System.out.println(direction);
        if (newPosition.isEqual(Util.goal))
            return new String("GOAL");
        switch (obstacleCase) {
            case 0 :
                turnRight();
                obstacleCase++;
                oldPosition = newPosition;
                return Util.SERVER_MOVE;
            case 2 :
                obstacleCase++;
                oldPosition = newPosition;
                return Util.SERVER_MOVE;
            case 1 :
                obstacleCase++;
                direction = determineDirection(newPosition);
                direction = turnRight();
                direction = turnRight();
                direction = turnRight();
                return Util.SERVER_TURN_LEFT;
        }
        if (checkDirection(newPosition)) {
            System.out.println(direction);
            direction = turnRight();
            hasTurned = true;
            return Util.SERVER_TURN_RIGHT;
        }
        oldPosition = newPosition;
        return Util.SERVER_MOVE;
    }

    public String moveObstacleL(String position) {
        Position newPosition;
        try {
            newPosition = new Position(position);
        } catch (Exception e) {
            return Util.SERVER_SYNTAX_ERROR;
        }
        System.out.println(newPosition.x + " " + newPosition.y);
        System.out.println(direction);
        if (newPosition.isEqual(Util.goal))
            return new String("GOAL");
        switch (obstacleCase) {
            case 0 :
                turnRight();
                turnRight();
                turnRight();
                obstacleCase++;
                oldPosition = newPosition;
                return Util.SERVER_MOVE;
            case 2 :
                obstacleCase++;
                oldPosition = newPosition;
                return Util.SERVER_MOVE;
            case 1 :
                obstacleCase++;
                direction = determineDirection(newPosition);
                direction = turnRight();
                return Util.SERVER_TURN_RIGHT;
        }
        if (checkDirection(newPosition)) {
            System.out.println(direction);
            hasTurned = true;
            direction = turnRight();
            return Util.SERVER_TURN_RIGHT;
        }
        oldPosition = newPosition;
        return Util.SERVER_MOVE;
    }

    private Direction turnRight () {
        if (direction.equals(Direction.UP))
            return Direction.RIGHT;
        if (direction.equals(Direction.RIGHT))
            return  Direction.DOWN;
        if (direction.equals(Direction.DOWN))
            return Direction.LEFT;
        if (direction.equals(Direction.LEFT))
            return Direction.UP;
        return Direction.UNKNOWN;
    }

    private Direction determineDirection(Position newPosition) {
        if (newPosition.x < oldPosition.x) {
            return Direction.LEFT;
        } else if (newPosition.x > oldPosition.x) {
            return Direction.RIGHT;
        }  else if (newPosition.y > oldPosition.y) {
            return Direction.UP;
        } else if (newPosition.y < oldPosition.y) {
            return Direction.DOWN;
        } else {
            return Direction.UNKNOWN;
        }
    }

    private Boolean checkDirection (Position newPosition) {
        if (direction == Direction.UNKNOWN) {
            return true;
        }
        if (newPosition.x < Util.goal.x && direction != Direction.RIGHT)
            return true;
        else if (newPosition.x > Util.goal.x && direction != Direction.LEFT)
            return true;
        if (newPosition.x == Util.goal.x) {
             if (newPosition.y > Util.goal.y && direction != Direction.DOWN)
                return true;
            else if (newPosition.y < Util.goal.y && direction != Direction.UP)
                return true;
        }
        return false;
    }

    public void setRecharging(Boolean recharging) {
        this.recharging = recharging;
    }

}
