//==============================================================================
/*
Software License Agreement (BSD License)
Copyright (c) 2003-2016, CHAI3D.
(www.chai3d.org)

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following
disclaimer in the documentation and/or other materials provided
with the distribution.

* Neither the name of CHAI3D nor the names of its contributors may
be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

\author    <http://www.chai3d.org>
\author    Francois Conti
\version   3.2.0 $Rev: 1659 $
*/
//==============================================================================

//------------------------------------------------------------------------------
#include "chai3d.h"
//------------------------------------------------------------------------------
#include <GLFW/glfw3.h>
//------------------------------------------------------------------------------
#include "COculus.h"
//------------------------------------------------------------------------------
using namespace chai3d;
using namespace std;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// CHAI3D
//------------------------------------------------------------------------------

// a world that contains all objects of the virtual environment
cWorld* world;

// a camera to render the world in the window display
cCamera* camera;

// a light source
//cDirectionalLight *light;
cSpotLight* light;

// a haptic device handler
cHapticDeviceHandler* handler;

// a pointer to the current haptic device
cGenericHapticDevicePtr hapticDevice;

// a virtual tool representing the haptic device in the scene
cToolCursor* tool;

// indicates if the haptic simulation currently running
bool simulationRunning = false;

// indicates if the haptic simulation has terminated
bool simulationFinished = false;

// frequency counter to measure the simulation haptic rate
cFrequencyCounter frequencyCounter;

// haptic thread
cThread *hapticsThread;

// Organelles
cMultiMesh* centriole1;
cMultiMesh* centriole2;
cMultiMesh* chromatin;
cMultiMesh* cytoplasmInside;
cMultiMesh* cytoplasmOutside;
cMultiMesh* golgi;
//cMultiMesh* golgiVesicle;
//cMultiMesh* golgiVesicle0;
//cMultiMesh* golgiVesicle1;
//cMultiMesh* golgiVesicle2;
//cMultiMesh* golgiVesicle3;
//cMultiMesh* golgiVesicle4;
//cMultiMesh* golgiVesicle5;
//cMultiMesh* golgiVesicle6;
//cMultiMesh* golgiVesicle7;
//cMultiMesh* golgiVesicle8;
//cMultiMesh* golgiVesicle9;
cMultiMesh* lysosome1;
cMultiMesh* lysosome2;
cMultiMesh* mitochondria1;
cMultiMesh* mitochondria2;
cMultiMesh* mitochondria3;
cMultiMesh* mitochondria4;
cMultiMesh* mitochondria5;
cMultiMesh* mitochondria6;
cMultiMesh* mitochondria7;
cMultiMesh* mitochondriaOuter1;
cMultiMesh* mitochondriaOuter2;
cMultiMesh* mitochondriaOuter3;
cMultiMesh* mitochondriaOuter4;
cMultiMesh* mitochondriaOuter5;
cMultiMesh* mitochondriaOuter6;
cMultiMesh* mitochondriaOuter7;
cMultiMesh* nuclearEnvelope;
cMultiMesh* nuclearSlice;
cMultiMesh* nuclearSliceInner;
cMultiMesh* nucleolus;
cMultiMesh* peroxisome1;
cMultiMesh* peroxisome2;
cMultiMesh* peroxisome3;
cMultiMesh* peroxisome4;
cMultiMesh* peroxisome5;
cMultiMesh* peroxisome6;
cMultiMesh* ribosome1;
cMultiMesh* ribosome2;
cMultiMesh* ribosome3;
cMultiMesh* ribosome4;
cMultiMesh* ribosome5;
cMultiMesh* ribosome6;
cMultiMesh* ribosome7;
cMultiMesh* ribosome8;
cMultiMesh* ribosome9;
cMultiMesh* ribosome10;
cMultiMesh* ribosome11;
cMultiMesh* roughER;
cMultiMesh* smoothER;

// label for organelle description
cLabel* labelOrganelle;

// a level for the label of the organelle
cLevel* levelOrganelle;

// a font for rendering the text
cFontPtr font;

// description of organelles
const string textMembrane;
const string textSmoothER = "Smooth Endoplasmic Reticulum:\n A system of membranes enclosing\n a fluid filled space,\n that synthesises and\n processes lipids.";
const string textRoughER = "Rough Endoplasmic Reticulum:\nCovered in ribosomes,\n folds, transports and processes\n proteins that have been\n made at ribosomes.\n 2µm per layer.";
const string textGolgi = "Golgi Aparatus:\n A group of fluid-filled\n sacs that process, transport and\n package new lipids and proteins,\n also makes lysosomes.\n 1µm per layer.";
const string textNucleus = "Nucleus:\n Large organelle surrounded\n by nuclear envelope, containing many\n pores. This structure contains\n chromatin and nucleolus,\n and is where genetic information\n is stored. The nucleolus\n makes ribosomes, and therefore\n the nucleus is the site for\n RNA synthesis. 10µm.";
const string textMitochondria = "Mitochondria:\n Oval shaped, with double membrane;\n inner membrane folded to form\n structures called cristae,\n inside is matrix containing enzymes\n required for aerobic respiration.\n There are large numbers in cells,\n they are very active and\n require lots of energy,\n they also always contain DNA.\n 2µm";
const string textRibosome = "Ribosome:\n Small organelles either\n floating in the cytoplasm or\n attached to the rough ER;\n Site where proteins are\n made. 0.025µm";
//const string textCytoplasm = "Cytoplasm:\n The site of chemical reactions,\n controlled by enzymes.";
const string textCytoplasm = "Plasma Membrane:\n Composed mainly of phospholipids\n and proteins and regulates\n the movement of substances\n into and out of the cell";
const string textLysosome = "Lysosome:\n Round organelles surrounded by\n a membrane that holds digestive\n enzymes and can digest invading\n cells or break down components\n within the cell.\n 1µm.";;
const string textPeroxisome = "Peroxisome";
const string textCentriole = "Centriole";
const string textChromatin = "Chromatin";

// location of the label
cVector3d labelPos(800.0, 1000.0, 1000.0);
//------------------------------------------------------------------------------
// OCULUS RIFT
//------------------------------------------------------------------------------

// display context
cOVRRenderContext renderContext;

// oculus device
cOVRDevice oculusVR;


//------------------------------------------------------------------------------
// DECLARED MACROS
//------------------------------------------------------------------------------

// convert to resource path
#define RESOURCE_PATH(p)    (char*)((resourceRoot+string(p)).c_str())


//------------------------------------------------------------------------------
// DECLARED FUNCTIONS
//------------------------------------------------------------------------------

// callback when an error GLFW occurs
void errorCallback(int error, const char* a_description);

// callback when a key is pressed
void keyCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods);

// function that closes the application
void close(void);

// main haptics simulation loop
void updateHaptics(void);



//==============================================================================
/*
DEMO:   Haptic CEll

This is a demo of a haptic animal cell where the student can interact with a
static and rigid virtual cell model.
*/
//==============================================================================

int main(int argc, char **argv)
{
	//--------------------------------------------------------------------------
	// INITIALIZATION
	//--------------------------------------------------------------------------

	cout << endl;
	cout << "-----------------------------------" << endl;
	cout << "CHAI3D" << endl;
	cout << "Demo: 06-object" << endl;
	cout << "Copyright 2003-2016" << endl;
	cout << "-----------------------------------" << endl << endl << endl;
	cout << "Keyboard Options:" << endl << endl;
	cout << "[ ] - Recenter view point" << endl;
	cout << "[q] - Exit application" << endl;
	cout << endl << endl;

	// parse first arg to try and locate resources
	string resourceRoot = string(argv[0]).substr(0, string(argv[0]).find_last_of("/\\") + 1);


	//--------------------------------------------------------------------------
	// SETUP DISPLAY CONTEXT
	//--------------------------------------------------------------------------

	// initialize GLFW library
	if (!glfwInit())
	{
		cout << "failed initialization" << endl;
		cSleepMs(1000);
		return 1;
	}

	// set error callback
	glfwSetErrorCallback(errorCallback);

	// set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// create display context
	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	if (!window)
	{
		cout << "failed to create window" << endl;
		cSleepMs(1000);
		glfwTerminate();
		return 1;
	}

	// set key callback
	glfwSetKeyCallback(window, keyCallback);

	// set current display context
	glfwMakeContextCurrent(window);

	// sets the swap interval for the current display context
	glfwSwapInterval(0);

#ifdef GLEW_VERSION
	// initialize GLEW library
	if (glewInit() != GLEW_OK)
	{
		oculusVR.destroyVR();
		renderContext.destroy();
		glfwTerminate();
		return 1;
	}
#endif

	// initialize oculus
	if (!oculusVR.initVR())
	{
		cout << "failed to initialize Oculus" << endl;
		cSleepMs(1000);
		glfwTerminate();
		return 1;
	}

	// get oculus display resolution
	ovrSizei hmdResolution = oculusVR.getResolution();

	// setup mirror display on computer screen
	ovrSizei windowSize = { hmdResolution.w / 2, hmdResolution.h / 2 };

	// inialize buffers
	if (!oculusVR.initVRBuffers(windowSize.w, windowSize.h))
	{
		cout << "failed to initialize Oculus buffers" << endl;
		cSleepMs(1000);
		oculusVR.destroyVR();
		renderContext.destroy();
		glfwTerminate();
		return 1;
	}

	// set window size
	glfwSetWindowSize(window, windowSize.w, windowSize.h);



	//--------------------------------------------------------------------------
	// WORLD - CAMERA - LIGHTING
	//--------------------------------------------------------------------------

	// create a new world.
	world = new cWorld();

	// set the background color of the environment
	// the color is defined by its (R,G,B) components.
	world->m_backgroundColor.setWhite();

	// create a camera and insert it into the virtual world
	camera = new cCamera(world);
	world->addChild(camera);

	// position and orient the camera
	camera->set(cVector3d(0.7, 0.0, 0.4),    // camera position (eye)
		cVector3d(0.0, 0.0, 0.0),    // lookat position (target)
		cVector3d(0.0, 0.0, 1.0));   // direction of the "up" vector

									 // set the near and far clipping planes of the camera
									 // anything in front/behind these clipping planes will not be rendered
	camera->setClippingPlanes(0.01, 20.0);

	// enable multipass rendering for better quality;
	camera->setUseMultipassTransparency(true);

	// create a light source
	//light = new cDirectionalLight(world);
	light = new cSpotLight(world);

	// attach light to camera
	camera->addChild(light);

	// enable light source
	light->setEnabled(true);

	//// define the direction of the light beam
	//light->setDir(-3.0, -0.5, 0.0);

	//// set lighting conditions
	//light->m_ambient.set(0.4f, 0.4f, 0.4f);
	//light->m_diffuse.set(0.8f, 0.8f, 0.8f);
	//light->m_specular.set(1.0f, 1.0f, 1.0f);

	light->setLocalPos(1.0, 0.0, 1.0);

	// define the direction of the light beam
	light->setDir(-1.0, 0.0, -1.0);

	// enable this light source to generate shadows
	light->setShadowMapEnabled(true);

	// (3) set the resolution of the shadow map - high quality  
	light->m_shadowMap->setQualityMedium();

	// set light cone half angle
	light->setCutOffAngleDeg(30);


	//--------------------------------------------------------------------------
	// HAPTIC DEVICES / TOOLS
	//--------------------------------------------------------------------------

	// create a haptic device handler
	handler = new cHapticDeviceHandler();

	// get access to the first available haptic device
	handler->getDevice(hapticDevice, 0);

	// retrieve information about the current haptic device
	cHapticDeviceInfo hapticDeviceInfo = hapticDevice->getSpecifications();

	// if the device has a gripper, enable the gripper to simulate a user switch
	hapticDevice->setEnableGripperUserSwitch(true);

	// create a 3D tool and add it to the world
	tool = new cToolCursor(world);
	camera->addChild(tool);

	// set position
	tool->setLocalPos(-1.0, 0.0, 0.0);

	// connect the haptic device to the tool
	tool->setHapticDevice(hapticDevice);

	// define the radius of the tool (sphere)
	double toolRadius = 0.02;

	// define a radius for the tool
	tool->setRadius(toolRadius);

	// enable if objects in the scene are going to rotate of translate
	// or possibly collide against the tool. If the environment
	// is entirely static, you can set this parameter to "false"
	tool->enableDynamicObjects(true);

	// map the physical workspace of the haptic device to a larger virtual workspace.
	tool->setWorkspaceRadius(1.0);

	// haptic forces are enabled only if small forces are first sent to the device;
	// this mode avoids the force spike that occurs when the application starts when 
	// the tool is located inside an object for instance. 
	tool->setWaitForSmallForce(true);

	// start the haptic tool
	tool->start();


	//--------------------------------------------------------------------------
	// CREATE OBJECT
	//--------------------------------------------------------------------------

	// read the scale factor between the physical workspace of the haptic
	// device and the virtual workspace defined for the tool
	double workspaceScaleFactor = tool->getWorkspaceScaleFactor();

	// stiffness properties
	double maxStiffness = hapticDeviceInfo.m_maxLinearStiffness / workspaceScaleFactor;

	// create a virtual mesh
	centriole1 = new cMultiMesh();
	centriole2 = new cMultiMesh();
	chromatin = new cMultiMesh();
	cytoplasmOutside = new cMultiMesh();
	golgi = new cMultiMesh();
	//golgiVesicle = new cMultiMesh();
	//golgiVesicle0 = new cMultiMesh();
	//golgiVesicle1 = new cMultiMesh();
	//golgiVesicle2 = new cMultiMesh();
	//golgiVesicle3 = new cMultiMesh();
	//golgiVesicle4 = new cMultiMesh();
	//golgiVesicle5 = new cMultiMesh();
	//golgiVesicle6 = new cMultiMesh();
	//golgiVesicle7 = new cMultiMesh();
	//golgiVesicle8 = new cMultiMesh();
	//golgiVesicle9 = new cMultiMesh();
	lysosome1 = new cMultiMesh();
	lysosome2 = new cMultiMesh();
	mitochondria1 = new cMultiMesh();
	mitochondria2 = new cMultiMesh();
	mitochondria3 = new cMultiMesh();
	mitochondria4 = new cMultiMesh();
	mitochondria5 = new cMultiMesh();
	mitochondria6 = new cMultiMesh();
	mitochondria7 = new cMultiMesh();
	mitochondriaOuter1 = new cMultiMesh();
	mitochondriaOuter2 = new cMultiMesh();
	mitochondriaOuter3 = new cMultiMesh();
	mitochondriaOuter4 = new cMultiMesh();
	mitochondriaOuter5 = new cMultiMesh();
	mitochondriaOuter6 = new cMultiMesh();
	mitochondriaOuter7 = new cMultiMesh();
	nuclearEnvelope = new cMultiMesh();
	nuclearSlice = new cMultiMesh();
	nuclearSliceInner = new cMultiMesh();
	nucleolus = new cMultiMesh();
	peroxisome1 = new cMultiMesh();
	peroxisome2 = new cMultiMesh();
	peroxisome3 = new cMultiMesh();
	peroxisome4 = new cMultiMesh();
	peroxisome5 = new cMultiMesh();
	peroxisome6 = new cMultiMesh();
	ribosome1 = new cMultiMesh();
	ribosome2 = new cMultiMesh();
	ribosome3 = new cMultiMesh();
	ribosome4 = new cMultiMesh();
	ribosome5 = new cMultiMesh();
	ribosome6 = new cMultiMesh();
	ribosome7 = new cMultiMesh();
	ribosome8 = new cMultiMesh();
	ribosome9 = new cMultiMesh();
	ribosome10 = new cMultiMesh();
	ribosome11 = new cMultiMesh();
	roughER = new cMultiMesh();
	smoothER = new cMultiMesh();

	// add object to world
	world->addChild(centriole1);
	world->addChild(centriole1);
	world->addChild(chromatin);
	world->addChild(cytoplasmOutside);
	world->addChild(golgi);
	//world->addChild(golgiVesicle);
	//world->addChild(golgiVesicle0);
	//world->addChild(golgiVesicle1);
	//world->addChild(golgiVesicle2);
	//world->addChild(golgiVesicle3);
	//world->addChild(golgiVesicle4);
	//world->addChild(golgiVesicle5);
	//world->addChild(golgiVesicle6);
	//world->addChild(golgiVesicle7);
	//world->addChild(golgiVesicle8);
	//world->addChild(golgiVesicle9);
	world->addChild(lysosome1);
	world->addChild(lysosome2);
	world->addChild(mitochondria1);
	world->addChild(mitochondria2);
	world->addChild(mitochondria3);
	world->addChild(mitochondria4);
	world->addChild(mitochondria5);
	world->addChild(mitochondria6);
	world->addChild(mitochondria7);
	world->addChild(mitochondriaOuter1);
	world->addChild(mitochondriaOuter2);
	world->addChild(mitochondriaOuter3);
	world->addChild(mitochondriaOuter4);
	world->addChild(mitochondriaOuter5);
	world->addChild(mitochondriaOuter6);
	world->addChild(mitochondriaOuter7);
	world->addChild(nuclearEnvelope);
	world->addChild(nuclearSlice);
	world->addChild(nuclearSliceInner);
	world->addChild(nucleolus);
	world->addChild(peroxisome1);
	world->addChild(peroxisome2);
	world->addChild(peroxisome3);
	world->addChild(peroxisome4);
	world->addChild(peroxisome5);
	world->addChild(peroxisome6);
	world->addChild(ribosome1);
	world->addChild(ribosome2);
	world->addChild(ribosome3);
	world->addChild(ribosome4);
	world->addChild(ribosome5);
	world->addChild(ribosome6);
	world->addChild(ribosome7);
	world->addChild(ribosome8);
	world->addChild(ribosome9);
	world->addChild(ribosome10);
	world->addChild(ribosome11);
	world->addChild(roughER);
	world->addChild(smoothER);

	centriole1->m_name = "centriole";
	centriole2->m_name = "centriole";
	chromatin->m_name = "chromatin";
	cytoplasmOutside->m_name = "cytoplasm";
	golgi->m_name = "golgi";
	lysosome1->m_name = "lysosome";
	lysosome2->m_name = "lysosome";
	mitochondria1->m_name = "mitochondria";
	mitochondria2->m_name = "mitochondria";
	mitochondria3->m_name = "mitochondria";
	mitochondria4->m_name = "mitochondria";
	mitochondria5->m_name = "mitochondria";
	mitochondria6->m_name = "mitochondria";
	mitochondria7->m_name = "mitochondria";
	mitochondriaOuter1->m_name = "mitochondria";
	mitochondriaOuter2->m_name = "mitochondria";
	mitochondriaOuter3->m_name = "mitochondria";
	mitochondriaOuter4->m_name = "mitochondria";
	mitochondriaOuter5->m_name = "mitochondria";
	mitochondriaOuter6->m_name = "mitochondria";
	mitochondriaOuter7->m_name = "mitochondria";
	nuclearEnvelope->m_name = "nucleous";
	nucleolus->m_name = "nucleous";
	peroxisome1->m_name = "peroxisome";
	peroxisome2->m_name = "peroxisome";
	peroxisome3->m_name = "peroxisome";
	peroxisome4->m_name = "peroxisome";
	peroxisome5->m_name = "peroxisome";
	peroxisome6->m_name = "peroxisome";
	ribosome1->m_name = "ribosome";
	ribosome2->m_name = "ribosome";
	ribosome3->m_name = "ribosome";
	ribosome4->m_name = "ribosome";
	ribosome5->m_name = "ribosome";
	ribosome6->m_name = "ribosome";
	ribosome7->m_name = "ribosome";
	ribosome8->m_name = "ribosome";
	ribosome9->m_name = "ribosome";
	ribosome10->m_name = "ribosome";
	ribosome11->m_name = "ribosome";
	roughER->m_name = "roughER";
	smoothER->m_name = "smoothER";


	// rotate the object 90 degrees
	//cytoplasmOutside->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), 90);

	// load an object file
	bool fileload;

	fileload = centriole1->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/centriole1.obj"));
	fileload = centriole2->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/centriole2.obj"));
	fileload = chromatin->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/Chromatin.obj"));
	fileload = cytoplasmOutside->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/cytoplasm_outer.obj"));
	fileload = golgi->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/Golgi.obj"));
	//fileload = golgiVesicle->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/gogli_vesicle.obj"));
	//fileload = golgiVesicle0->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/gogli_vesicle0.obj"));
	//fileload = golgiVesicle1->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/gogli_vesicle1.obj"));
	//fileload = golgiVesicle2->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/gogli_vesicle2.obj"));
	//fileload = golgiVesicle3->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/gogli_vesicle3.obj"));
	//fileload = golgiVesicle4->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/gogli_vesicle4.obj"));
	//fileload = golgiVesicle5->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/gogli_vesicle5.obj"));
	//fileload = golgiVesicle6->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/gogli_vesicle6.obj"));
	//fileload = golgiVesicle7->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/gogli_vesicle7.obj"));
	//fileload = golgiVesicle8->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/gogli_vesicle8.obj"));
	//fileload = golgiVesicle9->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/gogli_vesicle9.obj"));
	fileload = lysosome1->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/lysosome1.obj"));
	fileload = lysosome2->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/lysosome2.obj"));
	fileload = mitochondria1->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/mitochondria1.obj"));
	fileload = mitochondria2->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/mitochondria2.obj"));
	fileload = mitochondria3->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/mitochondria3.obj"));
	fileload = mitochondria4->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/mitochondria4.obj"));
	fileload = mitochondria5->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/mitochondria5.obj"));
	fileload = mitochondria6->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/mitochondria6.obj"));
	fileload = mitochondria7->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/mitochondria7.obj"));
	fileload = mitochondriaOuter1->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/mitochondria_outer1.obj"));
	fileload = mitochondriaOuter2->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/mitochondria_outer2.obj"));
	fileload = mitochondriaOuter3->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/mitochondria_outer3.obj"));
	fileload = mitochondriaOuter4->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/mitochondria_outer4.obj"));
	fileload = mitochondriaOuter5->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/mitochondria_outer5.obj"));
	fileload = mitochondriaOuter6->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/mitochondria_outer6.obj"));
	fileload = mitochondriaOuter7->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/mitochondria_outer7.obj"));
	fileload = nuclearEnvelope->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/nuclear_envelope.obj"));
	fileload = nuclearSlice->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/Nuclear_slice.obj"));
	fileload = nuclearSliceInner->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/Nuclear_slice_inner.obj"));
	fileload = nucleolus->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/Nucleolus.obj"));
	fileload = peroxisome1->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/peroxisome1.obj"));
	fileload = peroxisome2->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/peroxisome2.obj"));
	fileload = peroxisome3->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/peroxisome3.obj"));
	fileload = peroxisome4->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/peroxisome4.obj"));
	fileload = peroxisome5->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/peroxisome5.obj"));
	fileload = peroxisome6->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/peroxisome6.obj"));
	fileload = ribosome1->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/ribosome1.obj"));
	fileload = ribosome2->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/ribosome2.obj"));
	fileload = ribosome3->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/ribosome3.obj"));
	fileload = ribosome4->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/ribosome4.obj"));
	fileload = ribosome5->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/ribosome5.obj"));
	fileload = ribosome6->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/ribosome6.obj"));
	fileload = ribosome7->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/ribosome7.obj"));
	fileload = ribosome8->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/ribosome8.obj"));
	fileload = ribosome9->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/ribosome9.obj"));
	fileload = ribosome10->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/ribosome10.obj"));
	fileload = ribosome11->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/ribosome11.obj"));
	fileload = roughER->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/Rough_ER.obj"));
	fileload = smoothER->loadFromFile(RESOURCE_PATH("../../../../resources/models/cell/animal/Smooth_ER.obj"));

	// get dimensions of object
	cytoplasmOutside->computeBoundaryBox(true);
	double size = cSub(cytoplasmOutside->getBoundaryMax(), cytoplasmOutside->getBoundaryMin()).length();

	// resize object to screen
	if (size > 0.001)
	{
		centriole1->scale(1.0 / size);
		centriole2->scale(1.0 / size);
		chromatin->scale(1.0 / size);
		cytoplasmOutside->scale(1.0 / size);
		golgi->scale(1.0 / size);
		//golgiVesicle->scale(1.0 / size);
		//golgiVesicle0->scale(1.0 / size);
		//golgiVesicle1->scale(1.0 / size);
		//golgiVesicle2->scale(1.0 / size);
		//golgiVesicle3->scale(1.0 / size);
		//golgiVesicle4->scale(1.0 / size);
		//golgiVesicle5->scale(1.0 / size);
		//golgiVesicle6->scale(1.0 / size);
		//golgiVesicle7->scale(1.0 / size);
		//golgiVesicle8->scale(1.0 / size);
		//golgiVesicle9->scale(1.0 / size);
		lysosome1->scale(1.0 / size);
		lysosome2->scale(1.0 / size);
		mitochondria1->scale(1.0 / size);
		mitochondria2->scale(1.0 / size);
		mitochondria3->scale(1.0 / size);
		mitochondria4->scale(1.0 / size);
		mitochondria5->scale(1.0 / size);
		mitochondria6->scale(1.0 / size);
		mitochondria7->scale(1.0 / size);
		mitochondriaOuter1->scale(1.0 / size);
		mitochondriaOuter2->scale(1.0 / size);
		mitochondriaOuter3->scale(1.0 / size);
		mitochondriaOuter4->scale(1.0 / size);
		mitochondriaOuter5->scale(1.0 / size);
		mitochondriaOuter6->scale(1.0 / size);
		mitochondriaOuter7->scale(1.0 / size);
		nuclearEnvelope->scale(1.0 / size);
		nuclearSlice->scale(1.0 / size);
		nuclearSlice->scale(1.0 / size);
		nucleolus->scale(1.0 / size);
		peroxisome1->scale(1.0 / size);
		peroxisome2->scale(1.0 / size);
		peroxisome3->scale(1.0 / size);
		peroxisome4->scale(1.0 / size);
		peroxisome5->scale(1.0 / size);
		peroxisome6->scale(1.0 / size);
		ribosome1->scale(1.0 / size);
		ribosome2->scale(1.0 / size);
		ribosome3->scale(1.0 / size);
		ribosome4->scale(1.0 / size);
		ribosome5->scale(1.0 / size);
		ribosome6->scale(1.0 / size);
		ribosome7->scale(1.0 / size);
		ribosome8->scale(1.0 / size);
		ribosome9->scale(1.0 / size);
		ribosome10->scale(1.0 / size);
		ribosome11->scale(1.0 / size);
		roughER->scale(1.0 / size);
		smoothER->scale(1.0 / size);
	}

	// compute collision detection algorithm
	centriole1->createAABBCollisionDetector(toolRadius);
	centriole2->createAABBCollisionDetector(toolRadius);
	chromatin->createAABBCollisionDetector(toolRadius);
	cytoplasmOutside->createAABBCollisionDetector(toolRadius);
	golgi->createAABBCollisionDetector(toolRadius);
	lysosome1->createAABBCollisionDetector(toolRadius);
	lysosome2->createAABBCollisionDetector(toolRadius);
	mitochondriaOuter1->createAABBCollisionDetector(toolRadius);
	mitochondriaOuter2->createAABBCollisionDetector(toolRadius);
	mitochondriaOuter3->createAABBCollisionDetector(toolRadius);
	mitochondriaOuter4->createAABBCollisionDetector(toolRadius);
	mitochondriaOuter5->createAABBCollisionDetector(toolRadius);
	mitochondriaOuter6->createAABBCollisionDetector(toolRadius);
	mitochondriaOuter7->createAABBCollisionDetector(toolRadius);
	mitochondria1->createAABBCollisionDetector(toolRadius);
	mitochondria2->createAABBCollisionDetector(toolRadius);
	mitochondria3->createAABBCollisionDetector(toolRadius);
	mitochondria4->createAABBCollisionDetector(toolRadius);
	mitochondria5->createAABBCollisionDetector(toolRadius);
	mitochondria6->createAABBCollisionDetector(toolRadius);
	mitochondria7->createAABBCollisionDetector(toolRadius);
	nuclearEnvelope->createAABBCollisionDetector(toolRadius);
	nucleolus->createAABBCollisionDetector(toolRadius);
	peroxisome1->createAABBCollisionDetector(toolRadius);
	peroxisome2->createAABBCollisionDetector(toolRadius);
	peroxisome3->createAABBCollisionDetector(toolRadius);
	peroxisome4->createAABBCollisionDetector(toolRadius);
	peroxisome5->createAABBCollisionDetector(toolRadius);
	peroxisome6->createAABBCollisionDetector(toolRadius);
	ribosome1->createAABBCollisionDetector(toolRadius);
	ribosome2->createAABBCollisionDetector(toolRadius);
	ribosome3->createAABBCollisionDetector(toolRadius);
	ribosome4->createAABBCollisionDetector(toolRadius);
	ribosome5->createAABBCollisionDetector(toolRadius);
	ribosome6->createAABBCollisionDetector(toolRadius);
	ribosome7->createAABBCollisionDetector(toolRadius);
	ribosome8->createAABBCollisionDetector(toolRadius);
	ribosome9->createAABBCollisionDetector(toolRadius);
	ribosome10->createAABBCollisionDetector(toolRadius);
	ribosome11->createAABBCollisionDetector(toolRadius);
	roughER->createAABBCollisionDetector(toolRadius);
	smoothER->createAABBCollisionDetector(toolRadius);


	// disable culling so that faces are rendered on both sides
	//cytoplasmOutside->setUseCulling(false);

	// enable display list for faster graphic rendering
	cytoplasmOutside->setUseDisplayList(true);

	// center object in scene
	//cytoplasmOutside->setLocalPos(-1.0 * cytoplasmOutside->getBoundaryCenter());

	// rotate object in scene
	centriole1->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	centriole2->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	chromatin->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	cytoplasmOutside->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	golgi->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	//golgiVesicle->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	//golgiVesicle0->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	//golgiVesicle1->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	//golgiVesicle2->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	//golgiVesicle3->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	//golgiVesicle4->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	//golgiVesicle5->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	//golgiVesicle6->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	//golgiVesicle7->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	//golgiVesicle8->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	//golgiVesicle9->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	lysosome1->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	lysosome2->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	mitochondria1->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	mitochondria2->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	mitochondria3->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	mitochondria4->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	mitochondria5->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	mitochondria6->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	mitochondria7->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	mitochondriaOuter1->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	mitochondriaOuter2->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	mitochondriaOuter3->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	mitochondriaOuter4->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	mitochondriaOuter5->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	mitochondriaOuter6->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	mitochondriaOuter7->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	nuclearEnvelope->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	nuclearSlice->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	nuclearSliceInner->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	nucleolus->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	peroxisome1->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	peroxisome2->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	peroxisome3->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	peroxisome4->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	peroxisome5->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	peroxisome6->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	ribosome1->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	ribosome2->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	ribosome3->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	ribosome4->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	ribosome5->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	ribosome6->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	ribosome7->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	ribosome8->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	ribosome9->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	ribosome10->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	ribosome11->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	roughER->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	smoothER->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);



	// set haptic properties
	cMaterial mat;
	mat.setHapticTriangleSides(true, true);
	mat.setStiffness(0.2 * maxStiffness);
	mat.setStaticFriction(0.2);
	mat.setDynamicFriction(0.1);

	centriole1->setMaterial(mat);
	centriole2->setMaterial(mat);
	chromatin->setMaterial(mat);
	cytoplasmOutside->setMaterial(mat);
	golgi->setMaterial(mat);
	lysosome1->setMaterial(mat);
	lysosome2->setMaterial(mat);
	mitochondriaOuter1->setMaterial(mat);
	mitochondriaOuter2->setMaterial(mat);
	mitochondriaOuter3->setMaterial(mat);
	mitochondriaOuter4->setMaterial(mat);
	mitochondriaOuter5->setMaterial(mat);
	mitochondriaOuter6->setMaterial(mat);
	mitochondriaOuter7->setMaterial(mat);
	mitochondria1->setMaterial(mat);
	mitochondria2->setMaterial(mat);
	mitochondria3->setMaterial(mat);
	mitochondria4->setMaterial(mat);
	mitochondria5->setMaterial(mat);
	mitochondria6->setMaterial(mat);
	mitochondria7->setMaterial(mat);
	nuclearEnvelope->setMaterial(mat);
	nucleolus->setMaterial(mat);
	peroxisome1->setMaterial(mat);
	peroxisome2->setMaterial(mat);
	peroxisome3->setMaterial(mat);
	peroxisome4->setMaterial(mat);
	peroxisome5->setMaterial(mat);
	peroxisome6->setMaterial(mat);
	ribosome1->setMaterial(mat);
	ribosome2->setMaterial(mat);
	ribosome3->setMaterial(mat);
	ribosome4->setMaterial(mat);
	ribosome5->setMaterial(mat);
	ribosome6->setMaterial(mat);
	ribosome7->setMaterial(mat);
	ribosome8->setMaterial(mat);
	ribosome9->setMaterial(mat);
	ribosome10->setMaterial(mat);
	ribosome11->setMaterial(mat);
	roughER->setMaterial(mat);
	smoothER->setMaterial(mat);


	//--------------------------------------------------------------------------
	// CREATE ENVIRONMENT GLOBE
	//--------------------------------------------------------------------------

	world->m_backgroundColor.setBlack();

	// create a virtual mesh
	cMesh* globe = new cMesh();

	// add object to world
	world->addChild(globe);

	// set the position of the object at the center of the world
	globe->setLocalPos(0.0, 0.0, 0.0);

	// create cube
	cCreateSphere(globe, 10.0, 36, 36);

	globe->setUseDisplayList(true);
	globe->deleteCollisionDetector();

	// create a texture
	cTexture2dPtr textureSpace = cTexture2d::create();

	fileload = textureSpace->loadFromFile(RESOURCE_PATH("C:/chai3d/chai3d-3.2.0-vanilla/modules/OCULUS/bin/resources/images/shade.jpg"));
	if (!fileload)
	{
#if defined(_MSVC)
		fileload = textureSpace->loadFromFile("C:/chai3d/chai3d-3.2.0-vanilla/modules/OCULUS/bin/resources/images/shade.jpg");
#endif
	}
	if (!fileload)
	{
		cout << "Error - Texture image failed to load correctly." << endl;
		close();
		return (-1);
	}

	// apply texture to object
	globe->setTexture(textureSpace);

	// enable texture rendering 
	globe->setUseTexture(true);

	// Since we don't need to see our polygons from both sides, we enable culling.
	globe->setUseCulling(false);

	// disable material properties and lighting
	globe->setUseMaterial(false);


	//--------------------------------------------------------------------------
	// CREATE SHADERS
	//--------------------------------------------------------------------------

	// create program shader
	cShaderProgramPtr shaderProgram = cShaderProgram::create(C_SHADER_FONG_VERT, C_SHADER_FONG_FRAG);

	// set uniforms
	shaderProgram->setUniformi("uShadowMap", C_TU_SHADOWMAP);

	// assign shader to mesh objects in the world
	tool->setShaderProgram(shaderProgram);
	
	//centriole1->setShaderProgram(shaderProgram2);
	//centriole2->setShaderProgram(shaderProgram2);
	//chromatin->setShaderProgram(shaderProgram3);
	//cytoplasmOutside->setShaderProgram(shaderProgram);
	//golgi->setShaderProgram(shaderProgram);
	//lysosome1->setShaderProgram(shaderProgram5);
	//lysosome2->setShaderProgram(shaderProgram5);
	//mitochondriaOuter1->setShaderProgram(shaderProgram);
	//mitochondriaOuter2->setShaderProgram(shaderProgram);
	//mitochondriaOuter3->setShaderProgram(shaderProgram);
	//mitochondriaOuter4->setShaderProgram(shaderProgram);
	//mitochondriaOuter5->setShaderProgram(shaderProgram);
	//mitochondriaOuter6->setShaderProgram(shaderProgram);
	//mitochondriaOuter7->setShaderProgram(shaderProgram);
	//mitochondria1->setShaderProgram(shaderProgram);
	//mitochondria2->setShaderProgram(shaderProgram);
	//mitochondria3->setShaderProgram(shaderProgram);
	//mitochondria4->setShaderProgram(shaderProgram);
	//mitochondria5->setShaderProgram(shaderProgram);
	//mitochondria6->setShaderProgram(shaderProgram);
	//mitochondria7->setShaderProgram(shaderProgram);
	//nuclearEnvelope->setShaderProgram(shaderProgram);
	//nucleolus->setShaderProgram(shaderProgram);
	//peroxisome1->setShaderProgram(shaderProgram);
	//peroxisome2->setShaderProgram(shaderProgram);
	//peroxisome3->setShaderProgram(shaderProgram);
	//peroxisome4->setShaderProgram(shaderProgram);
	//peroxisome5->setShaderProgram(shaderProgram);
	//peroxisome6->setShaderProgram(shaderProgram);
	//ribosome1->setShaderProgram(shaderProgram);
	//ribosome2->setShaderProgram(shaderProgram);
	//ribosome3->setShaderProgram(shaderProgram);
	//ribosome4->setShaderProgram(shaderProgram);
	//ribosome5->setShaderProgram(shaderProgram);
	//ribosome6->setShaderProgram(shaderProgram);
	//ribosome7->setShaderProgram(shaderProgram);
	//ribosome8->setShaderProgram(shaderProgram);
	//ribosome9->setShaderProgram(shaderProgram);
	//ribosome10->setShaderProgram(shaderProgram);
	//ribosome11->setShaderProgram(shaderProgram);
	//roughER->setShaderProgram(shaderProgram);
	//smoothER->setShaderProgram(shaderProgram);

	//--------------------------------------------------------------------------
	// WIDGETS
	//--------------------------------------------------------------------------

	// create a font
	font = NEW_CFONTCALIBRI36();

	//cVector3d labelPos(800.0, 1000.0, 1000.0);
	labelOrganelle = new cLabel(font);
	camera->m_frontLayer->addChild(labelOrganelle);
	//labelOrganelle->setLocalPos(labelPos);
	labelOrganelle->setText("");
	labelOrganelle->m_fontColor.setRedCrimson();
	labelOrganelle->setShowBoundaryBox(true);
	labelOrganelle->scale(1.0 / size);
	//labelOrganelle->setEnabled(true);
	//labelOrganelle->setHapticEnabled(false);
	//labelOrganelle->setColor(cColorf(1.0, 1.0, 1.0));
	//labelOrganelle->setShowPanel(true);
	

	//--------------------------------------------------------------------------
	// START SIMULATION
	//--------------------------------------------------------------------------

	// create a thread which starts the main haptics rendering loop
	hapticsThread = new cThread();
	hapticsThread->start(updateHaptics, CTHREAD_PRIORITY_HAPTICS);

	// setup callback when application exits
	atexit(close);


	//--------------------------------------------------------------------------
	// MAIN GRAPHIC LOOP
	//--------------------------------------------------------------------------

	// recenter oculus
	oculusVR.recenterPose();

	// main graphic rendering loop
	while (!glfwWindowShouldClose(window) && !simulationFinished)
	{
		// handle key presses
		//processEvents();

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		
		world->updateShadowMaps(false, false);

		// start rendering
		oculusVR.onRenderStart();

		// render frame for each eye
		for (int eyeIndex = 0; eyeIndex < ovrEye_Count; eyeIndex++)
		{
			// retrieve projection and modelview matrix from oculus
			cTransform projectionMatrix, modelViewMatrix;
			oculusVR.onEyeRender(eyeIndex, projectionMatrix, modelViewMatrix);

			camera->m_useCustomProjectionMatrix = true;
			camera->m_projectionMatrix = projectionMatrix;

			camera->m_useCustomModelViewMatrix = true;
			camera->m_modelViewMatrix = modelViewMatrix;

			if (eyeIndex == 0)
			{
				labelOrganelle->setLocalPos(labelPos);
				labelOrganelle->setEnabled(true);
			}
			else
			{
				labelOrganelle->setLocalPos(labelPos + cVector3d(-480, 0, 0));
				labelOrganelle->setEnabled(true);
			}

			

			// render world
			ovrSizei size = oculusVR.getEyeTextureSize(eyeIndex);
			camera->renderView(size.w, size.h, C_STEREO_LEFT_EYE, false);

			// finalize rendering  
			oculusVR.onEyeRenderFinish(eyeIndex);
		}

		

		// update frames
		oculusVR.submitFrame();
		oculusVR.blitMirror();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// cleanup
	oculusVR.destroyVR();
	renderContext.destroy();

	glfwDestroyWindow(window);

	// exit glfw
	glfwTerminate();

	return (0);
}

//------------------------------------------------------------------------------

void errorCallback(int a_error, const char* a_description)
{
	cout << "Error: " << a_description << endl;
}

//------------------------------------------------------------------------------

void keyCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
{
	// filter calls that only include a key press
	if ((a_action != GLFW_PRESS) && (a_action != GLFW_REPEAT))
	{
		return;
	}

	// option - exit
	else if ((a_key == GLFW_KEY_ESCAPE) || (a_key == GLFW_KEY_Q))
	{
		glfwSetWindowShouldClose(a_window, GLFW_TRUE);
	}

	// option - spacebar
	else if (a_key == GLFW_KEY_SPACE)
	{
		oculusVR.recenterPose();
	}

	// option - enable/disable label
	else if (a_key == GLFW_KEY_L)
	{
		labelOrganelle->setEnabled(!(labelOrganelle->getEnabled()));
		cout << "Toggle labels" << endl;
	}

	// option - enable/disable haptics
	else if (a_key == GLFW_KEY_H)
	{
		tool->setForcesOFF();
		tool->updateToolImagePosition();
	}

	else if (a_key == GLFW_KEY_UP)
	{
		labelPos += cVector3d(0.0, 50.0, 0.0);
	}

	else if (a_key == GLFW_KEY_DOWN)
	{
		labelPos += cVector3d(0.0, -50.0, 0.0);
	}

	else if (a_key == GLFW_KEY_RIGHT)
	{
		labelPos += cVector3d(50.0, 0.0, 0.0);
	}

	else if (a_key == GLFW_KEY_LEFT)
	{
		labelPos += cVector3d(-50.0, 0.0, 0.0);
	}

	else if (a_key == GLFW_KEY_R)
	{
		centriole1->setLocalPos(0.0, 0.0, 0.0);
		centriole2->setLocalPos(0.0, 0.0, 0.0);
		chromatin->setLocalPos(0.0, 0.0, 0.0);
		cytoplasmOutside->setLocalPos(0.0, 0.0, 0.0);
		golgi->setLocalPos(0.0, 0.0, 0.0);
		//golgiVesicle->setLocalPos(0.0, 0.0, 0.0);
		//golgiVesicle0->setLocalPos(0.0, 0.0, 0.0);
		//golgiVesicle1->setLocalPos(0.0, 0.0, 0.0);
		//golgiVesicle2->setLocalPos(0.0, 0.0, 0.0);
		//golgiVesicle3->setLocalPos(0.0, 0.0, 0.0);
		//golgiVesicle4->setLocalPos(0.0, 0.0, 0.0);
		//golgiVesicle5->setLocalPos(0.0, 0.0, 0.0);
		//golgiVesicle6->setLocalPos(0.0, 0.0, 0.0);
		//golgiVesicle7->setLocalPos(0.0, 0.0, 0.0);
		//golgiVesicle8->setLocalPos(0.0, 0.0, 0.0);
		//golgiVesicle9->setLocalPos(0.0, 0.0, 0.0);
		lysosome1->setLocalPos(0.0, 0.0, 0.0);
		lysosome2->setLocalPos(0.0, 0.0, 0.0);
		mitochondria1->setLocalPos(0.0, 0.0, 0.0);
		mitochondria2->setLocalPos(0.0, 0.0, 0.0);
		mitochondria3->setLocalPos(0.0, 0.0, 0.0);
		mitochondria4->setLocalPos(0.0, 0.0, 0.0);
		mitochondria5->setLocalPos(0.0, 0.0, 0.0);
		mitochondria6->setLocalPos(0.0, 0.0, 0.0);
		mitochondria7->setLocalPos(0.0, 0.0, 0.0);
		mitochondriaOuter1->setLocalPos(0.0, 0.0, 0.0);
		mitochondriaOuter2->setLocalPos(0.0, 0.0, 0.0);
		mitochondriaOuter3->setLocalPos(0.0, 0.0, 0.0);
		mitochondriaOuter4->setLocalPos(0.0, 0.0, 0.0);
		mitochondriaOuter5->setLocalPos(0.0, 0.0, 0.0);
		mitochondriaOuter6->setLocalPos(0.0, 0.0, 0.0);
		mitochondriaOuter7->setLocalPos(0.0, 0.0, 0.0);
		nuclearEnvelope->setLocalPos(0.0, 0.0, 0.0);
		nuclearSlice->setLocalPos(0.0, 0.0, 0.0);
		nuclearSliceInner->setLocalPos(0.0, 0.0, 0.0);
		nucleolus->setLocalPos(0.0, 0.0, 0.0);
		peroxisome1->setLocalPos(0.0, 0.0, 0.0);
		peroxisome2->setLocalPos(0.0, 0.0, 0.0);
		peroxisome3->setLocalPos(0.0, 0.0, 0.0);
		peroxisome4->setLocalPos(0.0, 0.0, 0.0);
		peroxisome5->setLocalPos(0.0, 0.0, 0.0);
		peroxisome6->setLocalPos(0.0, 0.0, 0.0);
		ribosome1->setLocalPos(0.0, 0.0, 0.0);
		ribosome2->setLocalPos(0.0, 0.0, 0.0);
		ribosome3->setLocalPos(0.0, 0.0, 0.0);
		ribosome4->setLocalPos(0.0, 0.0, 0.0);
		ribosome5->setLocalPos(0.0, 0.0, 0.0);
		ribosome6->setLocalPos(0.0, 0.0, 0.0);
		ribosome7->setLocalPos(0.0, 0.0, 0.0);
		ribosome8->setLocalPos(0.0, 0.0, 0.0);
		ribosome9->setLocalPos(0.0, 0.0, 0.0);
		ribosome10->setLocalPos(0.0, 0.0, 0.0);
		ribosome11->setLocalPos(0.0, 0.0, 0.0);
		roughER->setLocalPos(0.0, 0.0, 0.0);
		smoothER->setLocalPos(0.0, 0.0, 0.0);

		// rotate object in scene
		centriole1->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		centriole2->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		chromatin->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		cytoplasmOutside->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		golgi->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		//golgiVesicle->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		//golgiVesicle0->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		//golgiVesicle1->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		//golgiVesicle2->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		//golgiVesicle3->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		//golgiVesicle4->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		//golgiVesicle5->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		//golgiVesicle6->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		//golgiVesicle7->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		//golgiVesicle8->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		//golgiVesicle9->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		lysosome1->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		lysosome2->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		mitochondria1->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		mitochondria2->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		mitochondria3->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		mitochondria4->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		mitochondria5->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		mitochondria6->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		mitochondria7->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		mitochondriaOuter1->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		mitochondriaOuter2->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		mitochondriaOuter3->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		mitochondriaOuter4->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		mitochondriaOuter5->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		mitochondriaOuter6->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		mitochondriaOuter7->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		nuclearEnvelope->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		nuclearSlice->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		nuclearSliceInner->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		nucleolus->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		peroxisome1->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		peroxisome2->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		peroxisome3->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		peroxisome4->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		peroxisome5->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		peroxisome6->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		ribosome1->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		ribosome2->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		ribosome3->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		ribosome4->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		ribosome5->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		ribosome6->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		ribosome7->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		ribosome8->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		ribosome9->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		ribosome10->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		ribosome11->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		roughER->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
		smoothER->rotateExtrinsicEulerAnglesDeg(90, 0, 0, C_EULER_ORDER_XYZ);
	}
}


//------------------------------------------------------------------------------

void close(void)
{
	// stop the simulation
	simulationRunning = false;

	// wait for graphics and haptics loops to terminate
	while (!simulationFinished) { cSleepMs(100); }

	// close haptic device
	tool->stop();

	// delete resources
	delete hapticsThread;
	delete world;
	delete handler;
}

//------------------------------------------------------------------------------

enum cMode
{
	IDLE,
	SELECTION
};

void updateHaptics(void)
{
	cMode state = IDLE;
	cGenericObject* selectedObject;
	cTransform tool_T_object;

	// simulation in now running
	simulationRunning = true;
	simulationFinished = false;

	// main haptic simulation loop
	while (simulationRunning)
	{
		/////////////////////////////////////////////////////////////////////////
		// HAPTIC RENDERING
		/////////////////////////////////////////////////////////////////////////

		// update frequency counter
		frequencyCounter.signal(1);

		// compute global reference frames for each object
		world->computeGlobalPositions(true);

		// update position and orientation of tool
		tool->updateFromDevice();

		// compute interaction forces
		tool->computeInteractionForces();


		/////////////////////////////////////////////////////////////////////////
		// MANIPULATION
		/////////////////////////////////////////////////////////////////////////

		// compute transformation from world to tool (haptic device)
		cTransform world_T_tool = tool->getDeviceGlobalTransform();

		// get status of user switch
		bool button = tool->getUserSwitch(0);

		//
		// STATE 1:
		// Idle mode - user presses the user switch
		//
		if ((state == IDLE) && (button == true))
		{
			// check if at least one contact has occurred
			if (tool->m_hapticPoint->getNumCollisionEvents() > 0)
			{
				// get contact event
				cCollisionEvent* collisionEvent = tool->m_hapticPoint->getCollisionEvent(0);

				// get object from contact event
				selectedObject = collisionEvent->m_object->getParent();

				if (selectedObject->m_name == "cytoplasm")
					labelOrganelle->setText(textCytoplasm);
				else if (selectedObject->m_name == "roughER")
					labelOrganelle->setText(textRoughER);
				else if (selectedObject->m_name == "smoothER")
					labelOrganelle->setText(textSmoothER);
				else if (selectedObject->m_name == "mitochondria")
					labelOrganelle->setText(textMitochondria);
				else if (selectedObject->m_name == "peroxisome")
					labelOrganelle->setText(textPeroxisome);
				else if (selectedObject->m_name == "chromatin")
					labelOrganelle->setText(textChromatin);
				else if (selectedObject->m_name == "ribosome")
					labelOrganelle->setText(textRibosome);
				else if (selectedObject->m_name == "nucleous")
					labelOrganelle->setText(textNucleus);
				else if (selectedObject->m_name == "lysosome")
					labelOrganelle->setText(textLysosome);
				else if (selectedObject->m_name == "golgi")
					labelOrganelle->setText(textGolgi);
				else if (selectedObject->m_name == "centriole")
					labelOrganelle->setText(textCentriole);

				          

				//cout << selectedObject->m_name << endl;
			}
			else
			{
				selectedObject = cytoplasmOutside;

				labelOrganelle->setText(textCytoplasm);
			}

			// get transformation from object
			cTransform world_T_object = selectedObject->getGlobalTransform();

			// compute inverse transformation from contact point to object 
			cTransform tool_T_world = world_T_tool;
			tool_T_world.invert();

			// store current transformation tool
			tool_T_object = tool_T_world * world_T_object;

			// update state
			state = SELECTION;
		}


		//
		// STATE 2:
		// Selection mode - operator maintains user switch enabled and moves object
		//
		else if ((state == SELECTION) && (button == true))
		{
			// compute new transformation of object in global coordinates
			cTransform world_T_object = world_T_tool * tool_T_object;

			// compute new transformation of object in local coordinates
			cTransform parent_T_world = selectedObject->getParent()->getLocalTransform();
			parent_T_world.invert();
			cTransform parent_T_object = parent_T_world * world_T_object;

			// assign new local transformation to object
			selectedObject->setLocalTransform(parent_T_object);

			// set zero forces when manipulating objects
			tool->setDeviceGlobalForce(0.0, 0.0, 0.0);

			tool->initialize();
		}

		//
		// STATE 3:
		// Finalize Selection mode - operator releases user switch.
		//
		else
		{
			state = IDLE;
		}


		/////////////////////////////////////////////////////////////////////////
		// FINALIZE
		/////////////////////////////////////////////////////////////////////////

		// send forces to haptic device
		tool->applyToDevice();
	}

	// exit haptics thread
	simulationFinished = true;
}

//------------------------------------------------------------------------------
