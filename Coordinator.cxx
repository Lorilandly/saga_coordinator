#include <cstdlib>
#include "Listener.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " number" << std::endl;
		return 1;
	}
	listener(std::atoi(argv[1]));
}
