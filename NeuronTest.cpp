#include "Neuron.hpp"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

constexpr unsigned int n_start(0); //first step of the simulation

int main() 
{
	Neuron neuron1;
	Neuron neuron2;
	vector<Neuron> neurons; //fire neurons
	vector<Neuron> target; //postsynaptic neurons
	
	unsigned int n(n_start); //actual step of the simulation
	double I_ext; //external input current

	double a(100.0);
	double b(400.0);
	double I(0.0);
	unsigned int spk(0);
	bool reception(false);
	
	//testing vectors
	vector <double> V(2,0.0); //vector to store the different membrane potential 
	vector <double> T(D+1,0.0); //vector to store the different times of response
	unsigned int ix(D+1);
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
		} else { I = 0.0; }
		 
			for(size_t i(0); i< neurons.size() ; ++i) { //for each fire neuron
				(neurons[i]).update(n,I, reception);
				if( neurons[i].getSpikes() > spk) { //if this neuron fired at least one spike
					file << "We are at time " << n*h << endl;
					file << "Neuron 1: " << endl;
					file << "Number of spikes : " << neurons[i].getSpikes() << endl;
					file << "Membrane potential is : " << neurons[i].getMembranePotential() << endl;
					++spk;
				}
			
				 if((neurons[i]).getSpikes() > 0) { //if this neuron fired at least one spike
					for(size_t j(0); j < target.size() ; ++j) { //for each postsynaptic neuron
							reception = true;
							(target[j]).fillRingBuffer(n); //we fill his ring buffer with the spikes he receives 
							(target[j]).update(n, I, reception);
							if(target[j].getMembranePotential() > 0.0) {
							file << "We are at time " << n*h << endl;
							file << "Neuron 2: " << endl;
							file << "Membrane potential is : " << target[j].getMembranePotential() << endl;
							file << endl;
						}
							
							//testing part for J and D
							/*V[1] = ((target[j]).getMembranePotential()); //we add the current potential
							T[ix] = (n*h); //we add the current time
							(target[j]).testDifferencePotential(V);
							(target[j]).testDelay(T, ix);
							V[0] = V[1];
							T[0] = T[1];
							--ix;
							if(ix < T.size()) { ix=D+1; }*/
					 }
				 }
			 }
			 ++n; //increase of the steps of the simulation
			 reception = false;
		}  
	}
	return 0;
}
