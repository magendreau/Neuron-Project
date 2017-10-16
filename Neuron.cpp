#include "Neuron.hpp"
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

Neuron::Neuron(double potential, unsigned int spike, double t, State st, vector<unsigned int> buffer)
	:membranePotential(potential), spikes(spike), spikesOccured(t), state(st), ringBuffer(buffer)
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
				state = REFRACTORY;
			}
		} else {
			const int t = step%(D+1);
			membranePotential = newMembranePotential(I, ringBuffer[t]); //membrane potential of neuron changes after reception
			ringBuffer[t] = 0.0;
	}
}

void Neuron::fillRingBuffer(unsigned int step)
{
	const unsigned int t = (step+D)%(D+1);
	if (spikes > 0) {
		ringBuffer[t] += J;
		assert((t) <= ringBuffer.size()); 
	}
}

double Neuron::newMembranePotential(double I, double J)
{
	return (membranePotential = c1*membranePotential + I*c2 +J);
}

void Neuron::testDifferencePotential(vector<double> V, size_t ix)
{	
		if(abs(abs(V[ix]-V[ix-1]) - J) <= pow(10,-3)) { //if the difference between the two values and J is low enough
			cout << "The difference between the two potential is J" << endl;
		} else {
			cout << V[ix] << " and " << V[ix-1] << endl;
			cout << abs(abs(V[ix]-V[ix-1]) - J) << endl;
			cerr << "The difference is not J" << endl;
		}
}

void Neuron::testDelay(vector<double> T, size_t ix)
{
			if(abs(abs(T[ix]-T[ix-1]) - D) <= pow(10,-3)) { //if the difference between the two values and D is low enough
			cout << "The time between the reception and the response is D" << endl;
		} else {
			cout << abs(abs(T[ix]-T[ix-1]) - D) << endl;
			cerr << "The difference is not D" << endl;
		}
}

Neuron::~Neuron()
{}
