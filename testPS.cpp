#include <iostream>
#include "packetScheduler.hpp"

int main()
{
	std::string input("packets.dat");

	PacketScheduler ps(input);
	ps.print();

	return 0;

}