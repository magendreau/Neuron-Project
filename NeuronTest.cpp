#include "Neuron.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <array>

using namespace std;

constexpr unsigned long n_start(0); //first step of the simulation

int main() 
{
	array<Neuron*, totalN> neurons; //vector containing the neuron that compose the network (12500)
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
	
	unsigned int n(n_start); //actual step of the simulation
	double I(0.0); //external input current
	
	ofstream file;
	file.open("Data.txt");
	
	if(file.fail()) { 
		cerr << "Error opening text file" << endl; 
	} else {
		
		for(auto p : neurons) { //for all neurons, create the connections with the others (1250 connections)
			p->instaureConnections(neurons);
		}
		
		do { //while we don't reach the total steps of the simulation
			
			cout << "We are at step " << n << " of the simulation" << endl;
			
			for(size_t ne(0); ne < neurons.size() ; ++ne) { //for each neuron
				neurons[ne]->update(n, I, false, false); //update the neuron
				if(neurons[ne]->getSpk()) { //if there was a spike, we write it in a file with the id of the neuron
					file << neurons[ne]->getSpikesOccured()/h << '\t' << ne << '\n';
				}
			}
			
			++n; //increase of the steps of the simulation 
			
		} while(n < n_stop);
		
	}
	
	for(auto& neuron : neurons) { //for all neurons, delete the pointer
		neuron = nullptr;
		delete neuron;
	}
	
	system("python pythonScript.py");
	
	return 0;
}
