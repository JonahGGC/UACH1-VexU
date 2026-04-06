#include "lemlib_config.hpp"

// TODO Creation of the drivetrain in Lemlib
lemlib::Drivetrain drivetrain(&leftTrain,              //* Left-side drivetrain
                              &rightTrain,             //* Left-side drivetrain
                              11.5,                     //* Width robot (distance between the right wheels and left wheels)
                              lemlib::Omniwheel::NEW_4, //* Type of wheels (to get their dimension)
                              200,                      //* RPM motors of the drivetrain
                              2                         //* Horizontal drift (default)
);
//* Define the encoder into lemlib
//* values -> encoder, wheel diameter, distance between the wheel and the center of rotation (all in inches)
lemlib::TrackingWheel trackingVertical(&verticalEncoder, 2.5, 0);
//* Define sensors in Lemlib
// TODO Creation of sensors in Lemlib.
lemlib::OdomSensors sensors(&trackingVertical, //* Vertical tracking wheel 1
                            nullptr,            //* Vertical tracking wheel 2
                            nullptr,            //* Horizontal tracking wheel 1
                            nullptr,            //* Horizontal tracking wheel 2
                            &inertialSensor           //* Inertial sensor
);
// TODO PID Values, only the * (highlighted) values has been calibrated
//* PID Lineal
//  Configuration of lineal PID             Default values, only for reference.
lemlib::ControllerSettings lateralController(
    12.5, //* proportional gain (kP)                       [10]
    0.25,     // integral gain (kI)                           [0]
    3,    //* derivative gain (kD)                         [3]
    3,    // anti windup                                  [3]
    1,    // small error range, in inches                 [1]
    100,  // small error range timeout, in milliseconds     [100]
    3,    // large error range, in inches                   [3]
    500,  // large error range timeout, in milliseconds     [500]
    5     // maximum acceleration (slew)                  [20]
);
//* PID Angular
// Configuration of spins/angular PID (angles)
lemlib::ControllerSettings angularController(
    6.8, //* proportional gain (kP)                        [2]
    0.5,     // integral gain (kI)                            [0]
    74,  //* derivative gain (kD)                          [10]
    3,   // anti windup                                   [3]
    0.5, // small error range, in degrees                 [1]
    200, // small error range timeout, in milliseconds    [100]
    3,   // large error range, in degrees                 [3]
    500, // large error range timeout, in milliseconds    [500]
    5    // maximum acceleration (slew)                   [0]
);
// TODO This is to smooth the use of the driver controller
//? This graphics show the RPM as a function of input power: https://www.desmos.com/calculator/uf8r0v3u1r
//*  Relacion de la entrada de joystick para velocidad lineal (Rectas)
//* Relation of the joystick input for lineal speed (lines)
lemlib::ExpoDriveCurve throttleCurve(5,    // *"Drift" of the joysticks [127]
                                      7,    //* minimum output where drivetrain will move out of 127
                                      1.019 //* expo curve gain
);


//* Relation of the joystick input for angular velocities (spins)
lemlib::ExpoDriveCurve steerCurve(7,    //* joystick deadband out of 127
                                   10,   //* minimum output where drivetrain will move out of 127
                                   1.019 //* expo curve gain
);
//* Create the chassis.
lemlib::Chassis chassis(drivetrain, lateralController, angularController, sensors, &throttleCurve, &steerCurve);