
#include <memory> // for shared_ptr
/* default size of the per-flow queue (in bytes) */
const int DEF_QUEUE_SIZE = std::INT_MAX; 

/* default flow weight */
const double DEF_FLOW_WEIGHT = 1.0


class flow{
public:
	std::queue<std::shared_ptr<Packet> > packetQueue;
	double weight;
	int maxQueueSize;
	int curQueueSize;
	flow(){
		weight = DEF_FLOW_WEIGHT;
		maxQueueSize = DEF_QUEUE_SIZE;
		curQueueSize = 0;		
	}
	flow(weight_){
		weight = weight_;
		maxQueueSize = DEF_QUEUE_SIZE;
		curQueueSize = 0;
	}
	void insert(Packet &pkt){
		std::shared_ptr<Packet> p = std::make_shared<Packet>(pkt);
		packetQueue.push(p);
		curQueueSize += pkt.size;
	}
	void remove()
	{
		if (packetQueue.empty())
			throw new std::runtime_error("Cannot remove packet from empty queue");
		std::shared_ptr<Packet> pkt = packetQueue.front();
		packetQueue.pop();
		curQueueSize -= pkt->size;
	}	
	bool empty()
	{
		return packetQueue.empty();
	}
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