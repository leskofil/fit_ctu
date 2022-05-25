package lab1;

import java.util.LinkedList;
import java.util.Scanner;


public class DFS {

    private final LinkedList<Node> path;
    public int visitedNodesCount;
    private int flag = 0;

    public DFS() {
        path = new LinkedList<>();
    }

    public LinkedList<Node> findPath(Maze maze, Node start) {
        if (flag == 0) {
            Scanner in = new Scanner(System.in);
            flag = (in.nextInt());
            maze.printMaze();
        }
        maze.array[start.getRow()][start.getCol()] = 'V';
        visitedNodesCount++;
        if (start.equalNode(start, maze.end)) {
            path.add(start);
            return path;
        }
        System.out.println(visitedNodesCount);

        for (Node n : start.neighbours(maze, start)) {
            if (!(maze.array[n.getRow()][n.getCol()] == 'V')) {
                findPath(maze, n);
                if (!path.isEmpty()) {
                    path.add(start);
                    return path;
                }
            }
        }
        return null;
    }

}
