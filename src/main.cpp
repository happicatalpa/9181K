#include "main.h"



/** DEFINE PORTS */
#define LEFT_MG_PORTS {1, -2, 3}
#define RIGHT_MG_PORTS {-4, 5, -6}
#define INTAKE_PORT 7
#define ARM_PORT 8
#define MOGO_CLAMP_PORT 'A'

/** GLOBAL VARIABLES */


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");




}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */




void opcontrol() {

	// initialization
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup left_mg(LEFT_MG_PORTS);    // Creates a motor group with forwards ports 1 & 3 and reversed port 2
	pros::MotorGroup right_mg(RIGHT_MG_PORTS);  // Creates a motor group with forwards port 5 and reversed ports 4 & 6
	pros::Motor intake(INTAKE_PORT, pros::v5::MotorGears::blue); // Initializes motor for intake at forward port 7
	pros::Motor arm (ARM_PORT, pros::v5::MotorGears::red); // Initializes motor for arm
	pros::ADIDigitalOut clamp (MOGO_CLAMP_PORT); // initalizes digital port for mobile clamp

	// State Variables
	bool clamping = false; // records the state of the piston used for mogo clamp

	while (true) {

		// Arcade control scheme
		int dir = master.get_analog(ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		int turn = master.get_analog(ANALOG_RIGHT_X);  // Gets the turn left/right from right joystick
		left_mg.move(dir + turn);                      // Sets left motor voltage
		right_mg.move(dir - turn);                     // Sets right motor voltage

		// Intake control
		if (master.get_digital(DIGITAL_R2)) {
			intake.move(100); // intake
		}
		else if (master.get_digital(DIGITAL_L2)){
			intake.move(-100); // outtake
		}
		else {
			intake.move(0);
		}

		// Mobile Goal Clamp Control
		if (master.get_digital(DIGITAL_R1)) {
			//switch the state of mogo clamp
			clamping = !clamping;
			clamp.set_value(clamping);
		}

		// Arm Control
		if (master.get_digital(DIGITAL_L1)) {
			arm.move_velocity(100);
		}
		else if (arm.get_position() > 0) {
			arm.move_velocity(-100);
		}
		else {
			arm.move_velocity(0);
		}

		pros::delay(20);                               // Run for 20 ms then update
	}
}