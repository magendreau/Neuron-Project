#ifndef NEURON_HPP
#define NEURON_HPP

#include <iostream>
#include <vector>
#include <fstream>

constexpr double taurp(2.0); //constant of time of the repository period
constexpr double tau(20.0); //constant of time 
constexpr double C(1.0); //capacity, connections received by each neuron
constexpr double R(tau/C); //resistance

enum State {REFRACTORY, NON_REFRACTORY}; // states in which the neuron can be

class Neuron {
	
	public:
	Neuron(double potential = 10, double spike = 0.0, std::vector<double> t = {0.0}, State st = REFRACTORY);
	
	double getMembranePotential() const;
	double getSpikes() const;
	std::vector<double> getSpikesOccured() const;
	State getState() const;
	
	void setMembranePotential(double potential);
	void setSpikes(double spk);
	void setSpikesOccured(std::vector<double> t);
	void setState(State st);
	
	void updateState(double t, size_t i); //updates the state of the neuron
	void storeSpikesTime(double t); //store in spikesOccured the times at which the spikes occured
	double newMembranePotential(double h, double I); //recalculates the menbrane potential at time t+h
	
	void show(std::vector<double> vec, std::ofstream& out); //shows what a vector contains
	
	~Neuron();
	
	private:
	double membranePotential; 
	double spikes; //number of spikes
	std::vector<double> spikesOccured; //time when the spikes occured
	State state;
	 
};

#endif
