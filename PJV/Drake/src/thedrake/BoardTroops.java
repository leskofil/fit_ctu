package thedrake;

import java.io.PrintWriter;
import java.util.*;
import java.util.stream.Collectors;

// TODO
public class BoardTroops implements JSONSerializable {
    private final PlayingSide playingSide;
    private final Map<BoardPos, TroopTile> troopMap;
    private final TilePos leaderPosition;
    private final int guards;

    public BoardTroops(PlayingSide playingSide) {
        // Místo pro váš kód
        this.playingSide = playingSide;
        troopMap = Collections.emptyMap();
        leaderPosition = TilePos.OFF_BOARD;
        guards = 0;
    }

    public BoardTroops(
            PlayingSide playingSide,
            Map<BoardPos, TroopTile> troopMap,
            TilePos leaderPosition,
            int guards) {
        // Místo pro váš kód
        this.playingSide = playingSide;
        this.troopMap = troopMap;
        this.leaderPosition = leaderPosition;
        this.guards = guards;
    }

    public Optional<TroopTile> at(TilePos pos) {
        // Místo pro váš kód
        PositionFactory pf = new PositionFactory(pos.column());
        BoardPos boardPos = pf.pos(pos.i(), pos.j());
        TroopTile tile = troopMap.get(boardPos);

        return Optional.ofNullable(tile);
    }

    public PlayingSide playingSide() {
        // Místo pro váš kód
        return playingSide;
    }

    public TilePos leaderPosition() {
        // Místo pro váš kód
        if (isLeaderPlaced())
            return leaderPosition;
        return TilePos.OFF_BOARD;
    }

    public int guards() {
        // Místo pro váš kód
        return guards;
    }

    public boolean isLeaderPlaced() {
        // Místo pro váš kód
        return leaderPosition != TilePos.OFF_BOARD;
    }

    public boolean isPlacingGuards() {
        // Místo pro váš kód
        return leaderPosition != TilePos.OFF_BOARD && guards < 2;
    }

    public Set<BoardPos> troopPositions() {
        // Místo pro váš kód
        return new HashSet<>(troopMap.keySet());
    }

    public BoardTroops placeTroop(Troop troop, BoardPos target) {
        // Místo pro váš kód
        if (troopMap.containsKey(target))
            throw new IllegalArgumentException();

        Map<BoardPos, TroopTile> newPlace = new HashMap<>(troopMap);
        TroopTile troopTile = new TroopTile(troop, playingSide, TroopFace.AVERS);
        newPlace.put(target, troopTile);

        if (!isLeaderPlaced())
            return new BoardTroops(playingSide(), newPlace, target, guards);
        else {
            if (isPlacingGuards())
                return new BoardTroops(playingSide(), newPlace, leaderPosition, guards + 1);
            else
                return new BoardTroops(playingSide(), newPlace, leaderPosition, guards);
        }
    }


    public BoardTroops troopStep(BoardPos origin, BoardPos target) {
        // Místo pro váš kód
        if (!isLeaderPlaced() || isPlacingGuards() || troopMap.isEmpty())
            throw new IllegalStateException();

        if (at(target).isPresent() || at(origin).isEmpty())
            throw new IllegalArgumentException();

        Map<BoardPos, TroopTile> newStep = new HashMap<>(troopMap);
        TroopTile tile = newStep.remove(origin);
        newStep.put(target, tile.flipped());

        if (origin.equals(leaderPosition))
            return new BoardTroops(playingSide(), newStep, target, guards);
        else
            return new BoardTroops(playingSide(), newStep, leaderPosition, guards);

    }

    public BoardTroops troopFlip(BoardPos origin) {
        if (!isLeaderPlaced()) {
            throw new IllegalStateException(
                    "Cannot move troops before the leader is placed.");
        }

        if (isPlacingGuards()) {
            throw new IllegalStateException(
                    "Cannot move troops before guards are placed.");
        }

        if (!at(origin).isPresent())
            throw new IllegalArgumentException();

        Map<BoardPos, TroopTile> newTroops = new HashMap<>(troopMap);
        TroopTile tile = newTroops.remove(origin);
        newTroops.put(origin, tile.flipped());

        return new BoardTroops(playingSide(), newTroops, leaderPosition, guards);
    }

    public BoardTroops removeTroop(BoardPos target) {
        // Místo pro váš kód
        if (!isLeaderPlaced() || isPlacingGuards())
            throw new IllegalStateException();

        if (at(target).isEmpty())
            throw new IllegalArgumentException();

        Map<BoardPos, TroopTile> removedTroops = new HashMap<>(troopMap);

        if (!removedTroops.containsKey(target))
            throw new IllegalArgumentException();

        removedTroops.remove(target);

        if (target.equals(leaderPosition))
            return new BoardTroops(playingSide(), removedTroops, TilePos.OFF_BOARD, guards);
        else
            return new BoardTroops(playingSide(), removedTroops, leaderPosition, guards);

    }

    @Override
    public void toJSON(PrintWriter writer) {
        writer.write("{\"side\":");
        playingSide().toJSON(writer);
        writer.write(",\"leaderPosition\":");
        leaderPosition().toJSON(writer);
        writer.write(",\"guards\":" + guards() + "");
        writer.write(",\"troopMap\":{");

        Set<BoardPos> keys = troopMap.keySet();
        SortedSet<BoardPos> set = new TreeSet<BoardPos>(sorting);
        set.addAll(keys);


        for (BoardPos boardPos : set) {
            writer.write("\"" + boardPos + "\":");
            troopMap.get(boardPos).toJSON(writer);
            if (set.last() != boardPos) {
                writer.write(",");
            }

        }

        writer.write("}}");
    }

    private final Comparator<BoardPos> sorting = new Comparator<BoardPos>() {
        @Override
        public int compare(BoardPos o1, BoardPos o2) {
            return o1.toString().compareTo(o2.toString());
        }
    };

}


