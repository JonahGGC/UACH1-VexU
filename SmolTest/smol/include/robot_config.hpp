#pragma once
#include "main.h"

// ──────────────────────────────────────────
//  Controllers
// ──────────────────────────────────────────
extern pros::Controller master;

// ──────────────────────────────────────────
//  Drive Motors
// ──────────────────────────────────────────
extern pros::MotorGroup rightTrain;
extern pros::MotorGroup leftTrain;
extern pros::MotorGroup sBand;
extern pros::Motor inTaker;
extern pros::Motor indexUp;
extern pros::Motor indexDown;

// ──────────────────────────────────────────
//  Sensors
// ──────────────────────────────────────────
extern pros::IMU inertialSensor;
extern pros::Rotation verticalEncoder;
extern pros::Distance distanceSensor;

// ──────────────────────────────────────────
//  Pheumatics
// ──────────────────────────────────────────
extern pros::ADIDigitalOut pistonPalette;
extern pros::ADIDigitalOut pistonArm;
extern bool statusPistonPalette; // Piston state
extern bool statusPistonArm; // Piston state
extern bool L1Pressed; // State of L1 button
extern bool R1Pressed; // State of R1 button