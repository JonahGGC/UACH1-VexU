#include "lemlib_config.hpp"

lemlib::TrackingWheel vertical_wheel(&vertical_rotation, 1.9775, 0);

lemlib::Drivetrain drivetrain(
    &left_mg,
    &right_mg,
    11.0,     // track width
    3.25,     // wheel diameter
    333.33,   // RPM
    2         // horizontal drift
);

lemlib::OdomSensors sensors(
    &vertical_wheel,
    nullptr,
    nullptr,
    nullptr,
    &imu
);

lemlib::ExpoDriveCurve throttle_curve(5, 7, 1.019);
lemlib::ExpoDriveCurve steer_curve(7, 10, 1.019);

lemlib::ControllerSettings lateral_controller(
    9.03, 0.1, 22,   // kP, kI, kD
    3,               // windup
    0.5, 200,        // smallError, smallTimeout
    3,   500,        // largeError, largeTimeout
    0                // slew
);

lemlib::ControllerSettings angular_controller(
    3.37, 0.1, 80,   // kP, kI, kD
    3,               // windup
    0.5, 300,        // smallError, smallTimeout
    3,   500,        // largeError, largeTimeout
    2                // slew
);

// chassis must be last — it depends on everything above
lemlib::Chassis chassis(
    drivetrain,
    lateral_controller,
    angular_controller,
    sensors,
    &throttle_curve,
    &steer_curve
);