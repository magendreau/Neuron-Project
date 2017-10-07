#include "Neuron.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;

Neuron::Neuron(double potential, double spike, vector<double> t, State st)
	:membranePotential(potential), spikes(spike), spikesOccured(t), state(st)
{}
	
double Neuron::getMembranePotential() const
{ 
	return membranePotential;
}

double Neuron::getSpikes() const
{
	return spikes;
}

vector<double> Neuron::getSpikesOccured() const
{
	return spikesOccured;
}

State Neuron::getState() const
{ 
	return state;
}
	
void Neuron::setMembranePotential(double potential)
{
	membranePotential = potential;
}

void Neuron::setSpikes(double spk)
{
	spikes = spk;
}

void Neuron::setSpikesOccured(vector<double> t)
{
	spikesOccured = t;
}

void Neuron::setState(State st)
{ 
	state = st;
}	
void Neuron::updateState(double t, size_t i)
{
	if((spikesOccured[i] <= t) and (t <= (spikesOccured[i]+taurp))) {
		//if the time is during the refractory period meaning if the tim is after the previous spike time 
		//but below the end of the refractory period
		state = REFRACTORY;
	} else {
		state = NON_REFRACTORY;
	}
}

void Neuron::storeSpikesTime(double t)
{
	// add to the times when spikes occured a new one
	spikesOccured.push_back(t);
}

double Neuron::newMembranePotential(double h, double I)
{
	membranePotential = exp(-h/tau)*membranePotential + I*R*(1-exp(-h/tau));
	return membranePotential;
}
	
void Neuron::show(vector<double> vec, ofstream& out)
{
	for(size_t i(0) ; i < vec.size() ; ++i) {
		out << vec[i] << "  " ;
	}
}

Neuron::~Neuron()
{}
