#include <limits.h>
#include <iostream>
#include "Neuron.hpp"
#include "Network.hpp"
#include "gtest/gtest.h"
#include <vector>

namespace {

	////Testing the difference in the amplitude
	/////////////////
	TEST(TestNeuron, Amplitude) {
		
		Neuron neuron;
		std::vector <double> V(2,0.0); //vector to store the different membrane potentials 
		
		//Postsynaptic neuron that receives 
		neuron.update(1,1.0,true, true);
		
		V[1] = neuron.getMembranePotential(); //we add the current potential
		V[0] = V[1]; //value is moved to the previous box	
		EXPECT_NEAR(V[1],V[0], pow(10,-2)); //verify that the membrane potentials increase of J

		//Firing neuron so doesn't receive
		neuron.update(1,1.0,false, true);
		V[1] = neuron.getMembranePotential(); //we add the current potential
		V[0] = V[1]; //value is moved to the previous box			
		EXPECT_NEAR(V[1],V[0], pow(10,-2));
	}

	///////Test delay between the reception and the reaction
	/////////////////
	TEST(TestNeuron, Response) {
		
		Neuron n1;
		Neuron n2;
		
		Neuron* neuron1(new Neuron(n1)); //firing neuron
		Neuron* neuron2(new Neuron(n2)); //postsynaptic neuron
	
		neuron1->setTargets(neuron2); //neuron2 becomes a target of neuron1
		neuron1->setSpikes(1); //the number of spikes of neuron1 is initialized at 1 to check this precise moment
		neuron1->fillRingBufferOfTargets(118); //neuron1 fills the buffer of neuron2
		
		for(size_t i(118); i <= 118+D; ++i) { //we know that the first spike is fired at 11.8 ms so we wait until this time+D

			if(neuron1->getSpikes() > 0.0) { //if neuron1 has spiked
				neuron2->update(i,0.0,true, true); //neuron2 reacts and updates
				EXPECT_NEAR(neuron1->getMembranePotential(), 0.0, 0.001); //check if membrane potential of the firing neuron
																		// has gone back to 0.0 so the refractory state
			}
		}
		EXPECT_NEAR(neuron2->getMembranePotential(), 0.1, 0.001); //check if the response happens after delay D
	}
	
	//////check the computation of new membrane potential at time 1
	///////////case where I_ext is positive
	TEST(TestNeuron, ExternalCurrentPositive) {
		
		Neuron neuron;
		neuron.update(1,1.01,false, true);
		EXPECT_EQ(neuron.getState(), NON_REFRACTORY); //check that the neuron is in a non-refractory state
		EXPECT_NEAR(neuron.getMembranePotential(), 1.01*c2, 0.001); //membrane potential at time 0 is 0mV and J is 0 too
	}
	
	//////check the computation of new membrane potential at time 1
	///////////case where I_ext is positive
	TEST(TestNeuron, ExternalCurrentNegative) {
		
		Neuron neuron;
		neuron.update(1,-1.01,false, true);
		EXPECT_EQ(neuron.getState(), NON_REFRACTORY); //check that the neuron is in a non-refractory state
		EXPECT_NEAR(neuron.getMembranePotential(), -1.01*c2, 0.001); //membrane potential at time 0 is 0mV and J is 0 too
	}
	
	/////Test the time when spikes arrive
	////////////////////
	TEST(TestNeuron, SpikeTimes) {
		
		Neuron neuron;
		for(size_t i(0); i < 1924; ++i) { //we make the neuron update until the time before the first spike
			if(i < 1000) { neuron.update(i,0.0,false, true); } //like in the main, we instaure an interval of time
			else {
			neuron.update(i,1.01,false, true);}
		}
		neuron.update(1, 1.01, false, true); //update once again (aka the time of the first spike)
		EXPECT_EQ(neuron.getClock(), 1925);
		EXPECT_NEAR(neuron.getSpikesOccured(), 192.4, 0.001); 
	}
	
	TEST(TestNetwork, networkSize) {
		Network net;
		net.initializeNetwork();
		EXPECT_EQ(net.getNeurons().size(), 12500);
	}
	
}
