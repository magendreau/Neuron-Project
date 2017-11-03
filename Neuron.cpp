#include "Neuron.hpp"
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

Neuron::Neuron( double potential, unsigned int spike, double t, State st, vector<double> buffer,
				double time, bool excit, std::vector<Neuron*> tg, bool spk)
	:membranePotential(potential), spikes(spike), spikesOccured(t), state(st), ringBuffer(buffer),
	 clock(time), excitatory(excit), targets(tg), spike(spk)
{}

////////////////GETTERS//////////////////////

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

bool Neuron::getExcitatory() const 
{
	return excitatory;
}

double Neuron::getRingBuffer(int i) const
{
	return ringBuffer[i];
}

vector<Neuron*> Neuron::getTargets() const
{
	return targets;
}

bool Neuron::getSpk() const
{
	return spike;
}

//////////////////SETTERS////////////////////////

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

void Neuron::setExcitatory(bool excit) 
{
	excitatory = excit;
}

void Neuron::setRingBuffer(unsigned int index, double J)
{
	//increases the ringBuffer at index "index" of J
	ringBuffer[index] += J;
}

void Neuron::setTargets(Neuron* n)
{
	targets.push_back(n);
}

/////////////////////////OTHER FUNCTIONS///////////////////////

double Neuron::externalSpikes()
{
	//creates a random value to add to the buffer which represents the number of spikes coming from the rest of the brains
	static random_device rd;
	static mt19937 generator(rd());
	static poisson_distribution<int> distribution(externalFrequency); //Poisson distribution describes the random repartition
	return (distribution(generator)*J_excitatory);  //we multiply it by the amplitude for the excitatory connections
													//as each spike increases the membrane potential of J_excitatory
}

void Neuron::updateState(double t)
{
	if(((spikesOccured <= t) and (t < (spikesOccured+taurp)) and (spikesOccured != 0.0))) {
		//if the time is during the refractory period and it's not 0 
		//( because the default time is 0 and otherwise it would start with 2 ms of delay)
		state = REFRACTORY;
	} else {
		state = NON_REFRACTORY;
	}
}

void Neuron::update(unsigned long step, double I, bool recep, bool test)
{	
		updateState(clock*h);

		if(state == REFRACTORY) {
			membranePotential = 0.0; // when the neuron is refractory it's membrane potential is 0
			spike = false;
			
		} else if((membranePotential >= theta)) { //if the membrane potential reaches the thresold
			if(!recep) { //if the neuron is not receiving 
				spikesOccured = clock*h; //a spike is emitted
				//cout << "A spike occured at t=" << spikesOccured << " ms" << endl;
				++spikes;
				spike = true;
				fillRingBufferOfTargets(step);  //as the number of spikes increased ( the neuron spiked) 
												//the neuron can fill the ring buffer of each one of its targets
			}
			membranePotential = 0.0; //after spiking the membrane potential goes back to zero
			state = REFRACTORY;
		} else {
			//if the neuron is neither refractory nor spiking, its membrane potential is increasing
			spike = false;
			const unsigned int t = step%(D+1);
			assert(t < D+1);
			if(test) { //if we are doing a test
				membranePotential = newVTest(I, ringBuffer[t]); //we don't take into account the random part to see if delay is working
			} else {
				membranePotential = newMembranePotential(I, ringBuffer[t]); //membrane potential of neuron changes after reception
			}
			ringBuffer[t] = 0; //after using its ring buffer value, it goes back to zero
		}
		
	++clock;
}

void Neuron::instaureConnections(array <Neuron*, totalN>& neurons)
{
	//connections between neurons are made and they stay the same for the whole time of the simulation
	//connections are chosen randomly but each neuron has necessarily 1250 connections
	// 1000 excitatory and 250 inhibitory
	
	random_device rd;
	mt19937 generator(rd());

	uniform_int_distribution<unsigned int> uniformExcitatory(0,excitatoryNeurons-1);
	uniform_int_distribution<unsigned int> uniformInhibitory(excitatoryNeurons,totalN-1);

		for(size_t j(0); j < excitatoryConnections; ++j) {
			//each chosen excitatory neuron becomes a target of the present one
			neurons[uniformExcitatory(generator)]->targets.push_back(this); 
		}
		for(size_t k(0); k < inhibitoryConnections; ++k) {
			//each chosen inhibitoryneuron becomes a target of the present one
			neurons[uniformInhibitory(generator)]->targets.push_back(this); 
		}
}
	
void Neuron::fillRingBufferOfTargets(unsigned long step)
{
	const unsigned int readOut = (step+D)%(D+1);
	assert((readOut) <= ringBuffer.size());
	
	//if the neuron has spiked and he has targets
	if((!targets.empty()) and (spikes > 0)) {
		for(auto const& target : targets) {
			assert(target != nullptr);
			if(excitatory) { //if the neuron is excitatory it adds to its target's buffer the excitatory amplitude value
				target->setRingBuffer(readOut, J_excitatory); 
			} else { 
				target->setRingBuffer(readOut, -J_inhibitory); //otherwise the inhibitory amplitude value
			} 
		}
	}
}

double Neuron::newMembranePotential(double I, double J)
{
	//computation of new membrane potential with the random spikes coming from the brain 
	return (membranePotential = c1*membranePotential + I*c2 + J + externalSpikes());
}

double Neuron::newVTest(double I, double J)
{
	//computation of new membrane potential without the random spikes coming from the brain 
	return (membranePotential = c1*membranePotential + I*c2 + J);
}

Neuron::~Neuron()
{}
