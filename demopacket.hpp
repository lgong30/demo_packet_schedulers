#ifndef PACKET_HPP
#define PACKET_HPP

#include <iostream>
#include <queue>

/* default flow weight */
const double DEF_FLOW_WEIGHT = 1.0

//! packet class
class Packet{
public:	
	//! which flow the packet belongs to
	int mFlowId;
	//! the index of current packet 
	int mPacketId;
	//! size (in terms of bytes) of this packet
	int mLength;
	//! GPS virtual finish time for this packet
	double mGPS_VFTime; 
	//! real arrival time of this packet
	long int mArrivalTime;
	//! the flow the packet belongs to
	Flow *mpFlow; 
	//! constructor
	Packet(int flowId,int pktId,int pktSize,long int arrivalTime)
	{
		mFlowId = flowId;
		mPacketId = pktId;
		mLength = pktSize;
		mArrivalTime = arrivalTime;
		mpFlow = NULL;
	}
	//! set the flow to which this packet belongs
	void SetFlow(Flow *f)
	{
		mpFlow = f;
	}
	//! define less than function
	friend bool operator<(Packet &cPacket1,Packet  &cPacket2)
	{
		return cPacket1.mGPS_VFTime < cPacket2.mGPS_VFTime;
	}
};

//! flow class
class Flow{
public:
	//! weight of this flow
	double mWeight;    
    //! size of this flow (in terms of bytes)
	int mLength;
	//! packets in this flow
	std::queue<Packet *> mPackets;
	//! constructor
	Flow(double weight = DEF_FLOW_WEIGHT)
	{
		if (weight <= 0)
			throw new std::runtime_error("Cannot create flow with negative or zero weight.");
		mWeight = weight;
		mLength = 0;
	}
	//! insert a packet
	void AppendPacket(Packet *pkt){
		packetQueue.push(pkt);
		mLength += pkt->mLength;
	}
	//! remove the currently first packet (i.e., head of line packet)
	void PopHOL()
	{
		if (mPackets.empty())
			throw new std::runtime_error("Cannot pop HOL packet from an empty flow.");
		mPackets.pop();
	}
	//! get the currently head of line packet	
	Packet* PeekHOL()
	{
		if (mPackets.empty())
			throw new std::runtime_error("Cannot peek HOL packet from an empty flow.");
		return mPackets.front();
	}
	//! get the status of this flow
	bool IsBackloggedUnderGPS()
	{
		return mPackets.empty();
	}
};

#endif