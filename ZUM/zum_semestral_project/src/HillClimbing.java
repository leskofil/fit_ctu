// The hill climbing algorithm can be found in the book Artificial
// Intelligence: A Modern Approach on page 122, as well as: 
// http://en.wikipedia.org/wiki/Hill_climbing_algorithm

import java.util.Random;

public class HillClimbing {
	private Sudoku sudoku;
	private final double N = 0.3;
	private final double B = 0.3;
	private int fitness;
	Random generator = new Random();
	private int iteration = 0;
	int neighborErrorCount;
	int currentErrorCount;
	int error = 1000;
	int [][] neighbourState;


	public HillClimbing() {
	}

	public int[][] solve(Sudoku puzzle){
		sudoku = puzzle;
		int it = 0;

		while (it != 3000) {
			sudoku.initialize();
			currentErrorCount = sudoku.verify(sudoku.getCurrentState());
			while (iteration != 500) {
				if (currentErrorCount == 0 ) {
					System.out.println("finish");
					return sudoku.getCurrentState();
				}
				if (currentErrorCount < error) {
					error = currentErrorCount;
				}
				neighbourState = sudoku.copyState(sudoku.getCurrentState());
				climb();
				iteration++;

			}
			iteration = 0;
			it++;
		}
		System.out.println(currentErrorCount);
		System.out.println(error);
		return sudoku.getCurrentState();

	}
	
	private boolean climb(){
		int temporaryValue, randomRowIndex;
		randomRowIndex = generator.nextInt(neighbourState.length);
		int[] colIndex = {generator.nextInt(neighbourState.length), generator.nextInt(neighbourState.length)};

		while(sudoku.getInitialState()[randomRowIndex][colIndex[0]] != 0
				||  sudoku.getInitialState()[randomRowIndex][colIndex[1]] != 0) {
			for(int i = 0; i < 2; i++)
				colIndex[i] = generator.nextInt(neighbourState.length);
		}

		temporaryValue = neighbourState[randomRowIndex][colIndex[0]];
		neighbourState[randomRowIndex][colIndex[0]] = neighbourState[randomRowIndex][colIndex[1]];
		neighbourState[randomRowIndex][colIndex[1]] = temporaryValue;

		neighborErrorCount = sudoku.verify(neighbourState);

		if (neighborErrorCount < currentErrorCount) {
			sudoku.setCurrentState(neighbourState);
			currentErrorCount = neighborErrorCount;
		}
		return false;
	}

	private boolean clumb() {
		//Sudoku.printCurrentState(neighbourState);
		//N_operation();
		B_Operation();
		//Sudoku.printCurrentState(neighbourState);

		neighborErrorCount = sudoku.verify(neighbourState);

		if (neighborErrorCount < currentErrorCount) {
			sudoku.setCurrentState(neighbourState);
			currentErrorCount = neighborErrorCount;
		}
		return false;
	}

	private void B_Operation() {
		for (int i = 0; i < neighbourState.length; i++) {
			for (int j = 0; j < neighbourState[i].length; j++) {
				if (sudoku.getInitialState()[i][j] != 0 ) {
						neighbourState[i][j] = generator.nextInt(neighbourState.length) + 1;
				}
			}
		}
	}

	private void N_operation() {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (sudoku.getInitialState()[i][j] != 0 && generator.nextInt(10) < 3) {
					if (neighbourState[i][j] == 9)
						neighbourState[i][j]--;
					else {
						neighbourState[i][j]++;
					}

				}
			}
		}
	}

}
