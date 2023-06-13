#include "Point.hpp"

#include <stdlib.h>

int main(int argc, char* argv[])
{
	Point* point = 0;
	if (argc > 3) {
		point = new Point(atoi(argv[1]), atoi(argv[2]));
	}
	point->reflectAcrossX();
	
	return 0;
}
