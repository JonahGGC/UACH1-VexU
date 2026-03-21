#pragma once
#include "main.h"
#include "lemlib/api.hpp"
#include "robot_config.hpp"  // needs motors & sensors

extern lemlib::TrackingWheel vertical_wheel;

extern lemlib::Drivetrain    drivetrain;
extern lemlib::OdomSensors   sensors;

extern lemlib::ExpoDriveCurve throttle_curve;
extern lemlib::ExpoDriveCurve steer_curve;

extern lemlib::ControllerSettings lateral_controller;
extern lemlib::ControllerSettings angular_controller;

extern lemlib::Chassis chassis;