#pragma once
#include "main.h"

// ──────────────────────────────────────────
//  Controllers
// ──────────────────────────────────────────
extern pros::Controller master;

// ──────────────────────────────────────────
//  Drive Motors
// ──────────────────────────────────────────
extern pros::MotorGroup left_mg;
extern pros::MotorGroup right_mg;

// ──────────────────────────────────────────
//  Sensors
// ──────────────────────────────────────────
extern pros::Imu imu;
extern pros::Rotation vertical_rotation;
// extern pros::Rotation rotation_sensor;
// extern pros::Distance distance_sensor;
// extern pros::Optical optical_sensor;

// ──────────────────────────────────────────
//  Pheumatics
// ──────────────────────────────────────────
extern pros::ADIDigitalOut piston_rod;