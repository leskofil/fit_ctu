public class StateMachine {
    private State state;

    private final Robot robot;

    private Response response;

    public StateMachine() {
        this.state = State.USERNAME;
        this.robot = new Robot("unk");
    }

    public Response makeResponse (String message) {
        System.out.println("Get response " + state);

        if (robot.recharging) {
            return checkRecharging(message);
        }

        if (message.equals("RECHARGING")) {
            robot.setRecharging(true);
            return null;
        }

        switch (state) {
            case USERNAME:
                if (!Util.validName(message)) {
                    System.out.println("Too long username");
                    return new Response(Util.SERVER_SYNTAX_ERROR, true);
                }
                robot.setName(message);
                state = State.KEY;
                System.out.println("Username valid " + message);
                return new Response(Util.SERVER_KEY_REQUEST);
            case KEY:
                if (!Util.validId(message)) {
                    System.out.println("Too long keyId");
                    return new Response(Util.SERVER_SYNTAX_ERROR, true);
                }
                Integer code = robot.calculateCode(message);
                if (code == -1) {
                    System.out.println("Invalid key " + message + code);
                    return new Response(Util.SERVER_KEY_OUT_OF_RANGE_ERROR, true);
                } else if (code == -2) {
                    System.out.println("Invalid key " + message + code);
                    return new Response(Util.SERVER_SYNTAX_ERROR, true);
                }
                state = State.CONFIRMATION;
                System.out.println("Key valid" + code.toString());
                return new Response(code.toString() + "\u0007\u0008");
            case CONFIRMATION:
                if (!Util.validConfirmation(message)) {
                    System.out.println("Too long keyConfirmation");
                    return new Response(Util.SERVER_SYNTAX_ERROR, true);
                }
                if (!robot.compareCode(message))
                    return new Response(Util.SERVER_LOGIN_FAILED, true);
                System.out.println("Authentification ok");
                state = State.FIRST_MOVE;
                return new Response(Util.SERVER_OK + Util.SERVER_MOVE);
            case FIRST_MOVE:
                if (!Util.validOk(message)) {
                    System.out.println("Too long OK");
                    return new Response(Util.SERVER_SYNTAX_ERROR, true);
                }
                String move = robot.move(message);
                if (move == null) {
                    System.out.println("Inv ok");
                    return new Response(Util.SERVER_SYNTAX_ERROR, true);
                }

                if (move.equals("GOAL")) {
                    state = State.PICK_UP;
                    return new Response(Util.SERVER_PICK_UP);
                }
                if (move.equals("OBSTACLE")) {
                    state = State.OBSTACLE;
                    switch (robot.direction) {
                        case DOWN:
                            if (robot.oldPosition.x >= Util.goal.x) {
                                return new Response(Util.SERVER_TURN_RIGHT);
                            } else
                                return new Response(Util.SERVER_TURN_LEFT);
                        case UP :
                            if (robot.oldPosition.x < Util.goal.x) {
                                return new Response(Util.SERVER_TURN_RIGHT);
                            } else
                                return new Response(Util.SERVER_TURN_LEFT);
                        case RIGHT:
                            if (robot.oldPosition.y >= Util.goal.y) {
                                return new Response(Util.SERVER_TURN_RIGHT);
                            } else
                                return new Response(Util.SERVER_TURN_LEFT);
                        case LEFT:
                            if (robot.oldPosition.y < Util.goal.y) {
                                return new Response(Util.SERVER_TURN_RIGHT);
                            } else
                                return new Response(Util.SERVER_TURN_LEFT);
                        default:
                            return new Response(Util.SERVER_TURN_RIGHT);
                    }
                }
                return new Response(move);
            case OBSTACLE:
                if (!Util.validOk(message)) {
                    System.out.println("Too long OK");
                    return new Response(Util.SERVER_SYNTAX_ERROR, true);
                }
                if (robot.obstacleCase == 3)
                    state = State.FIRST_MOVE;
                String resp;
                switch (robot.direction) {
                    case DOWN:
                        if (robot.oldPosition.x >= Util.goal.x) {
                            resp = robot.moveObstacleR(message);
                        } else
                            resp =  robot.moveObstacleL(message);
                        break;
                    case UP :
                        if (robot.oldPosition.x < Util.goal.x) {
                            resp =  robot.moveObstacleR(message);
                        } else
                            resp =  robot.moveObstacleL(message);
                        break;
                    case RIGHT:
                        if (robot.oldPosition.y >= Util.goal.y) {
                            resp =  robot.moveObstacleR(message);
                        } else
                            resp =  robot.moveObstacleL(message);
                        break;
                    case LEFT:
                        if (robot.oldPosition.y < Util.goal.y) {
                            resp =  robot.moveObstacleR(message);
                        } else
                            resp =  robot.moveObstacleL(message);
                        break;
                    case UNKNOWN:
                        resp =  robot.moveObstacleR(message);
                        break;
                    default:
                        throw new IllegalStateException("Unexpected value: " + robot.direction);
                }
                    if (resp.equals("GOAL")) {
                    state = State.PICK_UP;
                    return new Response(Util.SERVER_PICK_UP);
                }
                return new Response(resp);
            case PICK_UP:
                if (!Util.validMessage(message)) {
                    System.out.println("Inv message");
                    return new Response(Util.SERVER_SYNTAX_ERROR, true);
                }
                Response res = new Response(Util.SERVER_LOGOUT);
                res.setClose(true);
                return res;
        }
        return new Response("");
    }

    public boolean validMessage(String message) {
        switch (state) {
            case USERNAME:
                if (!Util.validName(message)) {
                    System.out.println("Too long username");
                    return false;
                }
                return true;
            case KEY:
                if (!Util.validId(message)) {
                    System.out.println("Too long keyId");
                    return false;
                }
                return true;
            case CONFIRMATION:
                if (!Util.validConfirmation(message)) {
                    System.out.println("Too long keyConfirmation");
                    return false;
                }
                return true;
            case FIRST_MOVE:
            case OBSTACLE:
                if (!Util.validOk(message)) {
                    System.out.println("Too long OK");
                    return false;
                }
                return true;
            case PICK_UP:
                if (!Util.validMessage(message)) {
                    System.out.println("Inv message");
                    return false;
                }
                return true;
        }
        return true;
    }

    private Response checkRecharging(String message) {
        if (message.equals("FULL POWER")) {
            robot.setRecharging(false);
            return null;
        } else {
            return new Response(Util.SERVER_LOGIC_ERROR, true);
        }
    }

    public Integer getTimeout(){
        return 1000 * (robot.recharging ? Util.TIMEOUT_RECHARGING : Util.TIMEOUT);
    }
}

