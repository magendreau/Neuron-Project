#ifndef NEURON_HPP
#define NEURON_HPP

#include <iostream>
#include <fstream>

constexpr double taurp(2.0); //constant of time of the repository period
constexpr double tau(20.0); //constant of time 
constexpr double C(1.0); //capacity, connections received by each neuron
constexpr double R(tau/C); //resistance
constexpr double h(0.5);
constexpr double theta(20.0); //thresold limit

constexpr double I_ext(1.01);
constexpr double Vr(10); //reset membrane potential
constexpr double t_stop(500);


enum State {REFRACTORY, NON_REFRACTORY}; // states in which the neuron can be

class Neuron {
	
	public:
	Neuron(double potential = 10, double spike = 0.0, double t = 0.0, State st = REFRACTORY);
	
	double getMembranePotential() const;
	double getSpikes() const;
	double getSpikesOccured() const;
	State getState() const;
	
	void setMembranePotential(double potential);
	void setSpikes(double spk);
	void setSpikesOccured(double t);
	void setState(State st);
	
	void updateState(double t); //updates the state of the neuron
	void update(double t, double a, double b, std::ofstream& out);
	double newMembranePotential(double h, double I); //recalculates the menbrane potential at time t+h
	
	~Neuron();
	
	private:
	double membranePotential; 
	double spikes; //number of spikes
	double spikesOccured; //time when the spikes occured
	State state;
	 
};

#endif
