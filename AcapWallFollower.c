#pragma config(Sensor, S1,     eyesIR,         sensorEV3_IRSensor)
#pragma config(Sensor, S2,     eyes,           sensorEV3_Ultrasonic)
#pragma config(Sensor, S3,     gyro,           sensorEV3_Gyro)
#pragma config(Motor,  motorA,          left,          tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorB,          head,          tmotorEV3_Medium, PIDControl, encoder)
#pragma config(Motor,  motorC,           ,             tmotorEV3_Medium, openLoop)
#pragma config(Motor,  motorD,          right,         tmotorEV3_Large, PIDControl, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

float GAP = 17;
//KD should be bigger than KP
float KP = 3;
float KD = 0.000018;
float KI = 0.000018;
float MINSPEED = 5;
float MAXSPEED = 30;
float SPEED = 10;

void depan(int masa){
		setMotor(left, 10);
		setMotor(right, 10);
		wait1Msec(masa);
		stopAllMotors();
}

void PIDFollowWall(){
	float error = 0;
	float integral = 0;
	float derivative = 0;
	float lastError;
	float turn = 0;
	float leftSpeed;
	float rightSpeed;
	float offset;

	wait10Msec(200);
  playSound(soundBlip);
  offset = getUSDistance(eyes);

	while(true){
		error = getUSDistance(eyes) - offset;
		integral = integral + error;
		derivative = error - lastError;
		turn = (KP*error)+(KI*integral)+(KD*derivative);

		leftSpeed = SPEED - turn;
		//if (leftSpeed < MINSPEED)
		//	leftSpeed = MINSPEED;
		if (leftSpeed > MAXSPEED)
			leftSpeed = MAXSPEED;

		rightSpeed = SPEED + turn;
		//if (rightSpeed < MINSPEED)
		//	rightSpeed = MINSPEED;
		if (rightSpeed > MAXSPEED)
			rightSpeed = MAXSPEED;

		setMotorSpeed(left, leftSpeed);
		setMotorSpeed(right, rightSpeed);
		lastError = error;
			//int lastError = 0;
			//int position = getUSDistance(eyes);
			//int error = position - GAP;

			//if(error < (3 * GAP)){
			//	int speed = (KP * error) + (KD * (error - lastError));
			//	lastError = error;

			//	int rightSpeed = RS + speed;
			//	int leftSpeed = LS - speed;

			//	if (rightSpeed > RM) rightSpeed = RM;
			//	if (leftSpeed > LM) leftSpeed = LM;
			//	if (rightSpeed < 0) rightSpeed = 0;
			//	if (leftSpeed < 0) leftSpeed = 0;
			//	setMotor(left, leftSpeed);
			//	setMotor(right, rightSpeed);
			//}
			//else{
			//	stopAllMotors();
			//	break;
			//}
	}
}

void followWall(bool flag){
		if(flag){
				clearTimer(T1);
				clearTimer(T2);
				flag = false;
		}

		int leftSpeed = 10;
		int rightSpeed = 10;

		//too far, at node
		if ((getIRDistance(eyes)-GAP) > GAP){
				//assign node
				stopAllMotors();
				depan(1000);
				resetGyro(gyro);
				setMotor(right, 3);
				waitUntil(getGyroDegrees(gyro) < -87);
				stopMotor(right);
				depan(1000);
		}
		//right drift
		else if((getIRDistance(eyes)-GAP) > 5){
				setMotor(right, rightSpeed + 1 + getTimer(T1, seconds));
				setMotor(left, leftSpeed - 1 - getTimer(T1, seconds));
		}
		//left drift
		else if((getIRDistance(eyes)-GAP) < -5){
				setMotor(left, leftSpeed + 1 + getTimer(T2, seconds));
				setMotor(right, rightSpeed - 1 - getTimer(T2, seconds));
		}
		//straight
		else{
				clearTimer(T1);
				clearTimer(T2);
				setMotor(left, leftSpeed);
				setMotor(right, rightSpeed);
		}
}

task main()
{
	PIDFollowWall();
}
