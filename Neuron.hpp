#ifndef NEURON_HPP
#define NEURON_HPP

#include <iostream>
#include <fstream>
#include <vector>

constexpr double taurp(2.0); //constant of time of the repository period
constexpr double tau(20.0); //constant of time 
constexpr double C(1.0); //capacity, connections received by each neuron
constexpr double R(tau/C); //input resistance
constexpr double h(0.1); //increment of time
constexpr double theta(20.0); //thresold limit

constexpr double Vr(10.0); //reset membrane potential
constexpr double t_stop(500.0); //number of steps of simulation, as h=0.1, the time of simulation is here 500 ms (t=n*h)
constexpr unsigned int n_stop(t_stop/h); //Maximal number of steps of the simulation based on time
constexpr double D(1.5); //delay before the spike is treated by the neuron
constexpr double J(2.0); //amplitude of the spike, equal for all synapses

enum State {REFRACTORY, NON_REFRACTORY}; // states in which the neuron can be

class Neuron {
	
	public:
	Neuron( double potential = 10, double spike = 0.0, 
			double t = 0.0, State st = REFRACTORY, 
			std::vector<unsigned int> buffer = std::vector<unsigned int>((D/h)));
			//we initialize the ring buffer at a size that matches the delay
	
	double getMembranePotential() const;
	double getSpikes() const;
	double getSpikesOccured() const;
	State getState() const;
	
	void setMembranePotential(double potential);
	void setSpikes(double spk);
	void setSpikesOccured(double t);
	void setState(State st);
	
	void updateState(double t); //updates the state of the neuron
	void update(double step, double I); 
	void fillRingBuffer(double nbSpikes, unsigned int readOut, unsigned int step);
	bool fullBuffer(unsigned int origin); //if the ring buffer is full, it returns true
	void receive(double step, double J, double I); //reception of spike by postsynaptic neuron
	double newMembranePotential(double I); //recalculates the membrane potential at time t+h
	
	void testDifferencePotential(std::vector<double> V, size_t ix); //verifies that the membrane potential increases of J each time
	void testDelay(std::vector<double> T, size_t ix);
	
	~Neuron();
	
	private:
	double membranePotential; 
	double spikes; //number of spikes
	double spikesOccured; //time when the spikes occured
	State state;
	std::vector<unsigned int> ringBuffer; //buffer to keep in memory the spikes before the reaction of postsynaptic neuron
};

#endif
