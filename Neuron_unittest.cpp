#include <limits.h>
#include <iostream>
#include "Neuron.hpp"
#include "gtest/gtest.h"
#include <vector>

namespace {

	//Testing the difference in the amplitude

	TEST(TestNeuron, Amplitude) {
		
		Neuron neuron;
		std::vector <double> V(2,0.0); //vector to store the different membrane potential 
		
		//Neuron which is postsynaptic and receives 
		neuron.update(1,1.0,true);
		
		V[1] = neuron.getMembranePotential(); //we add the current potential
		V[0] = V[1]; //value is moved to the previous box	
		EXPECT_NEAR(V[1],V[0], pow(10,-2));

		//Firing neuron so doesn't receive
		neuron.update(1,1.0,false);
		V[1] = neuron.getMembranePotential(); //we add the current potential
		V[0] = V[1]; //value is moved to the previous box			
		EXPECT_NEAR(V[1],V[0], pow(10,-2));
	}

	// Test delay between the reception and the reaction
	TEST(TestNeuron, Response) {
		
		/*Neuron neuron;
		std::vector <double> T(D+1,0.0); //vector to store the different times of response
		unsigned int ix(D+1);
		double t(192.5);
		
		neuron.update(1,1.01,true);
		if(abs(neuron.getMembranePotential()- 0.1) <= pow(10,-3)) {
			T[D] = t;
			T[D+1] = (neuron.getClock()*h); //we add the current time
			EXPECT_NEAR(T[D+1],T[D], pow(10,-3));
		}*/
		
		Neuron neuron;
		std::vector<double> test(neuron.getTesting());
		//bool yes(false);
		double sum(0.0);
		
		for(size_t i(0); i < (test.size()-1); ++i) {
			sum += test[i];
			/*if(testing[i] == 0.0) {
				yes = true;
			}*/
		}
		EXPECT_NEAR(sum,0.0, pow(10,-3));
		EXPECT_NEAR(test[D+1],J, pow(10,-3));

	}

}
