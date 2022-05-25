package lab1;


import java.io.File;
import java.util.List;
import java.util.Scanner;

import static java.lang.Integer.parseInt;

public class Main {

    public static void main(String[] args) {
        rumAlgorithm();
    }

    private static void rumAlgorithm() {
        List<Node> path = null;
        while (true) {
            Scanner s = new Scanner(System.in);
            System.out.println("Write path to file.");
            String file = s.nextLine();
            File file1 = new File(file);

            Maze maze = new Maze(file1);
            printMenu();

            int number = parseInt(s.nextLine());

            switch (number) {
                case (1) -> {
                    DFS dfs = new DFS();
                    path = dfs.findPath(maze, maze.start);
                    printMaze(path, maze);
                    printHead();
                    printData(dfs.visitedNodesCount, path.size() - 1);
                }
                case (2) -> {
                    BFS bfs = new BFS();
                    path = bfs.findPath(maze, maze.start);
                    printMaze(path, maze);
                    printHead();
                    printData(bfs.visitedNodesCount, path.size() - 1);
                }
                case (3) -> {
                    RandomSearch randomSearch = new RandomSearch();
                    path = randomSearch.findPath(maze, maze.start);
                    printMaze(path, maze);
                    printHead();
                    printData(randomSearch.visitedNodesCount, path.size() - 1);
                }
                case (4) -> {
                    GreedyAlgo bestSearch = new GreedyAlgo();
                    path = bestSearch.findPath(maze, maze.start, maze.end);
                    printMaze(path, maze);
                    printHead();
                    printData(bestSearch.visitedNodesCount, path.size() - 1);
                }
                case (5) -> {
                    Astar astar = new Astar();
                    path = astar.findPath(maze, maze.start, maze.end);
                    printMaze(path, maze);
                    printHead();
                    printData(astar.visitedNodesCount, path.size() - 1);
                }
                case (6) -> {
                    System.out.println("Exit");
                    System.exit(0);
                }
                default -> System.exit(0);
            }
        }
    }

    private static void printMaze(List<Node> path, Maze maze) {
        for (Node n : path) {
            maze.array[n.getRow()][n.getCol()] = 'P';

            maze.array[maze.start.getRow()][maze.start.getCol()] = 's';
            maze.array[maze.end.getRow()][maze.end.getCol()] = 'e';
        }
        maze.printMaze();
    }

    private static void printData(int i, int p) {
        System.out.println("Nodes expanded: " + i);
        System.out.println("Path length: " + p);
    }

    static void printMenu() {
        String[] programmers = {
                "Choose algorithm",
                "1. DFS",
                "2. BFS",
                "3. Random Search",
                "4. Greedy algorithm",
                "5. A*",
                "6. Quit"
        };
        for (String programmer : programmers) {
            System.out.println(programmer);
        }
    }

    static void printHead() {
        String[] programmers = {
                "---------------------------------------",
                "s Start",
                "e End",
                "O Opened node",
                "P Path",
                "X Wall",
                "space Fresh node",
                "---------------------------------------"
        };
        for (String programmer : programmers) {
            System.out.println(programmer);
        }
    }
}