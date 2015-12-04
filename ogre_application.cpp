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
int toon = 0;
bool keydown = false;
/* Viewport and camera settings */
float viewport_width_g = 0.95f;
float viewport_height_g = 0.95f;
float viewport_left_g = (1.0f - viewport_width_g) * 0.5f;
float viewport_top_g = (1.0f - viewport_height_g) * 0.5f;
unsigned short viewport_z_order_g = 100;
float camera_near_clip_distance_g = 0.01;
float camera_far_clip_distance_g = 100.0;
Ogre::Vector3 camera_position_g(0,0,-8);
Ogre::Vector3 camera_look_at_g(0.0, 0.0, 0.0);
Ogre::Vector3 camera_up_g(0.0, 1.0, 0.0);
const Ogre::ColourValue viewport_background_color_g(1.0, 0.0, 0.0);
int viewMode = 0;

//AI floats
float ai1_x =0, ai1_y =0, ai1_z =0;
float ai2_x =0, ai2_y =0, ai2_z =0;
float ai3_x =0, ai3_y =0, ai3_z =0;
float elapsed_time_AI=0;
float elapsed_time2_AI=0;
float elapsed_time3_AI=0;

/* Materials */
const Ogre::String material_directory_g = MATERIAL_DIRECTORY;

/* Spaceship variables */
Ogre::Vector3 ship_float(0.0, 0.0, 0.0);




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

	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	scene_manager->setSkyBox(true, "SkyBox",50, false);
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

		camera_->setPosition(camera_position_g);
		camera_->lookAt(camera_look_at_g);
		camera_->setFixedYawAxis(false);
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
		Ogre::Vector2 texture_coord;
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

				texture_coord = Ogre::Vector2(((float)i)/((float)num_loop_samples), 1.0-((float)j)/((float)num_circle_samples));
				/* Add them to the object */
				object->position(vertex_position);
				object->normal(vertex_normal);
				object->colour(vertex_color); 
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
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

void OgreApplication::CreateSphereGeometry(Ogre::String object_name, float radius, int num_samples_theta, int num_samples_phi){

    try {
		/* Create a sphere */

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
		float theta, phi; // Angles for parametric equation
		Ogre::Vector3 vertex_position;
		Ogre::Vector3 vertex_normal;
		Ogre::ColourValue vertex_color;
		Ogre::Vector2 texture_coord;
		
				
		for (int i = 0; i < num_samples_theta; i++){
			
			theta = Ogre::Math::TWO_PI*i/(num_samples_theta-1); // angle theta
			
			for (int j = 0; j < num_samples_phi; j++){
				
				phi = Ogre::Math::PI*j/(num_samples_phi-1); // angle phi
				
				/* Define position, normal and color of vertex */
				vertex_normal = Ogre::Vector3(cos(theta)*sin(phi), sin(theta)*sin(phi), -cos(phi));
				// We need z = -cos(phi) to make sure that the z coordinate runs from -1 to 1 as phi runs from 0 to pi
				// Otherwise, the normal will be inverted
				vertex_position = Ogre::Vector3(vertex_normal.x*radius, 
					                            vertex_normal.y*radius, 
												vertex_normal.z*radius),
				/*vertex_color = Ogre::ColourValue(1.0 - ((float) i / (float) num_samples_theta), 
				                                 (float) i / (float) num_samples_theta, 
				                                 (float) j / (float) num_samples_phi);*/
				vertex_color = Ogre::ColourValue(0.0, 0.0, 1.0);
				texture_coord = Ogre::Vector2(((float)i)/((float)num_samples_theta), 1.0-((float)j)/((float)num_samples_phi));
								
				/* Add them to the object */
				object->position(vertex_position);
				object->normal(vertex_normal);
				object->colour(vertex_color); 
				object->textureCoord(texture_coord);
			}
		}

		/* Add triangles to the object */
		for (int i = 0; i < num_samples_theta; i++){
			for (int j = 0; j < (num_samples_phi-1); j++){
				// Two triangles per quad
				object->triangle(((i + 1) % num_samples_theta)*num_samples_phi + j, 
									i*num_samples_phi + (j + 1),
									i*num_samples_phi + j);
				object->triangle(((i + 1) % num_samples_theta)*num_samples_phi + j, 
									((i + 1) % num_samples_theta)*num_samples_phi + (j + 1), 
									i*num_samples_phi + (j + 1));
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

void OgreApplication::CreateCube(void){

	try {
		/* Create a cube */

		/* Retrieve scene manager and root scene node */
		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create the 3D object */
        Ogre::ManualObject* object = NULL;
        Ogre::String object_name = "Cube";
        object = scene_manager->createManualObject(object_name);
        object->setDynamic(false);

        /* Create triangle list for the object */
		Ogre::String material_name = "ShinyBlue";
        object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_LIST);

		/* Vertices of a cube */
		Ogre::Vector3 v0(-0.5, -0.5,  0.5);
		Ogre::Vector3 v1( 0.5, -0.5,  0.5);
		Ogre::Vector3 v2( 0.5,  0.5,  0.5);
		Ogre::Vector3 v3(-0.5,  0.5,  0.5);
		Ogre::Vector3 v4(-0.5, -0.5, -0.5);
		Ogre::Vector3 v5( 0.5, -0.5, -0.5);
		Ogre::Vector3 v6( 0.5,  0.5, -0.5);
		Ogre::Vector3 v7(-0.5,  0.5, -0.5);

		/* Normal of each face of the cube */
		Ogre::Vector3 n0( 0.0,  0.0,  1.0);
		Ogre::Vector3 n1( 1.0,  0.0,  0.0);
		Ogre::Vector3 n2( 0.0,  0.0, -1.0);
		Ogre::Vector3 n3(-1.0,  0.0,  0.0);
		Ogre::Vector3 n4( 0.0,  1.0,  0.0);
		Ogre::Vector3 n5( 0.0, -1.0,  0.0);

		/* Cube's color */
		Ogre::ColourValue clr0(0.0, 0.0, 1.0);
		Ogre::ColourValue clr1(1.0, 0.0, 1.0);
		Ogre::ColourValue clr2(1.0, 1.0, 1.0);
		Ogre::ColourValue clr3(0.0, 1.0, 0.0);
		Ogre::ColourValue clr4(0.0, 0.0, 1.0);
		Ogre::ColourValue clr5(1.0, 0.0, 0.0);
		Ogre::ColourValue clr6(1.0, 1.0, 0.0);
		Ogre::ColourValue clr7(0.0, 1.0, 0.0);
		
		/* This construction only partially uses shared vertices, so that we can assign appropriate vertex normals
		   to each face */
		/* Each face of the cube is defined by four vertices (with the same normal) and two triangles */
		object->position(v0);
		object->normal(n0);
		object->textureCoord(0, 0);
		object->colour(clr0);

		object->position(v1);
		object->normal(n0);
		object->textureCoord(1, 0);
		object->colour(clr1);

		object->position(v2);
		object->normal(n0);
		object->textureCoord(1, 1);
		object->colour(clr2);

		object->position(v3);
		object->normal(n0);
		object->textureCoord(0, 1);
		object->colour(clr3);
		
		object->position(v1);
		object->normal(n1);
		object->textureCoord(0, 0);
		object->colour(clr1);

		object->position(v5);
		object->normal(n1);
		object->textureCoord(1, 0);
		object->colour(clr5);

		object->position(v6);
		object->normal(n1);
		object->textureCoord(1, 1);
		object->colour(clr6);

		object->position(v2);
		object->normal(n1);
		object->textureCoord(0, 1);
		object->colour(clr2);

		object->position(v5);
		object->normal(n2);
		object->textureCoord(0, 0);
		object->colour(clr5);

		object->position(v4);
		object->normal(n2);
		object->textureCoord(1, 0);
		object->colour(clr4);
		
		object->position(v7);
		object->normal(n2);
		object->textureCoord(1, 1);
		object->colour(clr7);

		object->position(v6);
		object->normal(n2);
		object->textureCoord(0, 1);
		object->colour(clr6);

		object->position(v4);
		object->normal(n3);
		object->textureCoord(0, 0);
		object->colour(clr4);

		object->position(v0);
		object->normal(n3);
		object->textureCoord(1, 0);
		object->colour(clr0);

		object->position(v3);
		object->normal(n3);
		object->textureCoord(1, 1);
		object->colour(clr3);

		object->position(v7);
		object->normal(n3);
		object->textureCoord(0, 1);
		object->colour(clr7);

		object->position(v3);
		object->normal(n4);
		object->textureCoord(0, 0);
		object->colour(clr3);

		object->position(v2);
		object->normal(n4);
		object->textureCoord(1, 0);
		object->colour(clr2);

		object->position(v6);
		object->normal(n4);
		object->textureCoord(1, 1);
		object->colour(clr6);

		object->position(v7);
		object->normal(n4);
		object->textureCoord(0, 1);
		object->colour(clr7);

		object->position(v1);
		object->normal(n5);
		object->textureCoord(0, 0);
		object->colour(clr1);

		object->position(v0);
		object->normal(n5);
		object->textureCoord(1, 0);
		object->colour(clr0);

		object->position(v4);
		object->normal(n5);
		object->textureCoord(1, 1);
		object->colour(clr4);

		object->position(v5);
		object->normal(n5);
		object->textureCoord(0, 1);
		object->colour(clr5);

		for (int i = 0; i < 6; i++){
			object->triangle(i*4 + 0, i*4 + 1, i*4 + 3);
			object->triangle(i*4 + 1, i*4 + 2, i*4 + 3);
		}
   
		/* We finished the object */
        object->end();
		
        /* Convert triangle list to a mesh */
        Ogre::String mesh_name = "Cube";
        object->convertToMesh(mesh_name);

	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

void OgreApplication::CreateEntity(Ogre::String entity_name, Ogre::String object_name, Ogre::String material_name){

	try {
		/* Create one instance of the torus (one entity) */
		/* The same object can have multiple instances or entities */

		/* Retrieve scene manager and root scene node */
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create entity */
        Ogre::Entity* entity = scene_manager->createEntity(object_name);

		/* Apply a material to the entity to give it color */
		/* We already did that above, so we comment it out here */
		entity->setMaterialName(material_name);
		/* But, this call is useful if we have multiple entities with different materials */

		/* Create a scene node for the entity */
		/* The scene node keeps track of the entity's position */
        Ogre::SceneNode* scene_node = root_scene_node->createChildSceneNode(entity_name);
        scene_node->attachObject(entity);

        /* Position and rotate the entity with the scene node */
		//scene_node->rotate(Ogre::Vector3(0, 1, 0), Ogre::Degree(60));
		//scene_node->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(30));
        //scene_node->translate(0.0, 0.0, 0.0);
		scene_node->scale(0.5, 0.5, 0.5);

	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

Ogre::SceneNode* OgreApplication::CreateModel_1(float x, float y, float z, int nm){

	try {
		// Retrieve scene manager and root scene node 
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		Ogre::Vector3 invScale;
		// Create entity 

		Ogre::String entity_name, prefix("Cube" + std::to_string(nm));
        for (int i = 0; i < (5); i++){
			// Create entity 
			entity_name = prefix + Ogre::StringConverter::toString(i);
			Ogre::Entity *entity = scene_manager->createEntity(entity_name, "Cube");

			//Change material
			if(i==0)
				entity->setMaterialName("Toon_One_Blue_Lighted"); // material for the main block
			else if(i == 1 || i == 2)
				entity->setMaterialName("Default_Blue_Light"); // material for the 2 side blocks
			else
				entity->setMaterialName("Toon_Four"); // materal for the 2 blocks inside the torus

			// Create a scene node for the entity 
			// The scene node keeps track of the entity's position 
			cube_[i] = root_scene_node->createChildSceneNode(entity_name);
			cube_[i]->attachObject(entity);
		}

		//torus entity name
		Ogre::String e_n = "Torus_1" + std::to_string(nm);
		Ogre::Entity* entity = scene_manager->createEntity(e_n, "TorusMesh");
		
		// Apply a material to the entity to give it color 
		entity->setMaterialName("Default_Blue_Light"); // material for the torus

		// The scene node keeps track of the entity's position 
		Ogre::SceneNode* torus1 = root_scene_node->createChildSceneNode(e_n);
		torus1->attachObject(entity);

		//Removing hierarchical connecting between nodes
		root_scene_node->removeChild(cube_[1]);
		root_scene_node->removeChild(cube_[2]);
		root_scene_node->removeChild(cube_[3]);
		root_scene_node->removeChild(cube_[4]);
		root_scene_node->removeChild(torus1);

		cube_[0]->addChild(cube_[1]);
		cube_[0]->addChild(cube_[2]);
		cube_[0]->addChild(torus1);
		torus1->addChild(cube_[3]);
		torus1->addChild(cube_[4]);

		// main block
		cube_[0]->scale(0.5, 0.1, 0.1);
		cube_[0]->translate(x,y,z);
		invScale = 1 / cube_[0]->getScale();

		for (int i = 1; i < 3; ++i)
			cube_[i]->scale(invScale);
		
		// side blocks
		cube_[1]->scale(0.2, 0.2, 0.12);
		cube_[1]->translate(0.5,0,0);

		cube_[2]->scale(0.2, 0.2, 0.12);
		cube_[2]->translate(-0.5,0,0);

		//blocks inside torus
		cube_[3]->scale(0.8, 0.25, 0.1);
		cube_[3]->translate(0,0,0);

		cube_[4]->scale(0.25, 0.8, 0.1);
		cube_[4]->translate(0,0,0);

		//torus
		torus1->scale(0.5, 2.5, 2.5);
		torus1->rotate(Ogre::Vector3(0,1,0), Ogre::Degree(90));
		torus1->translate(0,0,0);

		

		cube_[0]->translate(0,0,0);
		
		return cube_[0];
	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

Ogre::SceneNode* OgreApplication::CreateModel_Player(float x, float y, float z, int nm){
	try{
		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		Ogre::String entity_name, prefix("Player" + std::to_string(nm));

		Ogre::String parts[] = {"Body","WingOne", "WingTwo", "LeftBlaster", "RightBlaster"};
		for(int i = 0; i< 5; i++){
			entity_name = prefix + parts[i];
			Ogre::Entity * entity =  scene_manager->createEntity(entity_name, "Cube");
			if( i == 0){
				entity->setMaterialName("XBody");
			}else if(i < 3){
				entity->setMaterialName("XWings");
			}else{
				entity->setMaterialName("XBlasters");
			}
			cube_[i] = root_scene_node->createChildSceneNode(entity_name);
			cube_[i]->attachObject(entity);
		}

		//Remove existing hierarchy and implement our own
		for (int i = 1; i < 5; i++){
			root_scene_node->removeChild(cube_[i]);	
			cube_[0]->addChild(cube_[i]);
		}

		for(int i = 0; i<5; i++){
			cube_[i]->scale(.1,.1,.1);
		}
		
		//Body
		cube_[0]->scale(1,1,5);
		cube_[0]->translate(x,y,z);

		cube_[1]->setScale(6,.1,.3);
		cube_[2]->setScale(6,.1,.3);
		cube_[1]->rotate(Ogre::Vector3(0,0,1), Ogre::Degree(25));
		cube_[2]->rotate(Ogre::Vector3(0,0,1), Ogre::Degree(-25));
		cube_[1]->translate(0,0,-.345);
		cube_[2]->translate(0,0,-.345);

		cube_[3]->translate(0.45,-0.45,0.55);
		cube_[4]->translate(-0.45,-0.45,0.55);


		
		return cube_[0];
	}

	catch(Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

Ogre::SceneNode* OgreApplication::CreateModel_2(float x, float y, float z, int nm){
	Ogre::Vector3 invScale;
	int numCubes = 10;

	try {
		// Retrieve scene manager and root scene node 
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		// Create entity
		Ogre::String entity_name, prefix("Cube" + std::to_string(nm));

        for (int i = 0; i < numCubes; i++){
			// Create entity 
			entity_name = prefix + Ogre::StringConverter::toString(i);
			Ogre::Entity *entity = scene_manager->createEntity(entity_name, "Cube");

			//Change material
			if(i < 3)
				entity->setMaterialName("Default_Blue_Light"); // material for the main block
			else if(i < 6)
				entity->setMaterialName("Toon_One_Blue_Lighted"); // material for the 2 side blocks
			else
				entity->setMaterialName("Toon_Four"); // materal for the 2 blocks inside the torus

			// Create a scene node for the entity 
			// The scene node keeps track of the entity's position 
			cube_[i] = root_scene_node->createChildSceneNode(entity_name);
			cube_[i]->attachObject(entity);
		}

		//Removing hierarchical connection between nodes excluding cube 0
		for (int i = 1; i < numCubes; i++){
			root_scene_node->removeChild(cube_[i]);		
		}

		// --Main Body
		// Cube 0
		cube_[0]->scale(0.6, 1.2, 2.0);
		invScale = 1 / cube_[0]->getScale();

		cube_[0]->translate(x,y,z);

		cube_[0]->addChild(cube_[1]);
		cube_[0]->addChild(cube_[2]);
		cube_[0]->addChild(cube_[3]);
		
		// Cube 1 and 2
		cube_[1]->translate(-0.5, 0.0, 0.0);
		cube_[2]->translate(0.5, 0.0, 0.0);

		cube_[1]->scale(1.0, 0.7, 0.7);
		cube_[2]->scale(1.0, 0.7, 0.7);

		// Cube 3 
		cube_[3]->translate(0.0, -0.1, -0.5);

		cube_[3]->scale(invScale);
		cube_[3]->scale(0.8, 0.6, 0.5);

		// --Guns
		cube_[1]->addChild(cube_[4]);
		cube_[2]->addChild(cube_[5]);

		cube_[4]->addChild(cube_[6]);
		cube_[4]->addChild(cube_[8]);

		cube_[5]->addChild(cube_[7]);
		cube_[5]->addChild(cube_[9]);

		for (int i = 4; i < 10; ++i) {
			cube_[i]->scale(1/(cube_[1]->getScale()));
			cube_[i]->scale(1/(cube_[0]->getScale()));
		}

		// left gun
		cube_[4]->scale(1.5, 1.0, 3.0);
		cube_[4]->translate(-1.7, 0.0, -0.2);

		cube_[6]->scale(0.15, 0.3, 0.3);
		cube_[6]->translate(-0.3, 0.0, -0.55);

		cube_[8]->scale(0.15, 0.3, 0.3);
		cube_[8]->translate(0.3, 0.0, -0.55);

		// right gun
		cube_[5]->scale(1.5, 1.0, 3.0);
		cube_[5]->translate(1.7, 0.0, -0.2);

		cube_[7]->scale(0.15, 0.3, 0.3);
		cube_[7]->translate(-0.3, 0.0, -0.55);

		cube_[9]->scale(0.15, 0.3, 0.3);
		cube_[9]->translate(0.3, 0.0, -0.55);

		//Final scale of the ship
		cube_[0]->scale(0.2, 0.2, 0.2);

		return cube_[0];
	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

Ogre::SceneNode* OgreApplication::CreateModel_3(float x, float y, float z, int nm){
	const int numCubes = 7;
	const int numTorus = 2;
	Ogre::SceneNode* torus_[numTorus];

	try {
		// Retrieve scene manager and root scene node 
        Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
        Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		// Create entity
		Ogre::String entity_name, prefix("Cube" + std::to_string(nm));

        for (int i = 0; i < numCubes; i++){
			// Create entity 
			entity_name = prefix + Ogre::StringConverter::toString(i);
			Ogre::Entity *entity = scene_manager->createEntity(entity_name, "Cube");

			//Change material
			if(i < 4)
				entity->setMaterialName("Toon_One_Blue_Lighted"); // material for the connectors and engine
			else
				entity->setMaterialName("Default_Blue_Light"); // material for the guns

			// Create a scene node for the entity 
			// The scene node keeps track of the entity's position 
			cube_[i] = root_scene_node->createChildSceneNode(entity_name);
			cube_[i]->attachObject(entity);
		}

		//Removing hierarchical connecting between nodes excluding cube 0
		for (int i = 1; i < numCubes; i++){
			root_scene_node->removeChild(cube_[i]);		
		}

		prefix = "Torus" + std::to_string(nm);

		for (int i = 0; i < numTorus; i++){
			// Create entity 
			entity_name = prefix + Ogre::StringConverter::toString(i);
			Ogre::Entity *entity = scene_manager->createEntity(entity_name, "TorusMesh");

			//Change Material
			entity->setMaterialName("Toon_Four");

			// Create a scene node for the entity 
			// The scene node keeps track of the entity's position 
			torus_[i] = root_scene_node->createChildSceneNode(entity_name);
			torus_[i]->attachObject(entity);
		}

		//Removing hierarchical connection between torus nodes
		for (int i = 0; i < numTorus; i++){
			root_scene_node->removeChild(torus_[i]);		
		}

		cube_[0]->translate(x,y,z);
		
		//Center of ship
		cube_[0]->scale(2.0, 0.25, 0.25);

		for (int i = 1; i < numCubes; ++i)
			cube_[i]->scale(1/cube_[0]->getScale());

		cube_[0]->addChild(cube_[1]);
		cube_[1]->scale(0.75, 0.75, 2.0);
		cube_[1]->translate(-0.6, 0, 0);

		cube_[0]->addChild(cube_[6]);
		cube_[6]->scale(0.5, 0.5, 2.0);
		cube_[6]->translate(0.6, 0, -2.5);


		cube_[0]->addChild(torus_[0]);
		torus_[0]->scale(1/cube_[0]->getScale());
		torus_[0]->scale(1.0, 0.5, 1.0);
		torus_[0]->pitch(Ogre::Degree(45));

		cube_[0]->addChild(torus_[1]);
		torus_[1]->scale(1/cube_[0]->getScale());
		torus_[1]->scale(1.0, 0.5, 1.0);
		torus_[1]->pitch(Ogre::Degree(-45));

		//Right side of ship
		cube_[1]->addChild(cube_[2]);
		cube_[2]->scale(1/cube_[1]->getScale());
		cube_[2]->scale(0.25, 0.5, 0.25);
		cube_[2]->translate(0, 0.6, 0);

		cube_[1]->addChild(cube_[3]);
		cube_[3]->scale(1/cube_[1]->getScale());
		cube_[3]->scale(0.25, 0.5, 0.25);
		cube_[3]->translate(0, -0.6, 0);

		cube_[2]->addChild(cube_[4]);
		cube_[4]->scale(1/cube_[4]->_getDerivedScale());
		cube_[4]->scale(0.5, 0.5, 2.0);
		cube_[4]->translate(0, 0.6, -2.5);

		cube_[3]->addChild(cube_[5]);
		cube_[5]->scale(1/cube_[5]->_getDerivedScale());
		cube_[5]->scale(0.5, 0.5, 2.0);
		cube_[5]->translate(0, -0.6, -2.5);

		//Final scale of the ship
		cube_[0]->scale(Ogre::Vector3(0.5));

		return cube_[0];
	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

void OgreApplication::setCameraMode(int num) {
	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
	Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
	Ogre::Camera* camera = scene_manager->getCamera("MyCamera");
	Ogre::Node* cameraNode = camera->getParentNode();
	Ogre::Vector3 pos = camera->getPosition(), dir = camera->getDirection(), up = camera->getUp(), newPos;

	if (num == 0) { //First setting up camera (Default to first person)
		newPos = (pos + 2*dir);
		Ogre::SceneNode* player = CreateModel_Player(newPos.x, newPos.y, newPos.z, 1);
		root_scene_node->removeChild(player);
		cameraNode->addChild(player);
		++viewMode;
	} 
	else if (num == 1) { //First person mode
		cameraNode->getChild("Player1Body")->setOrientation(camera->getOrientation());
		cameraNode->getChild("Player1Body")->yaw(Ogre::Degree(180));
		cameraNode->getChild("Player1Body")->setPosition(pos - dir*0.15);
	} 
	else if (num == 2) { //Third person mode
		cameraNode->getChild("Player1Body")->setOrientation(camera->getOrientation());
		cameraNode->getChild("Player1Body")->yaw(Ogre::Degree(180));
		cameraNode->getChild("Player1Body")->setPosition(pos + dir*0.75 - up*0.15);
	} 
	//if num == 3, then cmera stops following

	else if (num == 4) { //Bring camera back to ship
		camera->setPosition(cameraNode->getChild("Player1Body")->getPosition());
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
  
	Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
	Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();
	//Ogre::SceneNode *Torus = scene_manager->getSceneNode("TorusEnt1");
	/* For the camera */
	Ogre::Camera* camera = scene_manager->getCamera("MyCamera");
	Ogre::SceneNode* cameraNode = camera->getParentSceneNode();
	Ogre::Quaternion qOld = camera->getDerivedOrientation();
	Ogre::Vector3 pos = camera->getPosition(), forw, right, up;
	forw = qOld * Ogre::Vector3(0,0,-1);
	up = qOld * Ogre::Vector3(0,1,0);
	right = forw.crossProduct(up);

	if (!camera){
		return false;
	}

	small1.run();

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
	if (keyboard_->isKeyDown(OIS::KC_ESCAPE)){
        ogre_root_->shutdown();
        ogre_window_->destroy();
        return false;
    }

	if (keyboard_->isKeyDown(OIS::KC_SPACE)){
		space_down_ = true;
	}
	if ((!keyboard_->isKeyDown(OIS::KC_SPACE)) && space_down_){
		animating_ = !animating_;
		space_down_ = false;
	}
	if (keyboard_->isKeyDown(OIS::KC_V)){
		animation_state_->setTimePosition(0);
	}
	if (!keyboard_->isKeyDown(OIS::KC_C) && keydown == true){
		keydown = false;
	}
	if (keyboard_->isKeyDown(OIS::KC_C) && keydown == false){
		keydown = true;
		if(toon == 0){
			//Ogre::Entity* mEntity = static_cast<Ogre::Entity*>(Torus->getAttachedObject(0));
			//mEntity->setMaterialName("Toon_One_Blue_Lighted");
			toon+=1;
		}else if(toon == 1){
			//Ogre::Entity* mEntity = static_cast<Ogre::Entity*>(Torus->getAttachedObject(0));
			//mEntity->setMaterialName("Toon_Two_Blue_Lighted");
			toon+=1;
		}else{
			//Ogre::Entity* mEntity = static_cast<Ogre::Entity*>(Torus->getAttachedObject(0));
			//mEntity->setMaterialName("Default_Blue_Light");
			toon=0;
		}
	}
 
	// Update time for compositor
	elapsed_time_ += fe.timeSinceLastFrame;
	//printf("%f\n",elapsed_time_);

	

	//AI for model1
	elapsed_time_AI += fe.timeSinceLastFrame;
	int random, max_value = 10, min_value = 1;

	if(elapsed_time_AI > 3){
	random = rand() % max_value + min_value;
	if(random % 2 )
		ai1_x = random;
	else
		ai1_x = -random;
	
	random = rand() % max_value + min_value;
	if(random % 2 )
		ai1_y = random;
	else
		ai1_y = -random;

	random = rand() % max_value + min_value;
	if(random % 2 )
		ai1_z = random;
	else
		ai1_z = -random;

	elapsed_time_AI =0;
	}

	Ogre::SceneNode *AI_1 = scene_manager->getSceneNode("Cube10");
	AI_1->translate(ai1_x/1000,ai1_y/1000,ai1_z/1000);

	//AI for model2
	Ogre::SceneNode *AI_2 = scene_manager->getSceneNode("Cube20");
	elapsed_time2_AI += fe.timeSinceLastFrame;
	int max_value2 = 10, min_value2 = 1;


	if(elapsed_time2_AI > 3){
	random = rand() % max_value2 + min_value2;
	if(random % 2 )
		ai2_x = random;
	else
		ai2_x = -random;
	
	random = rand() % max_value2 + min_value2;
	if(random % 2 )
		ai2_y = random;
	else
		ai2_y = -random;

	random = rand() % max_value2 + min_value2;
	if(random % 2 )
		ai2_z = random;
	else
		ai2_z = -random;

	elapsed_time2_AI =0;
	}

	AI_2->translate(ai2_x/1000,ai2_y/1000,ai2_z/1000);
	

	//AI for model3
	Ogre::SceneNode *AI_3 = scene_manager->getSceneNode("Cube30");
	elapsed_time3_AI += fe.timeSinceLastFrame;
	int  max_value3 = 10, min_value3 = 1;

	if(elapsed_time3_AI > 3){
	random = rand() % max_value3 + min_value3;
	if(random % 2 )
		ai3_x = random;
	else
		ai3_x = -random;
	
	random = rand() % max_value3 + min_value3;
	if(random % 2 )
		ai3_y = random;
	else
		ai3_y = -random;

	random = rand() % max_value3 + min_value3;
	if(random % 2 )
		ai3_z = random;
	else
		ai3_z = -random;

	elapsed_time3_AI =0;
	}

	AI_3->translate(ai3_x/1000,ai3_y/1000,ai3_z/1000);

	//////////////////////////////////////////////////////////////////

	// Collision Detection
	Ogre::SceneNode *player_c = scene_manager->getSceneNode("Player1Body");
	Ogre::Real distance1= player_c->getPosition().distance(AI_1->getPosition());
	Ogre::Real distance2= player_c->getPosition().distance(AI_2->getPosition());

	Ogre::Real distance3= player_c->getPosition().distance(AI_3->getPosition());

	if(distance1 < 1)
		printf("collide1");

	if(distance2 < 1)
		printf("collide2");

	if(distance3 < 1)
		printf("collide3");

	/* Move ship according to keyboard input and last move */
	/* Movement factors to apply to the ship */
	double trans_factor = 0.1; // Small continuous translation
	double small_trans_factor = 1.0; // Translation applied with thrusters
	Ogre::Radian rot_factor(Ogre::Math::PI / 90); // Camera rotation with directional thrusters
	
	/* Always keep the camera moving slightly */
	//camera->setPosition(camera->getPosition() + ship_float*small_trans_factor);

	/* Apply user commands */
	/* Camera rotation */
	if (keyboard_->isKeyDown(OIS::KC_UP)){
		qOld = Ogre::Quaternion(rot_factor, right) * qOld;
		qOld.normalise();
	}
	
	if (keyboard_->isKeyDown(OIS::KC_DOWN)){
		qOld = Ogre::Quaternion(-rot_factor, right) * qOld;
		qOld.normalise();
	}

	if (keyboard_->isKeyDown(OIS::KC_LEFT)){
		qOld = Ogre::Quaternion(rot_factor, up) * qOld;
		qOld.normalise();
	}

	if (keyboard_->isKeyDown(OIS::KC_RIGHT)){
		qOld = Ogre::Quaternion(-rot_factor, up) * qOld;
		qOld.normalise();
	}

	if (keyboard_->isKeyDown(OIS::KC_Q)){
		qOld = Ogre::Quaternion(-rot_factor, forw) * qOld;
		qOld.normalise();
	}

	if (keyboard_->isKeyDown(OIS::KC_E)){
		qOld = Ogre::Quaternion(rot_factor, forw) * qOld;
		qOld.normalise();
	}
	camera->setOrientation(qOld);

	/* Camera translation */
	if (keyboard_->isKeyDown(OIS::KC_W)){
		camera->setPosition(pos + forw*trans_factor);
		last_dir_ = Direction::Forward;
		ship_float = forw;
	}

	if (keyboard_->isKeyDown(OIS::KC_S)){
		camera->setPosition(pos - forw*trans_factor);
		last_dir_ = Direction::Backward;
		ship_float = -forw;
	}

	if (keyboard_->isKeyDown(OIS::KC_A)){
        camera->setPosition(pos - right*trans_factor);
		last_dir_ = Direction::Left;
		ship_float = -right;
	}
	
	if (keyboard_->isKeyDown(OIS::KC_D)){
		camera->setPosition(pos + right*trans_factor);
		last_dir_ = Direction::Right;
		ship_float = right;
	}

	if (keyboard_->isKeyDown(OIS::KC_R)){
        camera->setPosition(pos + up*trans_factor);
		last_dir_ = Direction::Up;
		ship_float = up;
	}
	
	if (keyboard_->isKeyDown(OIS::KC_F)){
        camera->setPosition(pos - up*trans_factor);
		last_dir_ = Direction::Down;
		ship_float = -up;
	}

	//Change the view mode
	if (keyboard_->isKeyDown(OIS::KC_F1))
		viewMode = 1;
	if (keyboard_->isKeyDown(OIS::KC_F2))
		viewMode = 2;
	if (keyboard_->isKeyDown(OIS::KC_F3))
		viewMode = 3;
	if (keyboard_->isKeyDown(OIS::KC_F4))
		viewMode = 4;

	setCameraMode(viewMode);
		
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