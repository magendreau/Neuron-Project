#include "Neuron.hpp"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

constexpr double t_start(0.0); //start of the simulation
constexpr unsigned int n_start(0); //first step of the simulation

int main() 
{
	Neuron neuron1;
	Neuron neuron2;
	vector<Neuron> neurons; //fire neurons
	vector<Neuron> target; //postsynaptic neurons
	
	unsigned int n(n_start); //actual step of the simulation
	double I_ext; //external input current
	unsigned int readOut(0); //origin of the ring buffer of each neuron
	double a(100.0);
	double b(400.0);
	double I(0.0);
	
	//testing vectors
	vector <double> V(1,Vr); //vector to store the different membrane potential 
	vector <double> T(1,0.0); //vector to store the different times of response
	size_t indexTest(0);
	//testing vectors
	
	cout << "Enter the external input I_ext : " ;
	cin >> I_ext;

	ofstream file;
	file.open("NeuronInfos.txt");
	
	neurons.push_back(neuron1); 
	target.push_back(neuron2);
	 
	if(file.fail()) { 
		cerr << "Error opening text file" << endl; 
	} else {
		
		while(n < n_stop) { //while we don't reach the total steps of the simulation
		if((a < n*h) and (n*h < b)) {
			I = I_ext;
			cout << "We are at step nb " << n << endl;
			file << "We are at time " << n*h << endl;
		} else { I = 0.0; }
		
			for(size_t i(0); i< neurons.size() ; ++i) { //for each fire neuron
				(neurons[i]).setSpikes(0.0); //we reset the spikes at 0 for the counting of each step
				(neurons[i]).update(n,I);
				if( neurons[i].getSpikes() > 0.0) { //if this neuron fired at least one spike
					file << "Neuron 1: " << endl;
					file << "Number of spikes : " << neurons[i].getSpikes() << endl;
					file << "Membrane potential is : " << neurons[i].getMembranePotential() << endl;
				}
					
				 if((neurons[i]).getSpikes() != 0.0) { //if this neuron fired at least one spike
					for(size_t j(0); j < target.size() ; ++j) { //for each postsynaptic neuron
						(target[j]).fillRingBuffer(neurons[i].getSpikes(), readOut, n); //we fill his ring buffer with the spikes he receives 
						++readOut; //the origin of the buffer changes position
						
						if((target[j]).fullBuffer(readOut)) { //if the delay is finished
							++indexTest; //we increase an index to know at whitch point we are with the postsynaptic neuron
							(target[j]).receive(n, J, I);
							readOut = 0; //we reset the origin at the beginning of the buffer
							file << "Neuron 2: " << endl;
							file << "Membrane potential is : " << target[j].getMembranePotential() << endl;
							
							//testing part for J and D
							V.push_back((target[j]).getMembranePotential()); //we add the current potential
							T.push_back(n*h); //we add the current time
							(target[j]).testDifferencePotential(V, indexTest);
							(target[j]).testDelay(T, indexTest);
						}
					 }
				 }
			 }
			 ++n; //increase of the steps of the simulation
			// cout << endl;
			 file << endl;
		}
		
	}
	
	return 0;
}
