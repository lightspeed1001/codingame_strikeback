#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{

    int boostsAvailable = 1;
    int boostDistThreshhold = 1000;
    // game loop
    while (1) {
        int x;
        int y;
        int nextCheckpointX; // x position of the next check point
        int nextCheckpointY; // y position of the next check point
        int nextCheckpointDist; // distance to the next checkpoint
        int nextCheckpointAngle; // angle between your pod orientation and the direction of the next checkpoint
        cin >> x >> y >> nextCheckpointX >> nextCheckpointY >> nextCheckpointDist >> nextCheckpointAngle; cin.ignore();
        int opponentX;
        int opponentY;
        cin >> opponentX >> opponentY; cin.ignore();

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;


        // You have to output the target position
        // followed by the power (0 <= thrust <= 100)
        // i.e.: "x y thrust"
        int thrust;
        if (nextCheckpointAngle > 90 || nextCheckpointAngle < -90)
        {
            thrust = 10;
        }
        else
        {
            thrust = 100;
        }

        if(boostsAvailable > 0 && nextCheckpointDist >= boostDistThreshhold)
        {
            cout << nextCheckpointX << " " << nextCheckpointY << " BOOST" << endl;
            boostsAvailable--;
        }
        else
        {
            cout << nextCheckpointX << " " << nextCheckpointY << " " << thrust << endl;
        }
    }
}
