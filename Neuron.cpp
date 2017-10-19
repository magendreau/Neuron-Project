#include "Neuron.hpp"
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

Neuron::Neuron(double potential, unsigned int spike, double t, State st, vector<double> buffer, double time)
	:membranePotential(potential), spikes(spike), spikesOccured(t), state(st), ringBuffer(buffer), clock(time)
{}
	
double Neuron::getMembranePotential() const
{ 
	return membranePotential;
}

unsigned int Neuron::getSpikes() const
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

double Neuron::getClock() const
{
	return clock;
}

void Neuron::setMembranePotential(double potential)
{
	membranePotential = potential;
}

void Neuron::setSpikes(unsigned int spk)
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

void Neuron::setClock(double time)
{
	clock = time;
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

void Neuron::update(unsigned int step, double I, bool recep)
{	
		updateState(step*h);
		
		if(state == REFRACTORY) {
			membranePotential = 0.0; // when the neuron is refractory it's membrane potential is 0
			
		} else if((membranePotential >= theta)) { //if the membrane potential reaches the thresold
			if(!recep) {
				spikesOccured = step*h; //a spike is emitted
				cout << "A spike occured at t=" << spikesOccured << " ms" << endl;
				++spikes;
			}
				membranePotential = 0.0;
				state = REFRACTORY;
		} else {
			if(recep) { I = 0.0; }
			const unsigned int t = step%(D+1);
			membranePotential = newMembranePotential(I, ringBuffer[t]); //membrane potential of neuron changes after reception
			ringBuffer[t] = 0;
	}
	++clock;
}

void Neuron::fillRingBuffer(unsigned int step)
{
	const unsigned int readOut = (step+D)%(D+1);
	assert((readOut) <= ringBuffer.size()); 
	ringBuffer[readOut] += J;
}

double Neuron::newMembranePotential(double I, double J)
{
	return (membranePotential = c1*membranePotential + I*c2 + J);
}

void Neuron::testDifferencePotential(vector<double> V)
{	
	if(abs(V[1]-V[0]) <= pow(10,-2)) { //if the difference between the two values and J is low enough
		cout << "The difference between the two potential is J" << endl;
	} else {
		cerr << "The difference is not J" << endl;
	}
}

void Neuron::testDelay(vector<double> T, unsigned int ix)
{
	if(abs(T[ix]-T[ix-D]) <= pow(10,-3)) { //if the difference between the two values and D is low enough
		cout << "The time between the reception and the response is D" << endl;
	} else {
		cerr << "The difference is not D" << endl;
	}
}

Neuron::~Neuron()
{}
