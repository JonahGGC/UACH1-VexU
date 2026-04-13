#include "main.h"
#include "lemlib/api.hpp"


// Define Drivetrain motors
#define RIGHT_BACK  6
#define RIGHT_MID   5
#define RIGHT_FRONT 4


#define LEFT_BACK  3
#define LEFT_MID   2
#define LEFT_FRONT 1


// Define subsystems motors
#define INTAKER   7


#define TRANSPORT_BOTTOM   -8
#define TRANSPORT_MID     18
#define TRANSPORT_TOP   -19


#define OUTTAKER  -20


// Motor Groups
pros::MotorGroup right_motors ({RIGHT_BACK, RIGHT_MID, RIGHT_FRONT}, pros::MotorGearset::blue);
pros::MotorGroup left_motors ({-LEFT_BACK, -LEFT_MID, -LEFT_FRONT}, pros::MotorGearset::blue);


// Subsystems
pros::MotorGroup intaker ({INTAKER}, pros::MotorGearset::green);
pros::MotorGroup transport ({TRANSPORT_BOTTOM, TRANSPORT_MID}, pros::MotorGearset::green);
pros::MotorGroup transportTop ({TRANSPORT_TOP}, pros::MotorGearset::blue );
pros::MotorGroup outtaker ({OUTTAKER}, pros::MotorGearset::green);


// Inertial sensor
pros::Imu inertial_sensor(10);


// Rotation sensor
pros::Rotation vertical_rotation(-9);


// Distance sensor
pros::Distance distance_sensor(17);


// Pneumatics
pros::ADIDigitalOut piston_ele('A'); 
bool status_ele=false;
pros::ADIDigitalOut piston_palette('B');
bool status_palette=false;
pros::ADIDigitalOut piston_hook('H'); 
bool status_hook=false;   


// Master Control
pros::Controller master (pros::E_CONTROLLER_MASTER);


// Encoder wheel
lemlib::TrackingWheel vertical_wheel(&vertical_rotation, 1.9695, 0); //antes -0.5


// Drivetrain measures
lemlib::Drivetrain drivetrain(
    &left_motors,  // left motors
    &right_motors, // right motors
    12.5,          // TRACK WIDTH
    3,          // wheel diameter
    360,        // RPM
    2              // Horizontal drift
);


// Odometry sensors
lemlib::OdomSensors sensors(
    &vertical_wheel, // Reference to vertical rotary sensor
    nullptr,         // Vertical tracking wheel 2
    nullptr,         // Horizontal tracking wheel 1
    nullptr,         // Horizontal tracking wheel 2
    &inertial_sensor // Inercial
);


// Control curves
lemlib::ExpoDriveCurve throttle_curve(
    5,  // "Drift" of the joysticks [127]
    7,  //  minimum output where drivetrain will move out of 127
    1.019 //  expo curve gain
);


lemlib::ExpoDriveCurve steer_curve(
    7,  //* joystick deadband out of 127
    10,   //* minimum output where drivetrain will move out of 127
    1.019 //* expo curve gain
);


// PIDs
lemlib::ControllerSettings lateral_controller(
    8,    // kP
    0.1,  // kI
    22,   // kD
    3,    // windup
    0.5,  // smallError
    200,  // smallTimeout
    3,    // largeError
    500,  // largeTimeout
    0     // slew
);


lemlib::ControllerSettings angular_controller(
    7.7,  // kP
    0.1,   // kI
    21.5,    // kD
    3,     // windup
    0.5,   // smallError
    300,   // smallTimeout
    3,     // largeError
    500,   // largeTimeout
    2      // slew
);


// Create chassis object
lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors, &throttle_curve, &steer_curve);


// -------------------------------------------------------------------------
// INIT FUCTIONS
// -------------------------------------------------------------------------


void screen_task(){
    while (true) {
        master.print(0,0,"%.2f %.2f",vertical_wheel.getDistanceTraveled(), inertial_sensor.get_rotation());

        pros::delay(80);
    }
}


void initialize() {
    pros::lcd::initialize();
   
    pros::lcd::set_text(1, "Calibrating... DON'T TOUCH");
    chassis.calibrate(); // IMU Calibration
    pros::delay(500); // Time to let IMU calibrate
    pros::lcd::set_text(1, "System Ok. IMU OK.");
    piston_ele.set_value(false);
    piston_palette.set_value(false);
    piston_hook.set_value(false);
    pros::lcd::set_text(1, "System Ok. IMU OK.");

    pros::Task Movement_thread(screen_task);
}


// -------------------------------------------------------------------------
// AUTONOMOUS
// -------------------------------------------------------------------------


void autonomous_match() { //Match

    //Get to the center:
    chassis.setPose(0, 0, 0);
    pros::delay(100);
    chassis.moveToPoint(0, 32, 3000, {.forwards = true, .maxSpeed = 70}, false);
    chassis.turnToHeading(-90,5000, {.maxSpeed=127}, false);

    //Block in the center:
    chassis.setPose(0,0,0);
    pros::delay(250);
    chassis.moveToPoint(0,6,3000, {.forwards=true, .maxSpeed=80},false);
    transport.move(-127);
    intaker.move(-127);
    pros::delay(2000);
    transport.move(0);
    intaker.move(0);

    //Get to the loader:
    chassis.moveToPoint(0,-50,3000, {.forwards=false, .maxSpeed=70},false);
    chassis.turnToHeading(-65,3000, {}, false);
    pros::delay(250);
    chassis.turnToHeading(-135,3000, {}, false);
    pros::delay(250);

    //Empty the loader:
    chassis.setPose(0,0,0);
    pros::delay(250);
    piston_ele.set_value(true);
    pros::delay(250);
    piston_palette.set_value(true);
    pros::delay(250);
    chassis.moveToPoint(0,15,3000, {.forwards=true, .maxSpeed=70},true);
    transportTop.move(127);
    transport.move(127);
    intaker.move(127);
    pros::delay(6000);
    transportTop.move(0);
    transport.move(0);
    intaker.move(0);
    pros::delay(250);
    piston_palette.set_value(false);
    pros::delay(250);
    chassis.moveToPoint(0,0,3000, {.forwards=false, .maxSpeed=70},true);
    pros::delay(250);
    chassis.turnToHeading(-45,3000, {}, false);
    pros::delay(150);
    transportTop.move(-127);
    transport.move(-127);
    intaker.move(-127);
    pros::delay(1500);
    transportTop.move(0);
    transport.move(0);
    intaker.move(0);
    chassis.turnToHeading(0,3000, {}, false);
    pros::delay(250);
    piston_palette.set_value(true);
    pros::delay(250);
    chassis.moveToPoint(0,14,3000, {.forwards=true, .maxSpeed=70},true);
    transportTop.move(127);
    transport.move(127);
    intaker.move(127);
    pros::delay(6000);
    transportTop.move(0);
    transport.move(0);
    intaker.move(0);

    //Fill the goal:
    chassis.moveToPoint(0,0,3000, {.forwards=false, .maxSpeed=70},true);
    pros::delay(250);
    piston_palette.set_value(false);
    pros::delay(250);
    chassis.turnToHeading(-90,3000,{},false);
    pros::delay(250);
    chassis.moveToPoint(1.5,0,3000, {.forwards=true, .maxSpeed=70},true);
    pros::delay(250);
    chassis.turnToHeading(180,3000,{},false);
    pros::delay(250);
    chassis.moveToPoint(0,-24,3000, {.forwards=true, .maxSpeed=60},false);
    pros::delay(350);
    transportTop.move(127);
    transport.move(127);
    outtaker.move(127);
    pros::delay(5000);
    transportTop.move(0);
    transport.move(0);
    outtaker.move(0);
    status_ele=true;

}


void autonomous() {
    chassis.setPose(0, 0, 0);
    pros::delay(1000);
    chassis.moveToPoint(0, 24, 3000, {.forwards = false, .maxSpeed = 100},  false);
}


void autonomous_giro() {
    chassis.setPose(0, 0, 0);
    pros::delay(1000);
    chassis.turnToHeading(180, 2000, {}, false);
    pros::delay(1000);
    chassis.turnToHeading(0, 2000, {}, false);
}



// -------------------------------------------------------------------------
// CONTROLLER
// -------------------------------------------------------------------------


void Task_Mov(){
    while (true) {
        // get joystick positions
        int leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the chassis with curvature drive
        chassis.arcade(leftY, rightX);
        // delay to save resources
        pros::delay(10);
    }
}


void Task_Ent_Uriel(){

    while (true) {
        if(master.get_digital(DIGITAL_R1)) {    
            intaker.move(-127);
            transport.move(-127);
            transportTop.move(-127);
        } else if(master.get_digital(DIGITAL_R2)) {
            intaker.move(127);
            transport.move(127);
            transportTop.move(127);
        } else {                                
            if ( !(master.get_digital(DIGITAL_L1) || master.get_digital(DIGITAL_L2)) )
            {
                transport.move(0);
                transportTop.move(0);
            }
            intaker.move(0);
           
        }


        if(master.get_digital(DIGITAL_L2)) {    
            transport.move(127);
            transportTop.move(127);
            outtaker.move(127);
        } else if(master.get_digital(DIGITAL_L1)) {
            transport.move(-127);
            transportTop.move(-127);
            outtaker.move(-127);
        } else {                
            if ( !(master.get_digital(DIGITAL_R1) || master.get_digital(DIGITAL_R2)) )
            {
                transport.move(0);
                transportTop.move(0);
            }                
            outtaker.move(0);
        }  

        if(master.get_digital_new_press(DIGITAL_X)) {  
            piston_ele.set_value(true);
            status_ele=false;
            pros::delay(20); 
            status_palette=!status_palette; 
            piston_palette.set_value(status_palette);
        }

        if(master.get_digital_new_press(DIGITAL_Y)){
            status_hook=!status_hook;
            piston_hook.set_value(status_hook);
        }
       
        if(master.get_digital_new_press(DIGITAL_RIGHT)) {
            status_ele=!status_ele;
            piston_ele.set_value(status_ele);
        }

        pros::delay(20);                        
    }
}


void opcontrol() {
    pros::Task Movement_thread(Task_Mov);      
    pros::Task Entry_thread(Task_Ent_Uriel);      
    while(true){
        pros::delay(20);
    }
}