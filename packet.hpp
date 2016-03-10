#ifndef PACKET_HPP
#define PACKET_HPP

#include <iostream>
#include <string>

class Packet{
public:
	std::string srcIP;
	std::string dstIP;
	int srcPort;
	int dstPort;
	int protocol;
	int size;
	Packet(std::string srcip_,std::string dstip_,int size_): srcIP(srcip_),
												   dstIP(dstip_),
												   size(size_)
												   {srcPort = 0;
												   	dstPort = 0;
												    protocol = 0;}
	Packet(std::string srcip_,std::string dstip_,int srcPort_,int dstPort_,int protocol_,int size_): srcIP(srcip_),
																						   dstIP(dstip_),
																						   srcPort(srcPort_),
																						   dstPort(dstPort_),
																						   protocol(protocol_),
																						   size(size_){}	
};

#endif