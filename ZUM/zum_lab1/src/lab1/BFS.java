package lab1;

import java.util.*;


public class BFS {

    int visitedNodesCount;
    private LinkedList<Node> opened;
    private HashSet<Node> closed;
    private Map<Node, Node> prev;
    private List<Node> path;
    private int flag = 0;

    public BFS() {
        this.opened = new LinkedList<>();
        path = new LinkedList<>();
    }

    public List<Node> findPath(Maze maze, Node startNode) {
        Node current;
        opened = new LinkedList<Node>();
        closed = new HashSet<Node>();
        prev = new HashMap<Node, Node>();
        path = null;
        opened.add(startNode);
        visitedNodesCount++;
        while (!opened.isEmpty()) {
            current = opened.pop();
            if (current.equalNode(current, maze.end)) {
                return buildPath(startNode, current);
            }
            for (Node y : current.neighbours(maze, current)) {
                if (flag == 0) {
                    Scanner in = new Scanner(System.in);
                    flag = (in.nextInt());
                    maze.printMaze();
                }
                if (maze.array[y.getRow()][y.getCol()] != 'V' && maze.array[y.getRow()][y.getCol()] != 'O') {
                    opened.addLast(y);
                    visitedNodesCount++;
                    maze.array[y.getRow()][y.getCol()] = 'O';
                    prev.put(y, current);
                }
            }
            maze.array[current.getRow()][current.getCol()] = 'V';
        }

        return path;
    }

    private List<Node> buildPath(Node start, Node target) {
        path = new ArrayList<Node>();
        path.add(target);
        while (start != target) {
            target = prev.get(target);
            path.add(target);
        }
        return path;
    }
}
