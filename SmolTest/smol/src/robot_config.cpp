#include "robot_config.hpp"

// ──────────────────────────────────────────
//  Controllers
// ──────────────────────────────────────────
pros::Controller master(pros::E_CONTROLLER_MASTER);

// ──────────────────────────────────────────
//  Drive Motors
//  Negative port  = reversed direction
// ──────────────────────────────────────────
pros::MotorGroup left_mg({-11, -12, -13});
pros::MotorGroup right_mg({1, 2, 3});

// ──────────────────────────────────────────
//  Sensors
// ──────────────────────────────────────────
pros::Imu imu(20);               // port 20
pros::Rotation vertical_rotation(-2); 
// pros::Distance distance_sensor(6);

// ──────────────────────────────────────────
//  Pheumatics
// ──────────────────────────────────────────
pros::ADIDigitalOut piston_rod('A');