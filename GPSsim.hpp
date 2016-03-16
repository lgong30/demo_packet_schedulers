#ifndef GPSSIM_HPP
#define GPSSIM_HPP

#include <math.h>       /* nearbyint */
#include <vector>
#include <fstream>
#include <string>
#include "demopacket.hpp"
#include "priorityqueue.hpp"


//! maximum number of flows
const int MAX_FLOW_NUM = 100;
//! default number of flows
const int DEFAULT_FLOW_NUM = 5;

//! class GPS simulator
class GPSSim{
	//! virtual time of last event
	double mThenVTime;
	//! real time of last event
	long int mThenRTime;
	//! current total weight
	double mSumWeight;
	//! is the system idle currently
	bool mIdling;
	//! priority queue of head of line packets
	PriorityQueue<Packet *,compare_vft> *mpPQ_HOL;
	//! the packet served currently
	Packet *mpCurPacket;
	//! real time for next wakeup
	long int mNextWakeupRTime;
	//! flows
	Flow* mpFlows;
	//! number of flows
	int mFlowNum;
public:
	//! constructor
	GPSSim(int flowNum = DEFAULT_FLOW_NUM){
		mThenVTime = 0.0;
		mThenRTime = 0;
		mSumWeight = 0.0;
		mIdling = true;
		mpPQ_HOL = new PriorityQueue<Packet *>();
		mpCurPacket = NULL;
		mNextWakeupRTime = 0;
		mFlowNum = flowNum;
		mpFlows = new Flow[mFlowNum];
	}
	//! constructor
	GPSSim(std::vector<double> flowWeights)
	{
		mThenVTime = 0.0;
		mThenRTime = 0;
		mSumWeight = 0.0;
		mIdling = true;
		mpPQ_HOL = new PriorityQueue<Packet *>();
		mpCurPacket = NULL;
		mNextWakeupRTime = 0;
		mFlowNum = flowWeights.size();
		mpFlows = new Flow[mFlowNum];

		for (int i = 0;i < mFlowNum;++ i)
			mpFlows[i] = new Flow(flowWeights[i]);		
	}

	void HandleNewPacketArrival(Packet *pPKT);
	void WakeupProcessing(long nowRTime);
	void ResetTimer(long nowRTime,double nowVTime,double newWakeupVTime);
	long GetNextWakeupRTime();
	bool BindPacket2Flow(Packet *pPKT);
	//void run(std::string input);
	
};
//! function bind the packet to the corresponding flow
bool GPSSim::BindPacket2Flow(Packet *pPKT)
{
	int flowId = pPKT->mFlowId;
	if (flowId >= 0 && flowId < mFlowNum)
	{
		pPKT->SetFlow(&mpFlows[flowId]);
		return true;
	}
	return false;
}
//! function to handle the newly arrived packet
void GPSSim::HandleNewPacketArrival(Packet *pPKT)
{
	//! current virtual time 
	double nowVTime;
	//! current real time
	long int nowRTime;
	//! flow pointer
	Flow *pFlow;
	//! packet pointer points to the packet which is enjoying the service
	Packet *pCurPacket;
    //! get current real 
	nowRTime = pPKT->mArrivalTime;

	//! check whether is system is idle or not
	if (mIdling)
	{
		nowVTime = 0.0;
		mIdling = false;
	}
	else
	{
		//! calculate current virtual time
		nowVTime = mThenVTime + (nowRTime - mThenRTime) / mSumWeight;
	}

	//! bind the arrival packet to flow
	if (!BindPacket2Flow(pPKT))
		throw new std::runtime_error("Cannot bind the packet to a unknown flow.");

	//! get the flow to which the newly arrived packet belongs
	pFlow = pPKT->mpFlow;
	//! get the flow's backlog status
	b = pFlow->IsBackloggedUnderGPS();

	if (!b)
	{
		//! newly active flow
		mSumWeight += pFlow->mWeight;
	}

	//! calculate the GPS virtual finish time for the newly arrived packet
	pPKT->GPS_VFTime = nowVTime + pPKT->mLength / pFlow->mWeight;
	//! append the packet into the corresponding flow
	pFlow->AppendPacket(pPKT);

	if (!b)
	{
		//! put the newly arrived packet into the priority queue of the head of line packet
		mpPQ_HOL->Enqueue(pPKT);
		//! get the packet with minimum GPS finish time
		pCurPacket = mpPQ_HOL->Peek_Min();
		//! check whether it is the same as the current packet
		if (pCurPacket != mpCurPacket)
		{
			mpCurPacket = pCurPacket;
		}
		//! reset the wakeup time
		ResetTimer(nowRTime,nowVTime,mpCurPacket->mGPS_VFTime);
	}
	//! update the virtual time of last event
	mThenRTime = nowVTime;
	//! update the real time of last event
	mThenRTime = nowRTime;

}
//! wakeup process
void GPSSim::WakeupProcessing(long nowRTime)
{
	double nowVTime;
	flow *pFlow;
	Packet *pPKT;
	nowVTime = mpCurPacket->mGPS_VFTime;
	mpPQ_HOL->PopMin();
	pFlow = mpCurPacket->mpFlow;
	pFlow->PopHOL();

	if (pFlow->IsBackloggedUnderGPS())
	{
		pPKT = pFlow->PeekHOL();
		mpPQ_HOL->Enqueue(pPKT);
	}
	else
	{
		mSumWeight -= pFlow->mWeight;
	}
	mpCurPacket = mpPQ_HOL->Peek_Min();
	if (mpCurPacket == NULL)
	{
		CleanUpAfterBusyPeriod();
	}
	else
	{
		ResetTimer(nowRTime,nowVTime,mpCurPacket->mGPS_VFTime);
	}
}

//! function to reset timer
void GPSSim::ResetTimer(long int nowRTime,double nowVTime,double newWakeupVTime)
{
	int newInterval;
	newInterval = (int) nearbyint((newWakeupVTime - nowVTime) * mSumWeight);
	mNextWakeupRTime = nowRTime + newInterval;
}

//! function to get next wakeup time
long GPSSim::GetNextWakeupRTime()
{
	return mNextWakeupRTime;
}


#endif