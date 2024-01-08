// sense env class

// create sensor instance

// sensor add
// max num samples
// ring buffer
// ring index
// sensor id
// sensor index
// sensor mqtt topic (temp/temp2/humidity)
// sensor i2c addr
// sensor unit
// sensor values or sensor group by type

// sensor calc
// avg
// mean
// mode
// running avg
// khalman filter
// validation/rejection/clamping

// timestamp or period reconstructor

// sensor alarms/threshholds

// offline mode/collection/logging to sd, maybe add a second buffer with long term ogging interval

// interrupt for sense_env sensor

// depencandies
// average
// statsistics
// filter

// ability to grab a sample window for deferred submission to mqtt for a series ( useful for battery operated devices or offline )
// ability to grab lastvalue as running average over subset of last n samples/n times
// ability to reject oulier values for known bad sensor values lacking error conditions

// get sensors by group/id for looping to displays etc

#ifndef sensorsdlog_h
#define sensorsdlog_h

// #include <iostream>
#include <vector>
// using namespace std;

#include <Average.h>
// Average<float> avg_a(20);

// #include <Statistic.h>
// Statistic myStats;

// vector or list of sensor objects
// group by type for sensors with many metrics
// addSensor(char* id, char* name, char* unit, T datatype);
// removeSensor(char* id);
// getSensorByID(char* id);
// vector<Statistic> sensors; // how to use a templated class here?

// collections of sensors
// T datatype
template <class T>
class sensorcollection {
    private:
		std::vector<Average<T>> sensors;
		// sensors.resize()
		uint32_t _numsamples = 20;

	public:
		sensorcollection();
		~sensorcollection();
		void begin();
		void addSensor(int idx, uint8_t size);
		void addSensorValue(int idx, T value);
		float getSensorValue(int idx);

};

	template <class T> 
	sensorcollection<T>::sensorcollection(){
		Serial.println("sensorcollection constructor");
		// testing in constructor for now
		// BUT how to create many new instances and retain them in class?
		// Average<T> avg_x(_numsamples); // scope this in class?, surely this will go bye bye
	}

	template <class T> 
	sensorcollection<T>::~sensorcollection(){
		Serial.println("sensorcollection de-constructor");
		sensors.clear();
	}

	template <class T> 
	void sensorcollection<T>::begin(){
		sensors.push_back(Average<T>(_numsamples)); // use temp obj or smart pointers?
	}

	template <class T> 
	void sensorcollection<T>::addSensor(int idx, uint8_t size){
		Average<T> *avg_x = new Average<T>(size);
		sensors.push_back(*avg_x); // use temp obj or smart pointers?
		Serial.println("added sensor: " + (String)idx);
	}

	template <class T> 
	void sensorcollection<T>::addSensorValue(int idx, T value){
		sensors[idx].push(value);
	}

	template <class T> 
	float sensorcollection<T>::getSensorValue(int idx){
		return sensors[idx].mean();
	}

// // individual sensor log, one per value
// // or find a way to create anonymous objects, but each one might have different units so might as well keep them seperate.
// template <typename T> 
// class sensordlog {
//     private:
                                        
//         const char* _id;        // unique id of collection                          
//         const char* _name;      // name
//         char* _unit;      // units of values eg. ug/L
//         T datatype;       // template of typename of sample, float, int etc
//         int _numsamples = 20;  // number of samples to store in buffer

//     public:
//     	sensordlog(); // add inializer list or use setters?
//     	~sensordlog();

// 		// setSensorRange(T min, T max);
// 		// setSensorNumSamples(int numSamples);

// 		// getValue(char* id); // self.value()
// 		// getAvg(); //self.avg()
// 		// getMean();
// 		// getMode();
// 		// getMax();
// 		// getMin();
		
// 		// extended
// 		// pass in pointer funcs for specific library methods
// 		// init()
// 		// update()
// };


// template <class T> sensordlog<T>::sensordlog(){
// 	Serial.println("constructor");
// 	// sensors.push_back(Statistic myStats1); // ? how to use an anoymous object pointer in vector?
// }

#endif