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
		application.CreateCube();

		// (material, x ,y ,z , number for mesh names)
		application.CreateModel_1(-2, 0, 0, 1);
		application.CreateModel_2(-1, 0, 0, 2);
		application.CreateModel_3(-1.5, 1, 0, 3);
		application.CreateModel_Player(-3.0, 1.6, 0, 4);
		application.setCameraMode(0);

		application.MainLoop();
	}
	catch (std::exception &e){
		PrintException(e);
	}

    return 0;
}
