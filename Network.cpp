#include "Network.hpp"
#include "Neuron.hpp"
#include <iostream>
#include <cassert>

using namespace std;

Network::Network()
{}

array<Neuron*, totalN> Network::getNeurons() const
{
	return neurons;
}

void Network::initializeNetwork()
{
	Neuron n1;
	n1.setExcitatory(true);
	Neuron n2;
	n2.setExcitatory(false);
	
	for( size_t i(0); i < neurons.size(); ++i) { //for each neuron of the network
		if(i < excitatoryNeurons) { //if the number of neurons if lower than the number expected for excitatory neurons (10000) 
			neurons[i] =  new Neuron(n1); //add an excitatory neuron
		}
		else { neurons[i] = new Neuron(n2); } //else an inhibitory one
	}
}

void Network::instaureConnections()
{
	//connections between neurons are made and they stay the same for the whole time of the simulation
	//connections are chosen randomly but each neuron has necessarily 1250 connections
	// 1000 excitatory and 250 inhibitory
	
	random_device rd;
	mt19937 generator(rd());

	uniform_int_distribution<unsigned int> uniformExcitatory(0,excitatoryNeurons-1);
	uniform_int_distribution<unsigned int> uniformInhibitory(excitatoryNeurons,totalN-1);

	for(auto n : neurons) { //for all neurons, create the connections with the others (1250 connections)
		
		for(size_t j(0); j < excitatoryConnections; ++j) {
			//each chosen excitatory neuron becomes a target of the present one(
			neurons[uniformExcitatory(generator)]->setTargets(n); 
		}
		for(size_t k(0); k < inhibitoryConnections; ++k) {
			//each chosen inhibitoryneuron becomes a target of the present one
			neurons[uniformInhibitory(generator)]->setTargets(n); 
			}
		}
}

Network::~Network()
{}
