#include "SkolarisInstance.h"
#include <iostream>
#include <string>

void SkolarisInstance::postMessage(const char *msg) const
{
	std::cout << msg << '\n';
}

int main()
{
	std::string line;
	while (std::getline(std::cin, line)) {
		if (!line.empty())
			SingleSkolarisInstance::Instance().handleMessage(line);
	}
	if (std::cin.bad()) {
		std::cerr << "Error reading from stdin\n";
		return 1;
	}
	return 0;
}
