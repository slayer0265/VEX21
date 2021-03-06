#include "main.h"

/**
* A callback function for LLEMU's center button.
*
* When this callback is fired, it will toggle line 2 of the LCD text between
* "I was pressed!" and nothing.
*/
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
* Runs initialization code. This occurs as soon as the program is started.
*
* All other competition modes are blocked by initialize; it is recommended
* to keep execution time for this mode under a few seconds.
*/
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
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
	//INITIALIZATION, MOVEMENT MOTORS
	pros::Motor front_L(10);
	pros::Motor front_R(4);
	pros::Motor back_L(8);
	pros::Motor back_R(3);
	//LIFT MOTORS
	pros::Motor Lift_R1(11);//can we combine 2 ports to make 2 motor functions instead of 4
	pros::Motor Lift_R2(2);
	pros::Motor Lift_L1(9);
	pros::Motor Lift_L2(15);
	//INTAKE
	pros::Motor Pacman_L1(20);
	pros::Motor Pacman_R1(12);

	pros::Controller main(pros::E_CONTROLLER_MASTER);
	//CONTROL PERIOD: MOVING/CONTROLLING
	while(true) {
		int left_y = main.get_analog(ANALOG_LEFT_Y); //LEFT ANALOG
		front_L.move( left_y);
		back_L.move( left_y);
		//WHY NOT USE JUST LEFT ANALOG FOR FORWARD AND BACKWARD BOTH SIDES?
		// THEN USE RIGHT ANALOG FOR TURNING AND SHIT
		int right_y = main.get_analog(ANALOG_RIGHT_Y);
		front_R.move( -right_y);
		back_R.move( -right_y);

		//LIFT MOVEMENT
		int left_L2 = 127*main.get_digital(DIGITAL_L2);
		int right_R2 = 127*main.get_digital(DIGITAL_R2);

		if (left_L2 > 0){
			Lift_L1.move(left_L2);
			Lift_L2.move(left_L2);
			Lift_R1.move(-left_L2);
			Lift_R2.move(-left_L2);
		}
		else if	(right_R2 > 0){
			Lift_L1.move(-right_R2);
			Lift_L2.move(-right_R2);
			Lift_R1.move(right_R2);
			Lift_R2.move(right_R2);
		}
		else {
			Lift_L1.move(0);
			Lift_L2.move(0);
			Lift_R1.move(0);
			Lift_R2.move(0);
		}


		//INTAKE
		int left_L1 = 127*main.get_digital(DIGITAL_L1);
		Pacman_L1.move(left_L1);
		Pacman_R1.move(-left_L1);

		int right_R1 = 127*main.get_digital(DIGITAL_R1);
		Pacman_L1.move(-left_L1);
		Pacman_R1.move(left_L1);
		pros::delay(2);
	}
}
