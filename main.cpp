#include <iostream>
#include <SFML/Graphics.hpp>
#include "headers/scene.h"
int main() {

	eg::SecondOrderCone object({ 5, 5, 6 }, 50.0);
	eg::Scene scene(1500, 800);
	scene.ProcessCycle(object);

	return 0;
}