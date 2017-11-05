#include "Neuron.hpp"
#include "Network.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <array>

using namespace std;

constexpr unsigned long n_start(0); //first step of the simulation

int main() 
{
	
	Network net; //network with all the neurons
	unsigned int n(n_start); //actual step of the simulation
	double I(0.0); //external input current
	int ind(0); //index of each neuron, number id
	
	net.initializeNetwork(); //the network is initialized at 12500 neurons
	
	ofstream file;
	file.open("Data.txt");
	
	if(file.fail()) { 
		cerr << "Error opening text file" << endl; 
	} else {
		
		net.instaureConnections(); //connections between the neurons are created, 1250 connections
		
		do { //while we don't reach the total steps of the simulation
			
			cout << "We are at step " << n << " of the simulation" << endl;
			
			for(auto neuron : net.getNeurons() ) { //for each neuron
				neuron->update(n, I, false, false); //gets updated
				if(neuron->getSpk()) { //if there was a spike, we write it in a file with the id of the neuron
					file << neuron->getSpikesOccured()/h << '\t' << ind << '\n';
				}
				++ind; //index of the neuron in the vector of class network
			}
			
			ind = 0; //for each while we start back at zero
			++n; //increase of the steps of the simulation 
			
		} while(n < n_stop);
		
	}
	
	system("python pythonScript.py");
	
	return 0;
}
