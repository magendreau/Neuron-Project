#include "Neuron.hpp"
#include <iostream>
#include <cmath>

using namespace std;

Neuron::Neuron(double potential, double spike, double t, State st)
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

double Neuron::getSpikesOccured() const
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

void Neuron::setSpikesOccured(double t)
{
	spikesOccured = t;
}

void Neuron::setState(State st)
{ 
	state = st;
}	
void Neuron::updateState(double t)
{
	if((spikesOccured <= t) and (t <= (spikesOccured+taurp))) {
		//if the time is during the refractory period
		state = REFRACTORY;
	} else {
		state = NON_REFRACTORY;
	}
}

void Neuron::update(double t, double a, double b, ofstream& out)
{	
	double I(0.0);
	while(t < t_stop) {
		updateState(t);
		if((a <= t) and (t <= b)) {
			I = I_ext;
		} else { I = 0.0; }
		if(state == REFRACTORY) {
			setMembranePotential(0.0);
		} else if((membranePotential > Vr) and (state == NON_REFRACTORY)) {
			if((membranePotential >= theta) and (I_ext > 1.0)) {
				spikesOccured = t;
				out << "A spike occured at t=" << spikesOccured << " ms" << endl;
				spikes+=1;
			}
		}
		setMembranePotential(newMembranePotential(h,I));
		t = t+h;
	}
}

double Neuron::newMembranePotential(double h, double I)
{
	membranePotential = exp(-h/tau)*membranePotential + I*R*(1-exp(-h/tau));
	return membranePotential;
}

Neuron::~Neuron()
{}
