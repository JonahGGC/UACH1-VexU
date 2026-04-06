#include "robot_config.hpp"

void taskMovement()
{
    while (true)
    {
        //* Arcade control scheme
        int dir = master.get_analog(ANALOG_LEFT_Y);
        int turn = -master.get_analog(ANALOG_RIGHT_X);
        leftTrain.move(dir - turn);
        rightTrain.move(dir + turn);
        pros::delay(50);
    }
}


void taskControlInput()
{
    while (true)
    {
        //* Intake, charge of blocks
        if (master.get_digital(DIGITAL_R1)) //* move blocks to index (s band)
            sBand.move_velocity(200);
        else if (master.get_digital(DIGITAL_R2)) //* move block to intake (s band, reverse, put block in down-side)
            sBand.move_velocity(-200);
        else
            sBand.move_velocity(0);


        if (master.get_digital(DIGITAL_L1)) //* put block in up-side
        {
            indexUp.move_velocity(-200);
            indexDown.move_velocity(200);
        }else if (master.get_digital(DIGITAL_L2)) //* put block in middle
        {
            indexUp.move_velocity(0);
            indexDown.move_velocity(-200);
        }else
        {
            indexUp.move_velocity(0);
            indexDown.move_velocity(0);
        }

        if (master.get_digital_new_press(DIGITAL_Y))
        {
            statusPistonPalette = !statusPistonPalette;
            pistonPalette.set_value(statusPistonPalette);
        }

        if(master.get_digital_new_press(DIGITAL_RIGHT))
        {
            statusPistonArm = !statusPistonArm;
            pistonArm.set_value(statusPistonArm);
        }
        pros::delay(20);
    }
}