package thedrake.ui;

import thedrake.GameState;
import thedrake.PlayingSide;

public interface BoardContext {

    void updateStack(BoardView boardView, GameState gameState);

    void updateCaptured(BoardView boardView, GameState gameState);

    void endGame(BoardView boardView, PlayingSide side);
}
