#include "ogre_application.h"
#include "bin/path_config.h"

namespace ogre_application {

/* Some configuration constants */
/* They are written here as global variables, but ideally they should be loaded from a configuration file */

/* Initialization */
const Ogre::String config_filename_g = "";
const Ogre::String plugins_filename_g = "";
const Ogre::String log_filename_g = "Ogre.log";

/* Main window settings */
const Ogre::String window_title_g = "Demo";
const Ogre::String custom_window_capacities_g = "";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const bool window_full_screen_g = false;

/* Viewport and camera settings */
float viewport_width_g = 0.95f;
float viewport_height_g = 0.95f;
float viewport_left_g = (1.0f - viewport_width_g) * 0.5f;
float viewport_top_g = (1.0f - viewport_height_g) * 0.5f;
unsigned short viewport_z_order_g = 100;
float camera_near_clip_distance_g = 0.01;
float camera_far_clip_distance_g = 100.0;
Ogre::Vector3 camera_position_g(0.5, 0.5, 1.5);
Ogre::Vector3 camera_look_at_g(0.0, 0.0, 0.0);
Ogre::Vector3 camera_up_g(0.0, 1.0, 0.0);
const Ogre::ColourValue viewport_background_color_g(1.0, 0.0, 0.0);;

bool Lazy= true;
bool qp = false;
bool ep = false;

/* Materials */
const Ogre::String material_directory_g = MATERIAL_DIRECTORY;


OgreApplication::OgreApplication(void){

    /* Don't do work in the constructor, leave it for the Init() function */
}


void OgreApplication::Init(void){

	/* Set default values for the variables */
	animating_ = false;
	space_down_ = false;

	/* Run all initialization steps */
    InitRootNode();
    InitPlugins();
    InitRenderSystem();
    InitWindow();
    InitViewport();
	InitFrameListener();
	InitOIS();
	LoadMaterials();

	InitCompositor();
}


void OgreApplication::InitRootNode(void){

    try {
		
		/* We need to have an Ogre root to be able to access all Ogre functions */
        ogre_root_ = std::auto_ptr<Ogre::Root>(new Ogre::Root(config_filename_g, plugins_filename_g, log_filename_g));
		//ogre_root_->showConfigDialog();

    }
    catch (Ogre::Exception &e){
		throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
		throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitPlugins(void){

    try {

        Strings plugin_names;
        plugin_names.push_back("RenderSystem_GL");
		
        Strings::iterator iter = plugin_names.begin();
        Strings::iterator iter_end = plugin_names.end();
        for (; iter != iter_end; iter++){
            Ogre::String& plugin_name = (*iter);
            if (OGRE_DEBUG_MODE){
                plugin_name.append("_d");
            }
            ogre_root_->loadPlugin(plugin_name);
        }

    }
    catch (Ogre::Exception &e){
		throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitRenderSystem(void){

    try {

        const Ogre::RenderSystemList& render_system_list = ogre_root_->getAvailableRenderers();
        if (render_system_list.size() == 0)
        {
			throw(OgreAppException(std::string("OgreApp::Exception: Sorry, no rendersystem was found.")));
        }

        Ogre::RenderSystem *render_system = render_system_list.at(0);
        ogre_root_->setRenderSystem(render_system);

    }
    catch (Ogre::Exception &e){
		throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

        
void OgreApplication::InitWindow(void){

    try {

        /* Create main window for the application */
		bool create_window_automatically = false;
        ogre_root_->initialise(create_window_automatically, window_title_g, custom_window_capacities_g);

        Ogre::NameValuePairList params;
        params["FSAA"] = "0";
        params["vsync"] = "true";
        ogre_window_ = ogre_root_->createRenderWindow(window_title_g, window_width_g, window_height_g, window_full_screen_g, &params);

        ogre_window_->setActive(true);
        ogre_window_->setAutoUpdated(false);
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitViewport(void){

    try {

       /* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->createSceneManager(Ogre::ST_GENERIC, "MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

        /* Create camera object */
        camera_ = scene_manager->createCamera("MyCamera");
        Ogre::SceneNode* camera_scene_node = root_scene_node->createChildSceneNode("MyCameraNode");
        camera_scene_node->attachObject(camera_);

        /* Create viewport */
        Ogre::Viewport *viewport = ogre_window_->addViewport(camera_, viewport_z_order_g, viewport_left_g, viewport_top_g, viewport_width_g, viewport_height_g);

        viewport->setAutoUpdated(true);
        viewport->setBackgroundColour(viewport_background_color_g);

        float ratio = float(viewport->getActualWidth()) / float(viewport->getActualHeight());
        camera_->setAspectRatio(ratio);

        camera_->setNearClipDistance(camera_near_clip_distance_g);
        camera_->setFarClipDistance(camera_far_clip_distance_g); 

		camera_->setPosition(Ogre::Vector3(0.5, 0.5, 1.5));
		camera_->lookAt(Ogre::Vector3(0.0, 0.0, 0.0));
		camera_->setFixedYawAxis(true, Ogre::Vector3(0.0, 1.0, 0.0));
    

		camera_->setPosition(65.5, 100.5,148);
		camera_->lookAt(100,100,100);
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitFrameListener(void){

	try {

		/* Add this object as a FrameListener (see frameRenderingQueued event) */
		ogre_root_->addFrameListener(this);

	}
    catch (Ogre::Exception &e){
		throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
		throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitOIS(void){

	/* Initialize the Object Oriented Input System (OIS) */
	try {

		/* Initialize input manager */
		OIS::ParamList pl; // Parameter list passed to the input manager initialization
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;
		ogre_window_->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
		pl.insert(std::make_pair(std::string("w32_mouse"), 
		std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_mouse"), 
		std::string("DISCL_NONEXCLUSIVE")));
		input_manager_ = OIS::InputManager::createInputSystem(pl);

		/*size_t hWnd = 0;
		ogre_window_->getCustomAttribute("WINDOW", &hWnd);
		input_manager_ = OIS::InputManager::createInputSystem(hWnd);*/

		/* Initialize keyboard and mouse */
		keyboard_ = static_cast<OIS::Keyboard*>(input_manager_->createInputObject(OIS::OISKeyboard, false));

		mouse_ = static_cast<OIS::Mouse*>(input_manager_->createInputObject(OIS::OISMouse, false));
		unsigned int width, height, depth;
		int top, left;
		ogre_window_->getMetrics(width, height, depth, left, top);
		const OIS::MouseState &ms = mouse_->getMouseState();
		ms.width = width;
		ms.height = height;

	}
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::LoadMaterials(void){

    try {
		
		/* Load materials that can then be assigned to objects in the scene */
		Ogre::String resource_group_name = "MyGame";
		Ogre::ResourceGroupManager& resource_group_manager = Ogre::ResourceGroupManager::getSingleton();
		resource_group_manager.createResourceGroup(resource_group_name);
		bool is_recursive = false;
		resource_group_manager.addResourceLocation(material_directory_g, "FileSystem", resource_group_name, is_recursive);
		resource_group_manager.initialiseResourceGroup(resource_group_name);
		resource_group_manager.loadResourceGroup(resource_group_name);

	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitCompositor(void){

	try{
		
		material_listener_.Init(this);

		Ogre::CompositorInstance *inst = Ogre::CompositorManager::getSingleton().addCompositor(camera_->getViewport(), "ScreenSpaceEffect");
		inst->addListener(&material_listener_);
		inst->setEnabled(false);
		//Ogre::CompositorManager::getSingleton().setCompositorEnabled(camera_->getViewport(), "ScreenSpaceEffect", true);
		
		elapsed_time_ = 0;
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::CreateTorusGeometry(Ogre::String object_name, float loop_radius, float circle_radius, int num_loop_samples, int num_circle_samples){

    try {
		/* Create a torus and add it to the resource list
		   The torus is built from a large loop with small circles around the loop */

        /* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

        /* Create the 3D object */
        Ogre::ManualObject* object = NULL;
        object = scene_manager->createManualObject(object_name);
        object->setDynamic(false);

        /* Create triangle list for the object */
		object->begin("", Ogre::RenderOperation::OT_TRIANGLE_LIST);

		/* Add vertices to the object */
		float theta, phi; // Angles for circles
		Ogre::Vector3 loop_center;
		Ogre::Vector3 vertex_position;
		Ogre::Vector3 vertex_normal;
		Ogre::ColourValue vertex_color;
				
		for (int i = 0; i < num_loop_samples; i++){ // large loop
			
			theta = Ogre::Math::TWO_PI*i/num_loop_samples; // loop sample (angle theta)
			loop_center = Ogre::Vector3(loop_radius*cos(theta), loop_radius*sin(theta), 0); // centre of a small circle

			for (int j = 0; j < num_circle_samples; j++){ // small circle
				
				phi = Ogre::Math::TWO_PI*j/num_circle_samples; // circle sample (angle phi)
				
				/* Define position, normal and color of vertex */
				vertex_normal = Ogre::Vector3(cos(theta)*cos(phi), sin(theta)*cos(phi), sin(phi));
				vertex_position = loop_center + vertex_normal*circle_radius;
				/*Ogre::Vector3(loop_center.x + local_normal.x*circle_radius, 
				                loop_center.y + local_normal.y*circle_radius, 
				     			loop_center.z + local_normal.z*circle_radius);*/
				vertex_color = Ogre::ColourValue(1.0 - ((float) i / (float) num_loop_samples), 
				                                 (float) i / (float) num_loop_samples, 
				                                 (float) j / (float) num_circle_samples);

				/* Add them to the object */
				object->position(vertex_position);
				object->normal(vertex_normal);
				object->colour(vertex_color); 
			}
		}

		/* Add triangles to the object */
		for (int i = 0; i < num_loop_samples; i++){
			for (int j = 0; j < num_circle_samples; j++){
				// Two triangles per quad
				object->triangle(((i + 1) % num_loop_samples)*num_circle_samples + j, 
							     i*num_circle_samples + ((j + 1) % num_circle_samples),
								 i*num_circle_samples + j);
				object->triangle(((i + 1) % num_loop_samples)*num_circle_samples + j,
					             ((i + 1) % num_loop_samples)*num_circle_samples + ((j + 1) % num_circle_samples),
								 i*num_circle_samples + ((j + 1) % num_circle_samples));
			}
		}
		
		/* We finished the object */
        object->end();
		
        /* Convert triangle list to a mesh */
        object->convertToMesh(object_name);
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

void OgreApplication::CreateTorus(Ogre::String object_name, Ogre::String material_name, Ogre::ColourValue color,float x, float y, float z, float loop_radius, float circle_radius, int num_loop_samples, int num_circle_samples){

    try {
		/* Create a torus
		   The torus is built from a large loop with small circles around the loop */

        /* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

        /* Create the 3D object */
       
		 Ogre::ManualObject* object = NULL;
        object = scene_manager->createManualObject(object_name);
        object->setDynamic(false);

        /* Create triangle list for the object */
		object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_LIST);

		/* Add vertices to the object */
		float theta, phi; // Angles for circles
		Ogre::Vector3 loop_center;
		Ogre::Vector3 vertex_position;
		Ogre::Vector3 vertex_normal;
		Ogre::ColourValue vertex_color;
		Ogre::Vector2 texture_coord;


		for (int i = 0; i < num_loop_samples; i++){ // large loop
			
			theta = Ogre::Math::TWO_PI*i/num_loop_samples; // loop sample (angle theta)
			loop_center = Ogre::Vector3(loop_radius*cos(theta), loop_radius*sin(theta), 0); // centre of a small circle

			for (int j = 0; j < num_circle_samples; j++){ // small circle
				
				phi = Ogre::Math::TWO_PI*j/num_circle_samples; // circle sample (angle phi)
				
				/* Define position, normal and color of vertex */
		
					vertex_normal = Ogre::Vector3(cos(theta)*cos(phi), sin(theta)*cos(phi), sin(phi));
				vertex_position = loop_center + vertex_normal*circle_radius;
				/*Ogre::Vector3(loop_center.x + local_normal.x*circle_radius, 
				                loop_center.y + local_normal.y*circle_radius, 
				     			loop_center.z + local_normal.z*circle_radius);*/
				vertex_color = Ogre::ColourValue(1.0 - ((float) i / (float) num_loop_samples), 
				                                 (float) i / (float) num_loop_samples, 
				                                 (float) j / (float) num_circle_samples);

				texture_coord = Ogre::Vector2(((float)i)/((float)num_loop_samples), 1.0-((float)j)/((float)num_circle_samples));
				/*vertex_color = Ogre::ColourValue(0.0, 0.0, 1.0);*/

				/* Add them to the object */
				object->position(vertex_position);
				object->normal(vertex_normal);
				
				object->colour(color); 
				object->textureCoord(texture_coord);
			}
		}

		/* Add triangles to the object */
		for (int i = 0; i < num_loop_samples; i++){
			for (int j = 0; j < num_circle_samples; j++){
				// Two triangles per quad
				object->triangle(((i + 1) % num_loop_samples)*num_circle_samples + j, 
							     i*num_circle_samples + ((j + 1) % num_circle_samples),
								 i*num_circle_samples + j);
				object->triangle(((i + 1) % num_loop_samples)*num_circle_samples + j,
					             ((i + 1) % num_loop_samples)*num_circle_samples + ((j + 1) % num_circle_samples),
								 i*num_circle_samples + ((j + 1) % num_circle_samples));
			}
		}
		
		/* We finished the object */
        object->end();
		
        /* Convert triangle list to a mesh */
        object->convertToMesh(object_name);

        /* Create one instance of the torus (one entity) */
		/* The same object can have multiple instances or entities */
        //Ogre::Entity* entity = scene_manager->createEntity(object_name);

		/* Apply a material to the entity to give it color */
		/* We already did that above, so we comment it out here */
		/* entity->setMaterialName(material_name); */
		/* But, this call is useful if we have multiple entities with different materials */

		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position */
		
       // Ogre::SceneNode* scene_node = root_scene_node->createChildSceneNode(object_name);
       // scene_node->attachObject(entity);

        /* Position and rotate the entity with the scene node */
		//scene_node->rotate(Ogre::Vector3(0, 1, 0), Ogre::Degree(60));
		//scene_node->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(30));
        //scene_node->translate(0.0, 0.0, 0.0);
		
		//scene_node->scale(0.5, 0.5, 0.5);
	//	scene_node->translate(x, y, z);
		
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::CreateCylinder(Ogre::String object_name, Ogre::String material_name){

		/* Create a cube */

		/* Retrieve scene manager and root scene node */
		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();


   Ogre::Real cylinder_start=0.0f;
   Ogre::Real cylinder_length=10.0f;
   Ogre::Real cylinder_raduis=1.0f;
   const int cylinder_circle_resolution=120;
   int loop_count;

   Ogre::Degree theta =(Ogre::Degree)0;
   Ogre::Degree alpha =(Ogre::Degree)360/cylinder_circle_resolution;

   Ogre::Vector3 cylinder_circle1[cylinder_circle_resolution];
   Ogre::Vector3 cylinder_circle2[cylinder_circle_resolution];

   Ogre::Vector3  cylinder_circle1_center;
   Ogre::Vector3  cylinder_circle2_center;

   cylinder_circle1_center.x=cylinder_start;
   cylinder_circle1_center.y=0;
   cylinder_circle1_center.z=0;

   cylinder_circle2_center.x=cylinder_start+cylinder_length;
   cylinder_circle2_center.y=0;
   cylinder_circle2_center.z=0;

   for(int loop_count=0;loop_count<cylinder_circle_resolution;loop_count++)
   {
      theta=theta+alpha;
      cylinder_circle1[loop_count].x=cylinder_start;
      cylinder_circle1[loop_count].y=Ogre::Math::Sin(theta)*cylinder_raduis; 
      cylinder_circle1[loop_count].z=Ogre::Math::Cos(theta)*cylinder_raduis; 

      cylinder_circle2[loop_count].x=cylinder_start+cylinder_length;
      cylinder_circle2[loop_count].y=Ogre::Math::Sin(theta)*cylinder_raduis; 
      cylinder_circle2[loop_count].z=Ogre::Math::Cos(theta)*cylinder_raduis; 

   }

   Ogre::ManualObject* cylinder=scene_manager->createManualObject(object_name);

   //////////////cylinder left circular base of circle//////////////////////////
   cylinder->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_FAN);
   cylinder->colour(Ogre::ColourValue(1,0,0));
   cylinder->position(cylinder_circle1_center);
   // printf("%f\n",cylinder_circle1_center);
   for( loop_count=0;loop_count<cylinder_circle_resolution;loop_count++)
   {
      cylinder->position(cylinder_circle1[loop_count]);
   }
   cylinder->position(cylinder_circle1[0]);
   cylinder->end();

   //////////////cylinder curved surface//////////////////////////
   cylinder->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_LIST);
   cylinder->colour(Ogre::ColourValue(0.2,0.6,1.0));

   for( loop_count=0;loop_count<cylinder_circle_resolution-1;loop_count++)
   {  
      cylinder->position(cylinder_circle1[loop_count]);
      cylinder->position(cylinder_circle2[loop_count]);
      cylinder->position(cylinder_circle1[loop_count+1]);

      cylinder->position(cylinder_circle1[loop_count+1]);
      cylinder->position(cylinder_circle2[loop_count]);
      cylinder->position(cylinder_circle2[loop_count+1]);

   }
   cylinder->position(cylinder_circle1[loop_count]);
   cylinder->position(cylinder_circle2[loop_count]);
   cylinder->position(cylinder_circle1[0]);

   cylinder->position(cylinder_circle1[0]);
   cylinder->position(cylinder_circle2[loop_count]);
   cylinder->position(cylinder_circle2[0]);

   cylinder->end();

   //////////////cylinder right circular base of circle//////////////////////////
   cylinder->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_FAN);
   cylinder->colour(Ogre::ColourValue(0,1,0));
   cylinder->position(cylinder_circle2_center);
  // printf("%f\n",cylinder_circle2_center);
   for( loop_count=cylinder_circle_resolution-1;loop_count>0;--loop_count)
   {
      cylinder->position(cylinder_circle2[loop_count]);
   }
   cylinder->position(cylinder_circle2[cylinder_circle_resolution-1]);
   cylinder->end();


   scene_manager->getRootSceneNode()->attachObject(cylinder);
           Ogre::String mesh_name = object_name;
        cylinder->convertToMesh(mesh_name);
	

	
}




void OgreApplication::CreateEntity(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name){

	try {
		/* Create one instance of the torus (one entity) */
		/* The same object can have multiple instances or entities */

		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create entity */
        //Ogre::Entity* entity = scene_manager->createEntity(object_name);

		/* Apply a material to the entity to give it color */
		/* We already did that above, so we comment it out here */
		//entity->setMaterialName(material_name);
		/* But, this call is useful if we have multiple entities with different materials */

		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position */

		//Ogre::SceneNode* scene_node = root_scene_node->createChildSceneNode(entity_name);
        //scene_node->attachObject(entity);
		//scene_node->scale(0.5, 0.5, 0.5);


        /* Position and rotate the entity with the scene node */
		//scene_node->rotate(Ogre::Vector3(0, 1, 0), Ogre::Degree(60));
		//scene_node->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(30));
        //scene_node->translate(0.0, 0.0, 0.0);
		

		Ogre::String entity_name, prefix("Cylinder");
		if(Lazy){
		for (int i = 0; i < (NUM_ELEMENTS2); i++){
			/* Create entity */
			entity_name = prefix + Ogre::StringConverter::toString(i);
			Ogre::Entity *entity = scene_manager->createEntity(entity_name, "Cylinder");
			//entity->setMaterialName(material_name);
			/* Create a scene node for the entity */
			/* The scene node keeps track of the entity's position */
			cylinder_[i] = root_scene_node->createChildSceneNode(entity_name);
			cylinder_[i]->attachObject(entity);
		}
		
		entity_t = scene_manager->createEntity("Torus2","Torus");
		entity_t->setMaterialName(material_name);
		torus2 = root_scene_node->createChildSceneNode("Torus2");
		torus2->attachObject(entity_t);

		entity_t2 = scene_manager->createEntity("Torus3","Torus");
		entity_t2->setMaterialName("ShinyBlueMaterial");
		torus = root_scene_node->createChildSceneNode("Torus3");
		torus->attachObject(entity_t2);

		root_scene_node->removeChild(cylinder_[1]);
		root_scene_node->removeChild(cylinder_[2]);
		root_scene_node->removeChild(cylinder_[4]);
		root_scene_node->removeChild(cylinder_[5]);
		root_scene_node->removeChild(cylinder_[6]);
		root_scene_node->removeChild(cylinder_[7]);
		root_scene_node->removeChild(torus);
		root_scene_node->removeChild(torus2);

		cylinder_[3]->addChild(torus);
		cylinder_[3]->addChild(torus2);
		torus->addChild(cylinder_[1]);
		torus->addChild(cylinder_[2]);

		torus2->addChild(cylinder_[4]);
		torus2->addChild(cylinder_[5]);
		cylinder_[3]->addChild(cylinder_[6]);
		cylinder_[3]->addChild(cylinder_[7]);

		cylinder_[3]->setPosition( 100,100,100);
	

	cylinder_[1]->setPosition( 0,-0.5,0);
	cylinder_[1]->setScale(0.125,0.125,0.125);
	cylinder_[1]->rotate(Ogre::Quaternion(sqrt(0.5),0,0,sqrt(0.5)),Ogre::Node::TransformSpace::TS_PARENT);

	cylinder_[2]->setPosition( 0.5,0,0);
	cylinder_[2]->setScale(0.125,0.125,0.125);
	cylinder_[2]->rotate(Ogre::Quaternion(0,0,0,-sqrt(0.5)),Ogre::Node::TransformSpace::TS_PARENT);
	
	cylinder_[4]->setPosition( 0,-0.5,0);
	cylinder_[4]->setScale(0.125,0.125,0.125);
	cylinder_[4]->rotate(Ogre::Quaternion(sqrt(0.5),0,0,sqrt(0.5)),Ogre::Node::TransformSpace::TS_PARENT);

	cylinder_[5]->setPosition( 0.5,0,0);
	cylinder_[5]->setScale(0.125,0.125,0.125);
	cylinder_[5]->rotate(Ogre::Quaternion(0,0,0,-sqrt(0.5)),Ogre::Node::TransformSpace::TS_PARENT);
	
	cylinder_[6]->setScale(0.15,1.5,1.5);
	cylinder_[6]->setPosition( 10,0,0);
	cylinder_[7]->setScale(0.15,1.5,1.5);
	cylinder_[7]->setPosition( -1.5,0,0);

	torus->translate(2.5,0,0);
	torus->setScale(7.5,7.5,7.5);
	torus->yaw(Ogre::Degree(90));

	torus2->translate(7.5,0,0);
	torus2->setScale(7.5,7.5,7.5);
	torus2->yaw(Ogre::Degree(90));

		}
	

		//cylinder_[3]->setPosition(100+(timer2),100,100);
		//cylinder_[3]->roll(Ogre::Degree(-timer));

		torus->roll(Ogre::Degree(-1));
		torus2->roll(Ogre::Degree(-1));

		//Ogre::Vector3 vec(2.5,3.75,0);
		//cylinder_[1]->translate( vec);
		//cylinder_[1]->rotate(Ogre::Quaternion(0.000001,0,0,0.001),Ogre::Node::TransformSpace::TS_PARENT);
		//cylinder_[1]->translate( -vec);
		//cylinder_[1]->setFixedYawAxis(true);
		//cylinder_[1]->yaw(Ogre::Degree(1+timer2), Ogre::Node::TS_PARENT);

			/*tw = TranslationMatrix(Ogre::Vector3(0.4f, 0.0f, 0.0f)) * tw;
		tw = RotationMatrix(Ogre::Vector3(0.0, 0.0, 1.0), Ogre::Radian(0.9*sin(0.01*timer))) * tw;
		tw = RotationMatrix(Ogre::Vector3(1.0, 0.0, 0.0), Ogre::Radian(0.5*cos(0.013*timer))) * tw;
		tw = TranslationMatrix(Ogre::Vector3(-0.4f, 0.0f, 0.0f)) * tw;
		*/
		
		Lazy= false;
    }


	
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::MainLoop(void){

    try {

        /* Main loop to keep the application going */

        ogre_root_->clearEventTimes();

        while(!ogre_window_->isClosed()){
            ogre_window_->update(false);

            ogre_window_->swapBuffers();

            ogre_root_->renderOneFrame();

            Ogre::WindowEventUtilities::messagePump();
        }
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::SetupAnimation(Ogre::String object_name){

	/* Retrieve scene manager and root scene node */
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
	Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	/* Set up animation */
	Ogre::Real duration = Ogre::Math::TWO_PI;
	Ogre::Real num_steps = 36;
	Ogre::Real step = duration/num_steps;
	Ogre::Animation* animation = scene_manager->createAnimation("Animation", duration);
	animation->setInterpolationMode(Ogre::Animation::IM_LINEAR);
	Ogre::Node *object_scene_node = root_scene_node->getChild(object_name);
	Ogre::NodeAnimationTrack* track = animation->createNodeTrack(0, object_scene_node);
	
	/* Set up frames for animation */
	Ogre::TransformKeyFrame* key;
	Ogre::Quaternion quat;
	Ogre::Matrix3 rot1, rot2, rot3;
	rot1 = Ogre::Matrix3::IDENTITY;
	for (int i = 0; i < num_steps; i++){
		Ogre::Real current = ((float) i) * step;
		key = track->createNodeKeyFrame(current);
		//rot1.FromAngleAxis(Ogre::Vector3(1, 0, 0), Ogre::Degree(-90)); // Comment for Torus
		rot2.FromAngleAxis(Ogre::Vector3(0, 1, 0), Ogre::Radian(-current));
		rot3 = rot2 * rot1;
		quat.FromRotationMatrix(rot3);
		key->setRotation(quat);
		key->setScale(Ogre::Vector3(0.8, 0.8, 0.8)); // Uncomment for Torus
	}

	/* Create animation state */
	animation_state_ = scene_manager->createAnimationState("Animation");
	animation_state_->setEnabled(true);
	animation_state_->setLoop(true);

	/* Turn on animating flag */
	animating_ = true;
}


bool OgreApplication::frameEnded(const Ogre::FrameEvent &fe){

	/* Render scene to texture before rendering the current scene */

	return true;
}


bool OgreApplication::frameRenderingQueued(const Ogre::FrameEvent& fe){
  
	/* This event is called after a frame is queued for rendering */
	/* Do stuff in this event since the GPU is rendering and the CPU is idle */

	/* Keep animating if flag is on */
	if (animating_){
		animation_state_->addTime(fe.timeSinceLastFrame);
	}

	/* Capture input */
	keyboard_->capture();
	mouse_->capture();

	/* Handle specific key events */
	if (keyboard_->isKeyDown(OIS::KC_Q)){
		entity_t->setMaterialName("ShinyBlueMaterial");
		entity_t2->setMaterialName("ShinyBlueMaterial");
		qp = true;
	}
		if (keyboard_->isKeyDown(OIS::KC_E)){
		entity_t->setMaterialName("ShinyTextureMaterial");
		entity_t2->setMaterialName("ShinyTextureMaterial");
		ep = true;
	}




	if (keyboard_->isKeyDown(OIS::KC_SPACE)){
		space_down_ = true;
	}
	if ((!keyboard_->isKeyDown(OIS::KC_SPACE)) && space_down_){
		animating_ = !animating_;
		space_down_ = false;
	}
	if (keyboard_->isKeyDown(OIS::KC_ESCAPE)){
		animation_state_->setTimePosition(0);
	}
 
	// Update time for compositor
	elapsed_time_ += fe.timeSinceLastFrame;
		
    return true;
}


void MaterialListener::Init(OgreApplication *app){

	app_ = app;
}


void MaterialListener::notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat){

}


void MaterialListener::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat){

	// Update compositor material parameters
	Ogre::GpuProgramParametersSharedPtr params = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("time", (float)(((int)(app_->elapsed_time_*100.0)) % app_->ogre_window_->getHeight()));
}


} // namespace ogre_application;