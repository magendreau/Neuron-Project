#include "Neuron.hpp"
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

constexpr double h(0.5); // interval of time and increment from time t to time t+h
constexpr double theta(20.0); //thresold limit, when the membrane potential reaches it a spike is produced

int main() 
{
	Neuron neuron;
	double spikeTime; //time when spike is produced
	double I(0.0); //current applied on neuron
	double I_ext; //current coming from rest of brain
	double Vr(10); //reset membrane potential
	size_t i(0);
	double t_start(0.0); //start of simulation
	double t_stop; //end of simulation
	double t(t_start); //present time
	double a;
	double b;
	
	cout << "How long do you want the simulation to be? : " ;
	cin >> t_stop;
	cout << "Enter times a and b between 0 ms and 1000 ms : ";
	cin >> a;
	cin >> b;
	cout << "Enter a value for the current arriving at the neuron between 0 pA and 400 pA: ";
	cin >> I_ext;
	
	while(t < t_stop) { //while the present time is below the time of the simulation
		neuron.updateState(t,i); 
		if((a <= t) and (t <= b)) { // if the simulation is between a and b
			I = I_ext; //current is the same as the external current
		} else { I = 0.0; }
		if(neuron.getState() == REFRACTORY) { //if the neuron is in the refractory state
			neuron.setMembranePotential(0.0); //membrane potential is zero
		} else if((neuron.getMembranePotential() > Vr) and (neuron.getState() == NON_REFRACTORY)) {
		  //if the membrane potential isn't at the reset value and the state is non refractory
			if(neuron.getMembranePotential() >= theta) { //is the thresold is reached
				spikeTime = t; //this time corresponds to a spike
				neuron.storeSpikesTime(spikeTime); 
				neuron.setSpikes(neuron.getSpikes()+1); //then the number of spikes increases
			}
			++i;
		}
		neuron.setMembranePotential(neuron.newMembranePotential(h,I)); //calculates the new membrane potential
		t = t+h; //increment of time
	}
	
	ofstream file; 
	file.open("NeuronInfos.txt"); //file containing the information about the neuron
	if(file.fail()) { //if opening of the file fail, send error message
		cerr << "Error opening text file" << endl; 
	} else {
		file << "Number of spikes : " << neuron.getSpikes() << endl;
		file << "Times when spikes occured : ";
		neuron.show(neuron.getSpikesOccured(), file);
		file << endl;
		file << "Membrane potential is : " << neuron.getMembranePotential() << endl;
	}
	
	/*cout << "Number of spikes : " << neuron.getSpikes() << endl;
	cout << "Times when spikes occured : " ;
	neuron.show(neuron.getSpikesOccured());
	cout << endl;
	cout << "Membrane potential is : " << neuron.getMembranePotential() << endl;*/
	
	return 0;
}
