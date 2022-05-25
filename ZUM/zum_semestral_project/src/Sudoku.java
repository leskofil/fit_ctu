import java.io.File;
import java.io.FileNotFoundException;
import java.util.HashSet;
import java.util.Random;
import java.util.Scanner;
import java.util.Set;

public class Sudoku {
	private final int[][] initialState;

	private final int[][] currentState;

	private final int[][] neighbourState;

	public Sudoku(int[][] state) {
		this.initialState = new int[state.length][state[0].length];
		this.currentState =  new int[state.length][state[0].length];
		this.neighbourState = new int[state.length][state[0].length];
		for (int i = 0; i < state.length; i++)
			for (int j = 0; j < state[i].length; j++)
				initialState[i][j] = neighbourState[i][j] = currentState[i][j] = state[i][j];
	}

	public void printCurrentState() {
		for (int[] i : currentState) {
			for (int j = 0; j < currentState.length; j++) {
				System.out.print(i[j]);
			}
			System.out.println("");
		}
	}

	public void printInitialState() {
		for (int[] i : initialState) {
			for (int j : i) {
				System.out.print(i[j]);
			}
			System.out.println("");
		}
	}

	public int[][] copyState(int[][] stateToBeCopied) {
		int[][] tempState = new int[stateToBeCopied.length][stateToBeCopied[0].length];

		for (int i = 0; i < stateToBeCopied.length; i++)
			System.arraycopy(stateToBeCopied[i], 0, tempState[i], 0, stateToBeCopied[i].length);

		return tempState;
	}

	public int[][] getCurrentState() {
		return copyState(currentState);
	}

	public int[][] getInitialState() {
		return copyState(initialState);
	}


	public void setCurrentState(int[][] state) {
		for (int i = 0; i < state.length; i++)
			System.arraycopy(state[i], 0, currentState[i], 0, state.length);
	}

	public void initialize() {
		Random randomGenerator = new Random();
		Set<Integer> set = new HashSet<Integer>();
		for (int i = 0; i < currentState.length; i++) {
			for (int j = 0; j < currentState[i].length; j++) {
				set.add(initialState[i][j]);
			}
			for (int x = 0; x < currentState[i].length; ) {
				int next = randomGenerator.nextInt(currentState.length) + 1;
				if (!set.contains(next) && initialState[i][x] == 0) {
					currentState[i][x] = next;
					set.add(next);
					x++;
				} else if (initialState[i][x] != 0) {
					x++;
				}
			}
			set.clear();
		}
	}

	public int verify(int [][] currentState) {
		int incorrect = 0;
		int rowSize = currentState.length;
		int colSize = currentState[0].length;
		long squareSize = Math.round(Math.sqrt(currentState.length));
		boolean[][] row = new boolean[rowSize][colSize];
		boolean[][] column = new boolean[rowSize][colSize];
		boolean[][] square = new boolean[rowSize][colSize];
		int value, square_no;

		for (int x = 0; x < rowSize; x++) {
			for (int y = 0; y < colSize; y++) {
				value = currentState[x][y] - 1;

				if ((value < 0) || (value >= rowSize)) {
					incorrect++;
				}

				square_no = (int) ((x / squareSize) + (y / squareSize) * squareSize);

				if (row[y][value] ||
						column[x][value] ||
						square[square_no][value]) {

					incorrect++;

				} else {
					row[y][value] = true;
					column[x][value] = true;
					square[square_no][value] = true;
				}
			}
		}
		return incorrect;
	}

	public Sudoku(File file) {
		Scanner sc = null;
		try {
			sc = new Scanner(file);
		} catch (FileNotFoundException e) {
			System.out.println("File not found! Application terminated\n" + e.toString());
			System.exit(1);
		}
		String line = sc.nextLine();
		int size = Integer.parseInt(line);

		this.initialState = new int[size][size];
		this.currentState = new int[size][size];
		this.neighbourState = new int[size][size];

		for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++)
				{
					initialState[i][j] = neighbourState[i][j] = currentState[i][j] = sc.nextInt();
				}
			}
			sc.close();
		}

}
