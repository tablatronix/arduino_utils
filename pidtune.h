#ifndef pidtune_d
#define pidtune_d

#include <pidautotuner.h>
#include <ssr.h>

float targetInputValue = 40.00;
long loopInterval = 100; // pid loop ms

float doSomethingToGetInput(){
	// updateTemps();
	printTC();
	return currentTempAvg;
}

float doSomethingToSetOutput(double output){
	SetSSRFrequency(output);
	Serial.println((String)output);
}

void init_pidtune(){
	Serial.print("[PIDTUNE] ");
	Serial.println(targetInputValue,2);
    PIDAutotuner tuner = PIDAutotuner();
	tuner.setTuningCycles(5);

    // Set the target value to tune to
    // This will depend on what you are tuning. This should be set to a value within
    // the usual range of the setpoint. For low-inertia systems, values at the lower
    // end of this range usually give better results. For anything else, start with a
    // value at the middle of the range.
    tuner.setTargetInputValue(targetInputValue);

    // Set the loop interval in microseconds
    // This must be the same as the interval the PID control loop will run at
    tuner.setLoopInterval(loopInterval*100);

    // Set the output range
    // These are the maximum and minimum possible output values of whatever you are
    // using to control the system (analogWrite is 0-255)
    tuner.setOutputRange(0, 255);

    // Set the Ziegler-Nichols tuning mode
    // Set it to either PIDAutotuner::ZNModeBasicPID, PIDAutotuner::ZNModeLessOvershoot,
    // or PIDAutotuner::ZNModeNoOvershoot. Test with ZNModeBasicPID first, but if there
    // is too much overshoot you can try the others.
    tuner.setZNMode(PIDAutotuner::ZNModeBasicPID);
    // tuner.setZNMode(PIDAutotuner::ZNModeLessOvershoot);
    // tuner.setZNMode(PIDAutotuner::ZNModeNoOvershoot);

    // This must be called immediately before the tuning loop
    tuner.startTuningLoop();

    // Run a loop until tuner.isFinished() returns true
    long microseconds;
    while (!tuner.isFinished()) {

        // This loop must run at the same speed as the PID control loop being tuned
        long prevMicroseconds = microseconds;
        microseconds = micros();

        // Get input value here (temperature, encoder position, velocity, etc)
        float input = doSomethingToGetInput();

        // Call tunePID() with the input value
        float output = tuner.tunePID(input);

        // Set the output - tunePid() will return values within the range configured
        // by setOutputRange(). Don't change the value or the tuning results will be
        // incorrect.
        doSomethingToSetOutput(output);

        // This loop must run at the same speed as the PID control loop being tuned
        while (micros() - microseconds < loopInterval){
        	delayMicroseconds(1);
        	delay(0);
        }

        delay(0);
    }

    // Turn the output off here.
    doSomethingToSetOutput(0);

    // Get PID gains - set your PID controller's gains to these
    float kp = tuner.getKp();
    float ki = tuner.getKi();
    float kd = tuner.getKd();
    Serial.println("[PIDTUNE] COMPLETE");
    Serial.println(kp,4);
    Serial.println(ki,4);
    Serial.println(kd,4);    
    Serial.println(kp,3);
    Serial.println(ki,3);
    Serial.println(kd,3);
    Serial.println(kp,BIN);
    Serial.println(ki,BIN);
    Serial.println(kd,BIN);
}


#endif
