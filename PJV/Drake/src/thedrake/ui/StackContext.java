package thedrake.ui;

import thedrake.Move;

import java.util.List;

public interface StackContext {

    void movesFromStack(StackView stackView, List<Move> moves);

    void clearMoves(StackView stackView);

}
