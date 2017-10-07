#include "Neuron.hpp"
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

constexpr double h(0.5);
constexpr double theta(20.0); //thresold limit

int main() 
{
	Neuron neuron;
	double spikeTime;
	double I(0.0);
	double I_ext;
	double Vr(10); //reset membrane potential
	size_t i(0);
	double t_start(0.0);
	double t_stop;
	double t(t_start);
	double a;
	double b;
	
	cout << "Enter a time for the simulation : " ;
	cin >> t_stop;
	cout << "Enter times a and b between 0 ms and 1000 ms : ";
	cin >> a;
	cin >> b;
	cout << "Enter a value for the current arriving at the neuron between 0 and 400 pA: ";
	cin >> I_ext;
	
	while(t < t_stop) {
		neuron.updateState(t,i);
		if((a <= t) and (t <= b)) {
			I = I_ext;
		} else { I = 0.0; }
		if(neuron.getState() == REFRACTORY) {
			neuron.setMembranePotential(0.0);
		} else if((neuron.getMembranePotential() > Vr) and (neuron.getState() == NON_REFRACTORY)) {
			if(neuron.getMembranePotential() >= theta) {
				spikeTime = t;
				neuron.storeSpikesTime(spikeTime);
				neuron.setSpikes(neuron.getSpikes()+1);
			}
			++i;
		}
		neuron.setMembranePotential(neuron.newMembranePotential(h,I));
		t = t+h;
	}
	
	ofstream file;
	file.open("NeuronInfos.txt");
	if(file.fail()) { 
		cerr << "Error opening text file" << endl; 
	} else {
		file << "Number of spikes : " << neuron.getSpikes() << endl;
		file << "Times when spikes occured : ";
		neuron.show(neuron.getSpikesOccured());
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
