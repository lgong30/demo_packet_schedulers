#include <iostream>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;


int main()
{
	json array_not_object;
	array_not_object["test"] ={{{"a",1}},{{"a",2}}};
	array_not_object["test"].push_back({{"a",3}});
	cout << array_not_object.dump() << endl;

	return 0;
}