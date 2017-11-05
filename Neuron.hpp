#ifndef NEURON_HPP
#define NEURON_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <array>
#include <random>

constexpr double taurp(2.0); //!< constant of time of the repository period
constexpr double tau(20.0); //!< constant of time 
constexpr double C(1.0); //!< capacity, connections received by each neuron
constexpr double R(tau/C); //!< input resistance
constexpr double h(0.1); //!< increment of time
constexpr double theta(20.0); //!< thresold limit

constexpr double t_stop(500.0); //!< number of steps of simulation, as h=0.1, the time of simulation is here 500 ms (t=n*h)
constexpr unsigned long n_stop(t_stop/h); //!< Maximal number of steps of the simulation based on time
constexpr int D(15); //!< delay before the spike is treated by the neuron
constexpr double g(6.0); //!< relative strenghts of connections g=J_inhibitory/J_excitatory
constexpr double J_excitatory(0.1); //!< amplitude of the spike, equal for all synapses
constexpr double J_inhibitory(g*J_excitatory); //!< amplitude of the spike, equal for all synapses

/////////constants for the computation of the new membrane potential
constexpr double c1(exp(-h/tau)); //!< constant 1
constexpr double c2(R*(1-exp(-h/tau))); //!< constant 2
/////////

constexpr unsigned long totalN(12500); //!< total number of neurons which form the network
constexpr unsigned long excitatoryNeurons(10000); //!< number of excitatory neurons in the network
constexpr unsigned long inhibitoryNeurons(2500); //!< number of inhibitory neurons in the network
constexpr unsigned long excitatoryConnections(1000); //!< number of excitatory connections a neurons receives
constexpr unsigned long inhibitoryConnections(250); //!< number of inhibitory connections a neurons receives

constexpr int etha(4); //!< value of externalFrequency over thresold frequency 
constexpr double externalFrequency((etha/0.1)*h); //!< rate at which a neuron receives a random spike
 
/*! @file Neuron.hpp
 * states in which the neuron can be (refractory or inhibitory)
*/
enum State {REFRACTORY, NON_REFRACTORY}; 

/*!
 * @class Neuron
 * Class that is used to model the behaviour of a neuron.
 * Neurons are separated into inhibitory and excitatory but are described in the same way.
 * This class gives acces to all informations about the neuron, its state and values of local time,
 * potential and so on. It updates the neuron at each time step based on the state of the neuron
 * and its features at that time. It also controls the spiking and reception of inputs based on a
 * delay that the neuron must respect.
 */
 
class Neuron {
	
	public:
	
	/*!
     * Constructor of the class Neuron
     * Important values and attributes of the neuron are initialized.
     * Especially, the neuron starts at the state non refractory and its buffer is equal to 0 for all the times of the delay
     * Every neuron created is by default excitatory and then we can choose if to change it's type
     * @param potential: value of the membrane potential; spike: the number of spikes; t: the time when spikes occured;
     * @param st: the state of the neuron; buffer: the vector to store the times of the spikes until the delay is reached
     * @param time: the clock of the neuron; excit: the type of the neuron, tg: the number of targets of the neuron
     */
	Neuron( double potential = 0.0, unsigned int spike = 0, 
			double t = 0.0, State st = NON_REFRACTORY, 
			std::vector<double> buffer = std::vector<double>((D+1), 0.0),
			double time = 0.0, bool excit = true, std::vector<Neuron*> tg = std::vector<Neuron*>(0), bool spk = false);
			//we initialize the ring buffer at a size that matches the delay
			//neurons starts in the non refractory state
			//neuron has no targets at the beginning of the simulation
	
	
	///////////////////////GETTERS////////////////////
	/*!
	 * Getter for the membrane potential of the neuron
	 * @return membranePotential
     */
	double getMembranePotential() const; 
	
	/*!
	 * Getter for the number of spikes fired by the neuron at the time
	 * @return spikes
     */
	unsigned int getSpikes() const;
	
	/*!
	 * Getter for the time at which a spike was fired
	 * @return spikesOccured
     */
	double getSpikesOccured() const;
	
	/*!
	 * Getter for the the state of the neuron
	 * @return state
     */
	State getState() const;
	
	/*!
	 * Getter for the local clock of the neuron
	 * @return clock
     */
	double getClock() const;
	
	/*!
	 * Getter for the type of the neuron, if it's excitatory of inhibitory
	 * @return excitatory
     */
	bool getExcitatory() const; 
	
	/*!
	 * Getter for the value int the ring buffer
	 * @return ringBuffer[i]
     */
	double getRingBuffer(int i) const;
	
	/*!
	 * Getter for the whole number of targets of the neuron
	 * @return targets
     */
	std::vector<Neuron*> getTargets() const;
	
	bool getSpk() const;
	
	///////////////////////SETTERS////////////////////
	
	/*!
	 * @param potential: a value for a potential
	 * Setter for the membrane potential
     */
	void setMembranePotential(double potential); 
	
	/*!
	 * @param spk: a value for the number of spikes
	 * Setter for the number of spikes fired by the neuron at the time
     */
	void setSpikes(unsigned int spk);
	
	/*!
	 * @param t: a value for the time when a spike was fired
	 * Setter for the time at which a spike was fired
     */
	void setSpikesOccured(double t);
	
	/*!
	 * @param st: a value for a state in which the neuron is
	 * Setter for the state of the neuron
     */
	void setState(State st);
	
	/*!
	 * @param time: a value for the clock of the neuron (its local time)
	 * Setter for the local clock of the neuron
     */
	void setClock(double time); 
	
	/*!
	 * @param excit: a value to determine the type of the neuron
	 * Setter for the type of the neuron, either excitatory or inhibitory
     */
	void setExcitatory(bool excit);
	
	/*!
	 * @param index, J: an index of the buffer and a value for the amplitude
	 * Setter for the ringBuffer at index "index"
     */
	void setRingBuffer(unsigned int index, double J);
	
	/*!
	 * @param n: a pointer on a neuron
	 * Setter for the targets of the neuron, adds the given neuron to the targets
     */
	void setTargets(Neuron* n); 
	
	/*!
	 * Calculates randomly a number of spikes that the neuron receives from the rest of the brain
	 * @return the number of random spikes times the value of the excitatory amplitude
     */
	double externalSpikes();
	
	/*!
	 * @param t: the simulation time at which the state must be updated
	 * says if neuron is refractory or not
     */
	void updateState(double t);
	
	/*!
	 * @param step: the simulation time at which the update must be done
	 * @param I: the input current received by the neuron
	 * @param recep: if the neuron receives or fires
	 * @param test: tells if the function is called for a test or not
	 * this function makes the update of the neuron based on :
	 * if the neuron is refractory
	 * if the neuron is spiking (spikes increase and membrane potential goes to zero)
	 * if the neuron is neither one nor the other case, so the next potential is calculated
     */
	void update(unsigned long step, double I, bool recep, bool test);
	
	/*!
	 * @param step: the simulation time at which the update must be done
	 * receives the time and fills the ring buffer of the targets of the neuron with the value of J_excitatory, J_inhibitory or 0
     */
	void fillRingBufferOfTargets(unsigned long step);
	
	/*!
	 * @param I: the external input current received
	 * @param J: the value of amplitude present in the ringBuffer
	 * recalculates the membrane potential at time t+h taking into account the random spikes
     */
	double newMembranePotential(double I, double J); 
	
	/*!
	 * @param I: the external input current received
	 * @param J: the value of amplitude present in the ringBuffer
	 * recalculates the membrane potential at time t+h without taking into account the random spikes
     */
	double newVTest(double I, double J);
	
	/*!
	 * destructor of the neuron class
     */
	~Neuron();
	
	private:
	
	double membranePotential; //!< membrane potential
	unsigned int spikes; //!< number of spikes fired by the neuron
	double spikesOccured; //!< time when the spike occured
	State state; //!< state of the neuron
	std::vector<double> ringBuffer; //!< buffer to keep in memory the spikes before the reaction of target neuron
	double clock; //!< local clock of the neuron  which tells at what time the neuron spikes
	bool excitatory; //!< if false, the neuron is inhibitory
	std::vector<Neuron*> targets; //!< vector containing all the targets of the neuron
	bool spike; //!< boolean to know if the neuron has spiked

};

#endif
