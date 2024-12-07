//
// Created by jjman on 07/12/2024.
//

#ifndef OBSTACLE_MANAGER_H
#define OBSTACLE_MANAGER_H



class obstacleManager {
private:
// lane positions
    const float* _lanePositions;
// list of obstacle vectors to choose from

// spawn interval
    float spawnInterval;
public:
    void createObstacle();

};



#endif //OBSTACLE_MANAGER_H
