
#include <memory> // for shared_ptr
/* default size of the per-flow queue (in bytes) */
const int DEF_QUEUE_SIZE = std::INT_MAX; 

/* default flow weight */
const double DEF_FLOW_WEIGHT = 1.0


class flow{
public:
	//! packets in the flow (stored by arriving order)
	std::queue<std::shared_ptr<Packet> > packetQueue;
	//! weight of this flow
	double weight;
	//! maximum queue size (in terms of bytes)
	int maxQueueSize;
	//! current queue size (in term of bytes)
	int curQueueSize;
	//! default constructor
	flow(){
		weight = DEF_FLOW_WEIGHT;
		maxQueueSize = DEF_QUEUE_SIZE;
		curQueueSize = 0;		
	}
	//! constructor
	flow(weight_){
		weight = weight_;
		maxQueueSize = DEF_QUEUE_SIZE;
		curQueueSize = 0;
	}
	//! insert a packet
	void insert(Packet &pkt){
		std::shared_ptr<Packet> p = std::make_shared<Packet>(pkt);
		packetQueue.push(p);
		curQueueSize += pkt.size;
	}
	//! remove the first packet
	void remove()
	{
		if (packetQueue.empty())
			throw new std::runtime_error("Cannot remove packet from empty queue");
		std::shared_ptr<Packet> pkt = packetQueue.front();
		packetQueue.pop();
		curQueueSize -= pkt->size;
	}	
	//! 
	bool empty()
	{
		return packetQueue.empty();
	}
	//! function to set the flow weight
	void setWeight(int weight_)
	{
		weight = weight;
	}
};


class WFQ{
	maxHeap<std::shared_ptr<Packet> > HOLpackets;
	std::vector<std::shared_ptr<flow> > queues;
	double totalWeight;
	std::unordered_map<std::shared_ptr<Packet>,int/*flowId*/> packetFlow;
public:
	WFQ()
	{
		totalWeight = 0;
	}
	void enqueue(Packet &pkt)
	{
		
	}

};