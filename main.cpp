#include <iostream>
#include <exception>
#include "ogre_application.h"

/* Macro for printing exceptions */
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

/* Main function that builds and runs the application */
int main(void){
    ogre_application::OgreApplication application;

	try {
		application.Init();
		application.CreateTorusGeometry("TorusMesh");
		application.CreateCylinder("Cylinder", "ShinyBlueMaterial");
		application.CreateTorus("Torus", "ShinyBlueMaterial", Ogre::ColourValue(0.0,0.0,1.0), 100.5, 100.5, 100);
		application.CreateEntity("TorusEnt1" ,"TorusMesh", "ShinyTextureMaterial");
		//application.SetupAnimation("TorusEnt1");
		application.MainLoop();
	}
	catch (std::exception &e){
		PrintException(e);
	}

    return 0;
}
