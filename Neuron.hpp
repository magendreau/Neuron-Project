#ifndef NEURON_HPP
#define NEURON_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <array>

constexpr double taurp(2.0); //constant of time of the repository period
constexpr double tau(20.0); //constant of time 
constexpr double C(1.0); //capacity, connections received by each neuron
constexpr double R(tau/C); //input resistance
constexpr double h(0.1); //increment of time
constexpr double theta(20.0); //thresold limit

constexpr double t_stop(500.0); //number of steps of simulation, as h=0.1, the time of simulation is here 500 ms (t=n*h)
constexpr unsigned int n_stop(t_stop/h); //Maximal number of steps of the simulation based on time
constexpr int D(15); //delay before the spike is treated by the neuron
constexpr double J_excitatory(0.1); //amplitude of the spike, equal for all synapses
constexpr double J_inhibitory(0.5); //amplitude of the spike, equal for all synapses

constexpr double c1(exp(-h/tau));
constexpr double c2(R*(1-exp(-h/tau)));

constexpr unsigned int totalN(12500);
constexpr unsigned int excitatoryNeurons(10000);
constexpr unsigned int inhibitoryNeurons(2500);
constexpr unsigned int excitatoryConnections(1000);
constexpr unsigned int inhibitoryConnections(250);
constexpr double thresoldFrequency(theta/excitatoryConnections*J_excitatory*tau);
constexpr double externalFrequency(2.0*thresoldFrequency);

enum State {REFRACTORY, NON_REFRACTORY}; // states in which the neuron can be

class Neuron {
	
	public:
	Neuron( double potential = 0.0, unsigned int spike = 0, 
			double t = 0.0, State st = NON_REFRACTORY, 
			std::vector<double> buffer = std::vector<double>((D+1), 0.0),
			double time = 0.0);
			//we initialize the ring buffer at a size that matches the delay
	
	double getMembranePotential() const;
	unsigned int getSpikes() const;
	double getSpikesOccured() const;
	State getState() const;
	double getClock() const;
	
	void setMembranePotential(double potential);
	void setSpikes(unsigned int spk);
	void setSpikesOccured(double t);
	void setState(State st);
	void setClock(double time);
	
	void updateState(double t); //updates the state of the neuron
	void update(unsigned int step, double I, bool recep); 
	void fillRingBuffer(unsigned int step);
	double newMembranePotential(double I, double J); //recalculates the membrane potential at time t+h
	
	~Neuron();
	
	private:
	double membranePotential; 
	unsigned int spikes; //number of spikes
	double spikesOccured; //time when the spikes occured
	State state;
	std::vector<double> ringBuffer; //buffer to keep in memory the spikes before the reaction of postsynaptic neuron
	double clock;

};

#endif
