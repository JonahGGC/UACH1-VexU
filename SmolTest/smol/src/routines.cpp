#include "routines.hpp"

int blockCounter = 0;

bool isBlockDetected = false;

int startTime = 0;
int elapsedTime = 0;

void displayInformation()
{
    while (true){
        lemlib::Pose pose = chassis.getPose();
        float distance = (distanceSensor.get() / 24.5);
        master.print(0, 0, "%.1f  %.1f  %d", pose.y, distance, blockCounter);
        //master.print(0, 0, "%d %.1f", blockCounter, distance);
        pros::delay(40);
    }
}

void countBlocks(int motorVelocity, int blocksToCount, int Time){
    sBand.move_velocity(motorVelocity);
    startTime = pros::millis();
    blockCounter = 0;  

    while (blockCounter < blocksToCount)
    {
        if((distanceSensor.get() / 24.5) < 8.5 && isBlockDetected == false)
        {
            isBlockDetected = true;
            blockCounter++;
        }else if(distanceSensor.get() / 24.5 > 8.5)
        {
            isBlockDetected = false;
        }
        
        pros::delay(50);
        if(blockCounter == 3 && motorVelocity < 0){
            pros::delay(200);
            sBand.move_velocity(200);
            pros::delay(300);
        }

        if((pros::millis() - startTime) >= Time){
            break;
            sBand.move_velocity(0);
        }
    }
    sBand.move_velocity(0);
}

void skillsAutonomous()
{   
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 41, 3000, {.forwards = true, .maxSpeed = 127}, false);
    chassis.turnToHeading(-90, 3000, {}, false);
    chassis.setPose(0, 0, 0);
    chassis.turnToHeading(-135, 4000, {}, false);
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 34, 3000, {.forwards = true, .maxSpeed = 127}, false);
    chassis.turnToHeading(-45, 3000, {}, false);
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 65, 3000, {.forwards = true, .maxSpeed = 127}, false);
    chassis.turnToHeading(-45, 3000, {}, false);
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 35, 3000, {.forwards = true, .maxSpeed = 127}, false);
    chassis.turnToHeading(45, 3000, {}, false);
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -10, 3000, {.forwards = false, .maxSpeed = 127}, false);
    chassis.moveToPoint(0, 18, 3000, {.forwards = true, .maxSpeed = 127}, false);
    chassis.moveToPoint(0, 0, 3000, {.forwards = false, .maxSpeed = 127}, false);
    chassis.turnToHeading(-45, 3000, {}, false);
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -35, 3000, {.forwards = false, .maxSpeed = 127}, false);
    chassis.turnToHeading(45, 3000, {}, false);
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -102, 3000, {.forwards = false, .maxSpeed = 127}, false);
    chassis.turnToHeading(-75, 3000, {}, false);
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -22, 3000, {.forwards = false, .maxSpeed = 127}, false);
}

void matchAutonomous()
{
    statusPistonPalette = false;
    pistonPalette.set_value(statusPistonPalette);

    // First segment (start position to mid point)
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -35, 3000, {.forwards = false, .maxSpeed = 127}, false);
    chassis.turnToHeading(45, 3000, {}, false);
    chassis.setPose(0, 0, 0);

    // second segment (put block on the middle)
    chassis.moveToPoint(0, -5, 3000, {.forwards = false, .maxSpeed = 127}, false);
    indexDown.move_velocity(-600);
    pros::delay(700);
    indexDown.move_velocity(0);
    statusPistonPalette = true;
    pistonPalette.set_value(statusPistonPalette);
    pros::delay(500);

    // Third segment (mid point to column)
    chassis.moveToPoint(0, 56, 4000, {.forwards = true, .maxSpeed = 127}, false);
    chassis.turnToHeading(-45, 3000, {}, false);
    chassis.setPose(0, 0, 0);

    // Fourth segment (take blocks from the column)
    chassis.moveToPoint(0, 6.5, 3000, {.forwards = true, .maxSpeed = 127}, false); 
    countBlocks(200, 6, 4000); // Take 6 blocks from the column
    chassis.moveToPoint(0, -6, 3000, {.forwards = false, .maxSpeed = 127}, false);
    statusPistonPalette = false;
    pistonPalette.set_value(statusPistonPalette);
    chassis.turnToHeading(45, 3000, {}, false);
    countBlocks(-200, 3, 3000); // Drop 3 blocks out of the robot
    statusPistonPalette = true;
    pistonPalette.set_value(statusPistonPalette);
    chassis.turnToHeading(0, 3000, {}, false);
    chassis.moveToPoint(1, 6.5, 3000, {.forwards = true, .maxSpeed = 127}, false);
    indexUp.move_velocity(200);
    countBlocks(200, 6, 4000); // Take the second 6 blocks from the column

    /// Final segment (Put blocks on the highest tube)
    chassis.moveToPoint(1, -24.5, 3000, {.forwards = false, .maxSpeed = 127}, false);
    sBand.move_velocity(200);
    indexUp.move_velocity(-200);
    indexDown.move_velocity(600);
    pros::delay(7000);
    indexUp.move_velocity(0);
    sBand.move_velocity(0);
    indexUp.move_velocity(0);
    indexDown.move_velocity(0);
    chassis.moveToPoint(0, -25, 3000, {.forwards = false, .maxSpeed = 127}, false);
}