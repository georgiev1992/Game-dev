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
		application.CreateEntity("TorusEnt1" ,"TorusMesh", "Default_Blue_Light");
		application.CreateModel_1("Toon_One_Blue_Lighted");
		application.CreateModel_2();

		application.SetupAnimation("TorusEnt1");
		application.MainLoop();
	}
	catch (std::exception &e){
		PrintException(e);
	}

    return 0;
}
