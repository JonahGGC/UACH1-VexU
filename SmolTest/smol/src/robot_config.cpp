#include "robot_config.hpp"

// TODO Motor names and port
//! The motor has been declared seeing the robot from behind (front is the intake/indexer low and back is the indexer mid/upper )
//* Left
#define LEFT_UPPER_BACK 16
#define LEFT_UPPER_FRONT -17
#define LEFT_LOWER_BACK 19
#define LEFT_LOWER_FRONT 18
//* Right
#define RIGHT_UPPER_BACK -5
#define RIGHT_UPPER_FRONT 1
#define RIGHT_LOWER_BACK -7
#define RIGHT_LOWER_FRONT -6
// TODO Bands
//* S design
#define ENTRY_MOTOR_DOWN 2
#define ENTRY_MOTOR_UP -3
#define EXIT_MOTOR_UP -4
// This motors are the same that input in the lower goal
//* indexer
#define INDEX_MOTOR_UP 9
#define INDEX_MOTOR_DOWN 10
// If those motors rotate in opposite direction from each other the block will goes up, if the bottom motor rotate in the opposite direction, the block goes in middle
// TODO Sensors
#define INERTIAL_SENSOR 11
#define ENCODER_VERTICAL 8
#define DISTANCE_SENSOR 12
// TODO ADI ports, neumathics
#define PALETTE_PISTON 'A'
#define ARM_PISTON 'B'

// ──────────────────────────────────────────
//  Controllers
// ──────────────────────────────────────────
pros::Controller master(pros::E_CONTROLLER_MASTER);

// ──────────────────────────────────────────
//  Drive Motors
//  Negative port  = reversed direction
// ──────────────────────────────────────────
//* Drive train
pros::MotorGroup leftTrain({LEFT_UPPER_BACK, LEFT_UPPER_FRONT, LEFT_LOWER_BACK, LEFT_LOWER_FRONT});
pros::MotorGroup rightTrain({RIGHT_UPPER_BACK, RIGHT_UPPER_FRONT, RIGHT_LOWER_BACK, RIGHT_LOWER_FRONT});
//* Loader / intake / The S design
pros::MotorGroup sBand({ENTRY_MOTOR_DOWN, ENTRY_MOTOR_UP, EXIT_MOTOR_UP});
//* Indexer
pros::Motor indexUp(INDEX_MOTOR_UP);
pros::Motor indexDown(INDEX_MOTOR_DOWN);

// ──────────────────────────────────────────
//  Sensors
// ──────────────────────────────────────────
//* Define inertial sensor
pros::IMU inertialSensor(INERTIAL_SENSOR);
//* Define first encoder (y-axis)
pros::Rotation verticalEncoder(ENCODER_VERTICAL);
//* Define distance sensors
pros::Distance distanceSensor(DISTANCE_SENSOR);

// ──────────────────────────────────────────
//  Pheumatics
// ──────────────────────────────────────────
pros::ADIDigitalOut pistonPalette(PALETTE_PISTON);
pros::ADIDigitalOut pistonArm(ARM_PISTON);
bool statusPistonPalette = false; // Piston state
bool statusPistonArm = false; // Piston state