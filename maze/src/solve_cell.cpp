#include <a_star.h>
#include <maze.h>

using namespace std;
using namespace ecn;

// a node is a x-y position, we move from 1 each time
class Position : public Point
{
    typedef std::unique_ptr<Position> PositionPtr;

public:
    // constructor from coordinates
    Position(int _x, int _y) : Point(_x, _y) {}

    // constructor from base ecn::Point
    Position(ecn::Point p) : Point(p.x, p.y) {}

    int distToParent()
    {
        // in cell-based motion, the distance to the parent is always 1
        return 1;
    }

    std::vector<PositionPtr> children()
    {
        // this method should return  all positions reachable from this one
        std::vector<PositionPtr> generated;
        // Create new variables for Position, to prevent modifying the current node
        int new_x;
        int new_y;
        
        // Loop through four neighbours since we move cell after cell
        for (int i = 0 ; i < 4 ; i++) {
            switch(i) {
                case(0):
                    new_x = x + 1;
                    new_y = y;
                    break;
                case(1):
                    new_x = x - 1;
                    new_y = y;
                    break;
                case(2):
                    new_y = y + 1;
                    new_x = x;
                    break;
                case(3):
                    new_y = y - 1;
                    new_x = x;
                    break;
            }

			// if the cell is a free cell, add it yto the vector.
			if (maze.isFree(new_x, new_y)) {
                generated.push_back(std::make_unique<Position>(new_x, new_y));
            }
        }

        return generated;
    }
};



int main( int argc, char **argv )
{
    // load file
    std::string filename = "maze.png";
    if(argc == 2)
        filename = std::string(argv[1]);

    // let Point know about this maze
    Position::maze.load(filename);

    // initial and goal positions as Position's
    Position start = Position::maze.start(),
             goal = Position::maze.end();

    // call A* algorithm
    ecn::Astar(start, goal);

    // save final image
    Position::maze.saveSolution("cell");
    cv::waitKey(0);

}
