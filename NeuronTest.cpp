#include "Neuron.hpp"
#include <iostream>
#include <fstream>

using namespace std;

constexpr double t_start(0.0);

int main() 
{
	Neuron neuron;
	double a;
	double b;
	double t(t_start);
	
	cout << "Enter times a and b between 0 ms and 1000 ms : ";
	cin >> a;
	cin >> b;	
	
	ofstream file;
	file.open("NeuronInfos.txt");
	
	if(file.fail()) { 
		cerr << "Error opening text file" << endl; 
	} else {
		neuron.update(t,a,b,file);
		file << "Number of spikes : " << neuron.getSpikes() << endl;
		file << "Membrane potential is : " << neuron.getMembranePotential() << endl;
	}


	cout << "Number of spikes : " << neuron.getSpikes() << endl;
	cout << "Membrane potential is : " << neuron.getMembranePotential() << endl;
	
	return 0;
}
