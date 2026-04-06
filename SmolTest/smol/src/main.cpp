#include "main.h"
#include "robot_config.hpp" 
#include "lemlib_config.hpp"
#include "routines.hpp"
#include "control_config.hpp"
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
    pros::lcd::initialize(); //* Init the lcd of the brain
    //inertial.reset();      //* Reset the inertial sensor
    //chassis.calibrate();   //* Lemlib calibrate the chassis


    pros::lcd::set_text(1, "Calibrando... NO TOCAR");
    chassis.calibrate(); // Calibra el IMU
   
    // IMPORTANTE: Pausa de seguridad para asegurar que la calibración terminó
    pros::delay(500);
    pros::lcd::set_text(1, "Sistema Listo. IMU OK.");

    // chassis.setBrakeMode(MOTOR_BRAKE_COAST);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous()
{
    pros::Task display(displayInformation);
    // Comment the function you don't want to run in autonomous
    matchAutonomous();
    //skillsAutonomous();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol()
{
    // TODO threads for the use of the robot
    pros::Task movementThread(taskMovement);
    pros::Task controllerInputThread(taskControlInput);
    pros::Task display(displayInformation);


    pros::delay(5); //* Delay to don't overcharge the brain
}