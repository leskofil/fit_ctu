package lab1;

import java.util.*;

public class Astar {
    int visitedNodesCount;
    int flag = 0;
    private LinkedList<Node> open;
    private Map<Node, Node> prev;
    private List<Node> path;

    public List<Node> findPath(Maze maze, Node startNode, Node endNode) {
        ArrayList<Node> arrayList = new ArrayList<>();
        prev = new HashMap<Node, Node>();
        PriorityQueue<Node> pg = new PriorityQueue<>(200, new cmp());

        Map<Node, Double> dist = new HashMap<Node, Double>();

        Node n1 = new Node(startNode.getRow(), startNode.getCol(), endNode);
        visitedNodesCount++;
        pg.add(n1);
        dist.put(n1, 0.0);
        Node current;
        double lenght;

        while (!pg.isEmpty()) {
            //maze.printMaze();
            current = pg.remove();
            if (startNode.equalNode(current, maze.end)) {
                return buildPath(n1, current);
            }
            for (Node y : current.neighbours(maze, current)) {
                lenght = calculateDistance(y, current) + dist.get(current);
                if (maze.array[y.getRow()][y.getCol()] == 'V')
                    continue;
                if (maze.array[y.getRow()][y.getCol()] == 'O') {
                    if (lenght < dist.get(y)) {
                        dist.remove(y);
                        dist.put(y, lenght);
                        prev.remove(y);
                        prev.put(y, current);
                    }
                } else {
                    maze.array[y.getRow()][y.getCol()] = 'O';
                    visitedNodesCount++;
                    if (flag == 0) {
                        Scanner in = new Scanner(System.in);
                        flag = (in.nextInt());
                        maze.printMaze();
                    }
                    y.distance = lenght + calculateDistance(y, endNode);
                    pg.add(y);
                    dist.put(y, lenght);
                    prev.put(y, current);
                }
            }

            maze.array[current.getRow()][current.getCol()] = 'V';
        }

        return null;
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

    public double calculateDistance(Node a, Node b) {
        return Math.sqrt((a.getCol() - b.getCol()) * (a.getCol() - b.getCol()) + (a.getRow() - b.getRow()) * (a.getRow() - b.getRow()));
    }
}


