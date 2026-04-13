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
        {
            R1Pressed = true;
            sBand.move_velocity(200);
            if(L1Pressed == false)
            {
                indexUp.move_velocity(200);
            }
        }else if (master.get_digital(DIGITAL_R2)) //* move block to intake (s band, reverse, put block in down-side)
        {
            sBand.move_velocity(-200);
            R1Pressed = false;
        }else
        {
            R1Pressed = false;
            sBand.move_velocity(0);
        }

        if (master.get_digital(DIGITAL_L1)) //* put block in up-side
        {
            L1Pressed = true;
            indexUp.move_velocity(-200);
            indexDown.move_velocity(600);
        }else if (master.get_digital(DIGITAL_L2)) //* put block in middle
        {
            indexUp.move_velocity(0);
            indexDown.move_velocity(-600);
            L1Pressed = false;
        }else
        {       
            L1Pressed = false;     
            if(R1Pressed == false)
            {
                indexUp.move_velocity(0);
            }
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