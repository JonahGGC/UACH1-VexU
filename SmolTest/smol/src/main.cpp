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
#define INTAKER   -7


#define TRANSPORT_BOTTOM   8
#define TRANSPORT_MID     -18
#define TRANSPORT_TOP   19


#define OUTTAKER  20


// Motor Groups
pros::MotorGroup right_motors ({RIGHT_BACK, RIGHT_MID, RIGHT_FRONT}, pros::MotorGearset::blue);
pros::MotorGroup left_motors ({-LEFT_BACK, -LEFT_MID, -LEFT_FRONT}, pros::MotorGearset::blue);


// Subsystems
pros::Motor intaker ({INTAKER}, pros::MotorGearset::green);
pros::MotorGroup transport ({TRANSPORT_BOTTOM, TRANSPORT_MID}, pros::MotorGearset::green);
pros::Motor transportTop ({TRANSPORT_TOP}, pros::MotorGearset::blue );
pros::Motor outtaker ({OUTTAKER}, pros::MotorGearset::green);


// Inertial sensor
pros::Imu inertial_sensor(10);


// Rotation sensor
pros::Rotation vertical_rotation(-9);


// Distance sensor
pros::Distance distance_sensor(17);
int wall_detection =5;
bool lastblock_detection=false;
bool block_detection=false;
int block_counter=0;


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
lemlib::TrackingWheel vertical_wheel(&vertical_rotation, 2.0056, 0); //antes -0.5


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
    8.75,    // kP
    0.1,  // kI
    23,   // kD
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


void robotReposition(int){
    int lastblock_counter=block_counter;
    int count=pros::millis();

}


void blockCounter(){
    while (true) { 
        if(distance_sensor.get() > wall_detection && block_detection==false){
            block_detection=true;
            if(intaker.get_voltage()>0){
                block_counter++;
            }
            else if(intaker.get_voltage()<0){
                block_counter--;
            }
        }
        else if (distance_sensor.get() <= wall_detection){
            block_detection=false;
        }
        pros::delay(10);
    }
}


// void dischargeBlocks(int block_counter,int time){
//     int firstTime=pros::millis();
//     while (true) { 
//         intaker.move(-127);
//         transport.move(-100);
//         transportTop.move(-127);
//         if(distance_sensor.get() < wall_detection && block_detection==false){
//             block_detection=true;
//             block_counter++;
//         }
//         else if (distance_sensor.get() >= wall_detection){
//             block_detection=false;
//         }
//         if(pros::millis()-firstTime>=time){
//             intaker.move(0);
//             transport.move(0);
//             transportTop.move(0); 
//             break;
//         }
//         pros::delay(10);
//     }
// }


// void emptyLoad(int block_counter,int time){
//     int firstTime=pros::millis();
//     while (true) { 
//         intaker.move(-127);
//         transport.move(-127);
//         transportTop.move(-127);       
//         if(pros::millis()-firstTime>=time){
//             intaker.move(0);
//             transport.move(0);
//             transportTop.move(0); 
//             break;
//         }
//         pros::delay(10);
//     }
// }


void screen_task(){
    while (true) {
        master.print(0,0,"%.2f",chassis.getPose().y);
        // master.print(0,0,"%.2f %.2f %.2f",chassis.getPose().y, inertial_sensor.get_rotation(), right_motors.get_temperature());
        // master.print(0,0,"%d, %d", block_counter, distance_sensor.get());
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
    pros::Task blockCounter_thread(blockCounter);
}


// -------------------------------------------------------------------------
// AUTONOMOUS
// -------------------------------------------------------------------------


void autonomous() { //Match

    //Go to the center:
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 31.5, 3000, {.forwards = true, .maxSpeed = 127}, false);
    chassis.turnToHeading(-47,5000, {.maxSpeed=127}, false);

    //Block in the center:
    chassis.setPose(0,0,0);
    transport.move(-127);
    intaker.move(-80);
    chassis.moveToPoint(0,6.5,1500, {.forwards=true, .maxSpeed=20},true);
    pros::delay(1000);
    transport.move(0);
    intaker.move(0);

    //Get to the loader:
    chassis.moveToPoint(0,-42,4000, {.forwards=false, .maxSpeed=127},false);
    chassis.turnToHeading(-134.5,3000, {.maxSpeed=127}, false);

    //Empty the loader:
    chassis.setPose(0,0,0);
    piston_ele.set_value(true);
    pros::delay(250);
    piston_palette.set_value(true);
    pros::delay(500);
    chassis.moveToPoint(0,14,3000, {.forwards=true, .maxSpeed=127},true);
    transportTop.move(127);
    transport.move(127);
    intaker.move(127);
    pros::delay(500);
    chassis.setPose(0,0,0);
    chassis.moveToPoint(0,3,3000, {.forwards=false, .maxSpeed=50},false);
    pros::delay(2000);

    //Discharge the blocks
    chassis.moveToPoint(0,-10,3000, {.forwards=false, .maxSpeed=127},false);
    transportTop.move(0);
    transport.move(0);
    intaker.move(0);
    piston_palette.set_value(false);
    pros::delay(100);
    chassis.turnToHeading(-45,3000, {.maxSpeed=127}, false);
    chassis.setPose(0,0,0);
    chassis.moveToPoint(0,3,3000, {.forwards=true, .maxSpeed=127},true);
    // dischargeBlocks(3, 1200);
    transportTop.move(-127);
    transport.move(-127);
    intaker.move(-127);
    pros::delay(1300);
    transportTop.move(0);
    transport.move(0);
    intaker.move(0);

    //Empty the second load
    chassis.turnToHeading(0.5,3000, {.maxSpeed=127}, false);
    piston_palette.set_value(true);
    pros::delay(250);
    chassis.moveToPoint(0,3,3000, {.forwards=true, .maxSpeed=127},true);
    transportTop.move(127);
    transport.move(127);
    intaker.move(127);
    pros::delay(2500);

    //Fill the goal:
    chassis.moveToPoint(0,-10,3000, {.forwards=false, .maxSpeed=127},false);
    transportTop.move(0);
    transport.move(0);
    intaker.move(0);
    piston_palette.set_value(false);
    pros::delay(250);
    chassis.turnToHeading(-179,3000,{.maxSpeed=127},false);
    chassis.setPose(0,0,0);
    chassis.moveToPoint(2,17,3000, {.forwards=true, .maxSpeed=127},false);
    chassis.moveToPoint(0,20,5000, {.forwards=true, .maxSpeed=50},true);
    transportTop.move(127);
    transport.move(127);
    outtaker.move(127);
    intaker.move(127);
    pros::delay(10000);
    intaker.move(0);
    transportTop.move(0);
    transport.move(0);
    outtaker.move(0);
    status_ele=true;

}


void autonomous_skills() {

    //Get to the first loader
    chassis.setPose(0, 0, 0);

    //Empty the first loader

    //Get to the goal

    //Fill the goal
    
    //Get to the second loader

    //Empty the second loader

    //Get to the goal

    //Fill the goal

    //Come back

    //To park



}


void autonomous_l() {
    chassis.setPose(0, 0, 0);
    pros::delay(1000);
    chassis.moveToPoint(0, 60, 3000, {.forwards = true, .maxSpeed = 127},  false);
}


void autonomous_gyro() {
    chassis.setPose(0, 0, 0);
    pros::delay(1000);
    chassis.turnToHeading(180, 2000, {}, false);
    pros::delay(1000);
    chassis.turnToHeading(0, 2000, {}, false);
}


void autonomous_x() {
    intaker.move(127);
    transport.move(127);
    transportTop.move(127);
    pros::delay(2000);
    intaker.move(-127);
    transport.move(-127);
    transportTop.move(-127);
    pros::delay(2000);
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
            intaker.move(127);
            transport.move(127);
            transportTop.move(127);
        } else if(master.get_digital(DIGITAL_R2)) {
            intaker.move(-80);
            transport.move(-127);
            transportTop.move(-127);
        } else {                                
            if ( !(master.get_digital(DIGITAL_L1) || master.get_digital(DIGITAL_L2)) )
            {
                transport.move(0);
                transportTop.move(0);
            }
            intaker.move(0);
           
        }


        if(master.get_digital(DIGITAL_L2)) {    
            transport.move(-127);
            transportTop.move(-127);
            outtaker.move(-127);
        } else if(master.get_digital(DIGITAL_L1)) {
            transport.move(127);
            transportTop.move(127);
            outtaker.move(127);
        } else {                
            if ( !(master.get_digital(DIGITAL_R1) || master.get_digital(DIGITAL_R2)) )
            {
                transport.move(0);
                transportTop.move(0);
            }                
            outtaker.move(0);
        }  

        if(master.get_digital_new_press(DIGITAL_X)) {
            if(status_ele==false){
                piston_ele.set_value(true);
                status_ele=true;
                pros::delay(60);
            }  
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