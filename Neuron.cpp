#include "Neuron.hpp"
#include <iostream>
#include <cmath>

using namespace std;

Neuron::Neuron(double potential, double spike, double t, State st, std::vector<unsigned int> buffer)
	:membranePotential(potential), spikes(spike), spikesOccured(t), state(st), ringBuffer(buffer)
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

void Neuron::update(double step, double I)
{	
	while(step < n_stop) { //for the local time, if it hasn't reach the total time it continues
		updateState(step*h);
		if(state == REFRACTORY) {
			setMembranePotential(0.0); // when the neuron is refractory it's membrane potential is 0
		} else if((membranePotential > Vr) and (state == NON_REFRACTORY)) { 
			if((membranePotential >= theta) and (I > 1.0)) { //if the membrane potential reaches the thresold
				spikesOccured = step*h; //a spike is emitted
				cout << "A spike occured at t=" << spikesOccured << " ms" << endl;
				++spikes;
			}
		}
		setMembranePotential(newMembranePotential(I)); //calculates the new membrane potential
		++step;
	}
	
	/*while(step < n_stop) { //while the number of steps doesn't reach the total number
		updateState(step*h);
		
		if(state == REFRACTORY) {
			setMembranePotential(0.0);
			spikes = 0.0;
		} else if((membranePotential > Vr) and (state == NON_REFRACTORY)) {
			//setMembranePotential(newMembranePotential(I,S));
			if((membranePotential >= theta) and (I > 1.0)) {
				spikesOccured = step*h;
				cout << "A spike occured at t=" << spikesOccured << " ms" << endl;
				++spikes;
				cout << " Local cout spikes " << spikes << endl;
				membranePotential = 0.0;
			}
		}
		setMembranePotential(newMembranePotential(I,S));
		++step;
	}*/
	
	/*while(step < n_stop) { //while the number of steps doesn't reach the total number
		updateState(step*h);

		if((membranePotential > theta) and (I > 1.0)) {
			spikesOccured = step*h;
			cout << "A spike occured at t=" << spikesOccured << " ms" << endl;
			++spikes;
			membranePotential = 0.0;
			state = REFRACTORY;
		} else if(state == REFRACTORY) {
			membranePotential = 0.0;
		} else {
			setMembranePotential(newMembranePotential(I,spikes));
		}
		++step;
	}*/	
}

void Neuron::fillRingBuffer(double nbSpikes, unsigned int readOut, unsigned int step)
{
	if(step >= ringBuffer.size()) { step = 0; } //if the step of the simulation is higher than the size of the buffer 
												//it means we can empty or replace the first case, so step goes to 0
	ringBuffer[step*h] = nbSpikes; //stores for each time or step the number of spike emitted by the fire neuron in a place in the ring
}

bool Neuron::fullBuffer(unsigned int origin)
{
	if(origin == (ringBuffer.size()+1)) { //if the origin of the ring buffer reaches the size of the buffer
										  //the buffer is full and the delay is passed for the spikes in the first case
		return true;
	} 
	return false;
}

void Neuron::receive(double step, double J, double I)
{
	setMembranePotential(newMembranePotential(I) + J); //membrane potential of postsynaptic neuron changes after reception and delay
	spikesOccured = step*h; //a spike occures for the postsynaptic neuron
	testDifferencePotential(membranePotential); //we assure that the difference between the potential is the amplitude J
}

double Neuron::newMembranePotential(double I)
{
	return (membranePotential = exp(-h/tau)*membranePotential + I*R*(1-exp(-h/tau)));
}

void Neuron::testDifferencePotential(double V1)
{
	vector <double> V(1,0.0); //vector to store the different membrane potentials
	V.push_back(V1);
	
	for(size_t i(0) ; i < V.size() ; ++i) {
		if(abs(abs(V[i]-V[i+1]) - J) < pow(10,-3)) { //if the difference between the two values and J is low enough
			cout << "The difference between the two potential is J" << endl;
		} else {
			cerr << "The difference is not J" << endl;
		}
	}
}

Neuron::~Neuron()
{}
