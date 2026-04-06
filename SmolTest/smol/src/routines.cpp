#include "routines.hpp"

int blockCounter = 0;

void displayInformation()
{
    while (true){
        lemlib::Pose pose = chassis.getPose();
        float distance = (distanceSensor.get() / 24.5);
        master.print(0, 0, "%.1f  %.1f ", pose.y, distance);
        //master.print(0, 0, "%d %.1f", blockCounter, distance);
        pros::delay(40);
    }
}

void skillsAutonomous()
{   
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 41, 3000, {.forwards = true, .maxSpeed = 127}, false);
    chassis.turnToHeading(-90, 3000, {}, false);
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -24, 3000, {.forwards = false, .maxSpeed = 127}, false);
    chassis.moveToPoint(0, 0, 3000, {.forwards = true, .maxSpeed = 127}, false);
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
    chassis.moveToPoint(0, 23, 3000, {.forwards = true, .maxSpeed = 127}, false);
    chassis.moveToPoint(0, 0, 3000, {.forwards = false, .maxSpeed = 127}, false);
    chassis.turnToHeading(-45, 3000, {}, false);
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -35, 3000, {.forwards = false, .maxSpeed = 127}, false);
    chassis.turnToHeading(45, 3000, {}, false);
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -60, 3000, {.forwards = false, .maxSpeed = 127}, false);
    chassis.turnToHeading(-45, 3000, {}, false);
}

bool isBlockDetected = false;

void countInBlocks(){
    sBand.move_velocity(160);
    while (blockCounter < 6)
    {
        if((distanceSensor.get() / 24.5) < 8.5 && isBlockDetected == false) 
        {
            isBlockDetected = true;
            blockCounter++;
        }else if(distanceSensor.get() / 24.5 > 8.5)
        {
            isBlockDetected = false;
        }
        pros::delay(80);
    }

    isBlockDetected = false;
    sBand.move_velocity(0);
}

void countOutBlocks(){
    sBand.move_velocity(-160);
    blockCounter = 0;  
    while (blockCounter < 3)
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
        if(blockCounter == 3){
            pros::delay(900);
        }
    }
    sBand.move_velocity(160);
    pros::delay(800);
}

void matchAutonomous()
{
    statusPistonPalette = false;
    pistonPalette.set_value(statusPistonPalette);
    //* Set origin
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -35, 3000, {.forwards = false, .maxSpeed = 127}, false);
    chassis.turnToHeading(45, 3000, {}, false);
    chassis.setPose(0, 0, 0);
    
    chassis.moveToPoint(0, -5, 3000, {.forwards = false, .maxSpeed = 127}, false); // Central goal
    //* Move to mid point
    indexDown.move_velocity(-200);
    pros::delay(800);
    indexDown.move_velocity(0);
    statusPistonPalette = true;
    pistonPalette.set_value(statusPistonPalette);
    pros::delay(500);
    chassis.moveToPoint(0, 56, 4000, {.forwards = true, .maxSpeed = 127}, false); // Mid point
    chassis.turnToHeading(-45, 3000, {}, false);
    chassis.setPose(0, 0, 0);

    
    chassis.moveToPoint(0, 6, 3000, {.forwards = true, .maxSpeed = 127}, false); // Central goal
    countInBlocks();
    chassis.moveToPoint(0, -6, 3000, {.forwards = false, .maxSpeed = 127}, false); // Central goal
    statusPistonPalette = false;
    pistonPalette.set_value(statusPistonPalette);
    chassis.turnToHeading(45, 3000, {}, false);
    countOutBlocks();
    statusPistonPalette = true;
    pistonPalette.set_value(statusPistonPalette);
    chassis.turnToHeading(0, 3000, {}, false);
    chassis.moveToPoint(0, 6, 3000, {.forwards = true, .maxSpeed = 127}, false); // Central goal
    countInBlocks();

    //* Move to the column
    chassis.moveToPoint(1, -24.5, 3000, {.forwards = false, .maxSpeed = 127}, false);
    sBand.move_velocity(160);
    indexUp.move_velocity(-200);
    indexDown.move_velocity(200);
    pros::delay(5000);
    sBand.move_velocity(0);
    indexUp.move_velocity(0);
    indexDown.move_velocity(0);
}