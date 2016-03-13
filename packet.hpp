#ifndef PACKET_HPP
#define PACKET_HPP

#include <iostream>
#include <string>

/* default flow weight */
const double DEF_FLOW_WEIGHT = 1.0

class Packet{
public:
	// std::string srcIP;
	// std::string dstIP;
	// int srcPort;
	// int dstPort;
	// int protocol;
	// int size;
	// Packet(std::string srcip_,std::string dstip_,int size_): srcIP(srcip_),
	// 											   dstIP(dstip_),
	// 											   size(size_)
	// 											   {srcPort = 0;
	// 											   	dstPort = 0;
	// 											    protocol = 0;}
	// Packet(std::string srcip_,std::string dstip_,int srcPort_,int dstPort_,int protocol_,int size_): srcIP(srcip_),
	// 																					   dstIP(dstip_),
	// 																					   srcPort(srcPort_),
	// 																					   dstPort(dstPort_),
	// 																					   protocol(protocol_),
	// size(size_){}	
	// 

public:	
	int flowId;// which flow the packet belongs to
	int packetId;// the index of current packet (for demo purpose)
	int size;// size (in terms of bytes) of this packet

	double VFTime; // virtual finish time for this packet
	long int arrivalTime;// real arrival time of this packet

	explicit Packet(){}
	explicit Packet(int fId_,int pId_,int size_,long int arrivalTime_)
	{
		flowId = fId_;
		packetId = pId_;
		size = size_;
		arrivalTime = arrivalTime_;
	}




};

class FirstPacket: public Packet{
public:
	double weight;
	FirstPacket(){}
	FirstPacket(int fId_,int pId_,int size_,long int arrivalTime_,double weight_ = DEF_FLOW_WEIGHT)
	{
		Packet(fId_,pId_,size_,arrivalTime_);
		weight = weight_;
	}

}

#endif