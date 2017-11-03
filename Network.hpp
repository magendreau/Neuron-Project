#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <iostream>
#include "Neuron.hpp"
#include <array>
#include <random>

/*!
 * @class Network
 * Class that is used to model the network of neurons in the brain.
 * We consider a network of 12500 neurons with 10000 excitatory and 2500 inhibitory.
 * The connections are 1000 with excitatory neurons and 250 with inhibitory ones.
 * Each neuron has the same number of connections (1250).
 */
 
class Network {
	
	public:
	
	/*!
     * Constructor of the class Network
     */
	Network();
	
	/*!
     * Getter of the vector neurons which contains all the neurons of the network
     * @return neurons; 
     */
	std::array<Neuron*, totalN> getNeurons() const;

	/*!
     * Functions used to initialize the vector neurons.
     * It creates two neurons, one excitatory and one inhibitory and it fills the array.
     * It puts 10000 excitatory neurons and 2500 inhibitory neurons
     */
	void initializeNetwork();
	
	/*!
	 * Instaures the connections between the neurons randomly
	 * 1000 with excitatory ones and 250 for inhibitory
     */
	void instaureConnections();
	
	/*!
     * destructor of the class Network
     */
	~Network();
	
	private:

	std::array<Neuron*, totalN> neurons; //!< vector containing the neuron that compose the network (12500)

};

#endif
