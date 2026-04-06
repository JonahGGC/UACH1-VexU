#pragma once
#include "main.h"
#include "lemlib/api.hpp"
#include "robot_config.hpp"  // needs motors & sensors

extern lemlib::TrackingWheel trackingVertical;

extern lemlib::Drivetrain    drivetrain;
extern lemlib::OdomSensors   sensors;

extern lemlib::ExpoDriveCurve throttleCurve;
extern lemlib::ExpoDriveCurve steerCurve;

extern lemlib::ControllerSettings lateralController;
extern lemlib::ControllerSettings angularController;

extern lemlib::Chassis chassis;