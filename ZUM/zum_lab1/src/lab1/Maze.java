package lab1;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class Maze {
    ArrayList<String> maze;

    char[][] array;

    Node start;

    Node end;


    public Maze(File file) {
        Scanner sc = null;
        try {
            sc = new Scanner(file);
        } catch (FileNotFoundException e) {
            System.out.println("File not found! Application terminated\n" + e.toString());
            System.exit(1);
        }
        String line = sc.nextLine();
        maze = new ArrayList<>();
        while (sc.hasNextLine()) {
            if (line.charAt(0) != 'X') {
                break;
            }
            maze.add(line);
            line = sc.nextLine();
        }

        array = new char[maze.size()][];
        for (int i = 0; i < maze.size(); i++) {
            array[i] = maze.get(i).toCharArray();
        }

        String[] tokens = line.split("[ ,]+");
        start = new Node(Integer.parseInt(tokens[2]), Integer.parseInt(tokens[1]));
        //printMaze();

        array[start.getRow()][start.getCol()] = 's';

        line = sc.nextLine();
        tokens = line.split("[ ,]+");
        end = new Node(Integer.parseInt(tokens[2]), Integer.parseInt(tokens[1]));
        array[end.getRow()][end.getCol()] = 'e';
    }

    public final void printMaze() {
        for (char[] s : array) {
            System.out.println(s);
        }
        System.out.println("\n");
    }

}
