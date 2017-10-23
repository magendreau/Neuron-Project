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
		
		Neuron neuron1;
		Neuron neuron2;

		neuron1.setSpikes(1);
		for(size_t i(1925); i <= 1925+D; ++i) { //we know that the first spike is fired at 192.5 ms so we wait until this time+D

			if(neuron1.getSpikes() > 0.0) {
				neuron2.fillRingBuffer(i);
				neuron2.update(i,1.01,true);
				EXPECT_NEAR(neuron1.getMembranePotential(), 0.0, 0.001); //check if membrane potential of the firing neuron has gone back to 0.0
			}
		}
		EXPECT_NEAR(neuron2.getMembranePotential(), 0.1, 0.001); //check if the response happens 
	}
	
	TEST(TestNeuron, ExternalCurrentPositive) {
		
		Neuron neuron;
		neuron.update(1,1.01,false);
		EXPECT_EQ(neuron.getState(), NON_REFRACTORY);
		EXPECT_NEAR(neuron.getMembranePotential(), 1.01*c2, 0.001); 
	}
	
	TEST(TestNeuron, ExternalCurrentNegative) {
		
		Neuron neuron;
		neuron.update(1,-1.01,false);
		EXPECT_EQ(neuron.getState(), NON_REFRACTORY);
		EXPECT_NEAR(neuron.getMembranePotential(), -1.01*c2, 0.001); 
	}
	

}
