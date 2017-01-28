//==============================================================================
/*
\author    Ozan Tokatli
*/
//==============================================================================

//------------------------------------------------------------------------------
#include "CApplication.h"
//------------------------------------------------------------------------------
using namespace chai3d;
//------------------------------------------------------------------------------

cApplication::cApplication ()
{
	//--------------------------------------------------------------------------
	// INITIALIZATION
	//--------------------------------------------------------------------------

	// initialize variables
	m_running = false;
	m_finished = false;
	m_timerID = 0;

	// reset frequency counters
	m_graphicRate.reset();
	m_hapticRate.reset();


	//--------------------------------------------------------------------------
	// WORLD - CAMERA - LIGHTING
	//--------------------------------------------------------------------------

	// create a new world.
	m_world = new cWorld();

	// set the background color of the environment
	m_world->m_backgroundColor.setBlack();

	// create a camera and insert it into the virtual world
	m_camera = new cCamera(m_world);
	m_cameraOculus = new cCamera(m_world);
	m_world->addChild(m_camera);
	m_world->addChild(m_cameraOculus);

	// define a basis in spherical coordinates for the camera
	m_camera->setSphericalReferences(cVector3d(0, 0, 0),    // origin
	cVector3d(0, 0, 1),    // zenith direction
	cVector3d(1, 0, 0));   // azimuth direction

	m_cameraOculus->setSphericalReferences(cVector3d(0, 0, 0),    // origin
	cVector3d(0, 0, 1),    // zenith direction
	cVector3d(1, 0, 0));   // azimuth direction

	m_camera->setSphericalDeg(2.0,    // spherical coordinate radius
	0,     // spherical coordinate azimuth angle
	0);    // spherical coordinate polar angle

    m_cameraOculus->setSphericalDeg(2.0,    // spherical coordinate radius
      0,     // spherical coordinate azimuth angle
      0);    // spherical coordinate polar angle

      // set the near and far clipping planes of the camera
      m_camera->setClippingPlanes (0.01, 10.0);
      m_cameraOculus->setClippingPlanes(0.1, 100.0);

      // create a light source
      m_light = new cDirectionalLight (m_world);

      // add light to camera
      m_camera->addChild(m_light);
      m_cameraOculus->addChild(m_light);

      // enable light source
      m_light->setEnabled(true);

      // define the direction of the light beam
      m_light->setDir(-1.0, -1.0, -1.0);
	  //m_light->setDir(-1.0, 0.0, -1.0);

	  // enable this light source to generate shadows
	  //m_light->setShadowMapEnabled(true);

	  // set the resolution of the shadow map
	  //m_light->m_shadowMap->setQualityLow();


	  //--------------------------------------------------------------------------
	  // HAPTIC DEVICES / TOOLS
	  //--------------------------------------------------------------------------

	  // create a haptic device handler
	  handler = new cHapticDeviceHandler();

	  // get access to the first available haptic device found
	  handler->getDevice(m_device, 0);

	  // open a connection to haptic device
	  m_device->open();

	  // calibrate device (if necessary)
	  m_device->calibrate();

	  // retrieve information about the current haptic device
	  info = m_device->getSpecifications();

	  // create a tool (cursor) and insert into the world
	  m_tool = new cToolCursor(m_world);
	  m_world->addChild(m_tool);

	  // display a reference frame if haptic device supports orientations
	  //if (info.m_sensedRotation == true)
	  //{
	  //	// display reference frame
	  //	m_tool->setShowFrame(true);

	  //	// set the size of the reference frame
	  //	m_tool->setFrameSize(0.05);
	  //}

	  // connect the haptic device to the virtual tool
	  m_tool->setHapticDevice(m_device);

	  // initialize tool by connecting to haptic device
	  m_tool->start();

	  // define the radius of the tool (sphere)
	  double toolRadius = 0.05;
	  m_tool->setRadius(toolRadius);

	  // map the physical workspace of the haptic device to a larger virtual workspace.
	  m_tool->setWorkspaceRadius(1.2);

	  // hide the device sphere. only show proxy.
	  m_tool->setShowContactPoints(true, false);

	  // enable if objects in the scene are going to rotate of translate
	  // or possibly collide against the tool. If the environment
	  // is entirely static, you can set this parameter to "false"
	  m_tool->enableDynamicObjects(true);

	  // haptic forces are enabled only if small forces are first sent to the device;
	  // this mode avoids the force spike that occurs when the application starts when
	  // the tool is located inside an object for instance.
	  m_tool->setWaitForSmallForce(true);

	  // if the device has a gripper, enable the gripper to simulate a user switch
	  //hapticDevice->setEnableGripperUserSwitch(false);

	  // start the haptic tool
	  m_tool->start();


      //--------------------------------------------------------------------------
      // OBJECTS
      //--------------------------------------------------------------------------

      // read the scale factor between the physical workspace of the haptic
      // device and the virtual workspace defined for the tool
      double workspaceScaleFactor = m_tool->getWorkspaceScaleFactor();

      // stiffness properties
      double maxLinearForce = info.m_maxLinearForce;
      double maxLinearDamping = info.m_maxLinearDamping;
      double m_maxStiffness = info.m_maxLinearStiffness / workspaceScaleFactor;

      phase1(m_maxStiffness);
      phase2(m_maxStiffness);
      phase3(m_maxStiffness);

      //--------------------------------------------------------------------------
      // WIDGETS
      //--------------------------------------------------------------------------

      // create a background
      background = new cBackground();
	  background2 = new cBackground();
      m_camera->m_backLayer->addChild(background);
	  m_cameraOculus->m_backLayer->addChild(background2);

      // set background properties
      //background->loadFromFile("C:/3DLearning/images/background/background_animal.jpg");
	  //background->loadFromFile("C:/3DLearning/images/background/plainBackground.png");
	  //background2->loadFromFile("C:/3DLearning/images/background/plainBackground.png");
	  background->loadFromFile("../../../../../../images/background/plainBackground.png");
	  background2->loadFromFile("../../../../../../images/background/plainBackground.png");
    };

    //------------------------------------------------------------------------------

    cApplication::~cApplication()
    {
      delete m_world;
    }

    //------------------------------------------------------------------------------

    void* cApplication::hapticThread()
    {
      cMode state = IDLE;
      cTransform tool_T_object;

      // update state
      m_running = true;
      m_finished = false;

      while (m_running)
      {
        // update frequency counter
        m_hapticRate.signal(1);

        // compute global reference frames for each object
        m_world->computeGlobalPositions(true);

        // update position and orientation of tool
        m_tool->updateFromDevice();

        // compute interaction forces
        m_tool->computeInteractionForces();

        //-------------------------------------------------------------
        // Manipulation
        //-------------------------------------------------------------
        if (m_simulationPhase != PHASE1)
        {
          manipulateObject(state, tool_T_object);
        }



        // send forces to haptic device
        m_tool->applyToDevice();
      }

      // disable forces
      m_device->setForceAndTorqueAndGripperForce(cVector3d(0.0, 0.0, 0.0),
      cVector3d(0.0, 0.0, 0.0),
      0.0);

      // update state
      m_running = false;
      m_finished = true;

      // exit thread
      return (NULL);
    }

    //------------------------------------------------------------------------------

    void* cApplication::oculusThread()
    {
      if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
      {
        //cout << "failed initialization" << endl;
        //cSleepMs(1000);
        //return (NULL);
      }

      if (!m_oculusVR.initVR())
      {
        /*cout << "failed to initialize Oculus" << endl;
        cSleepMs(1000);*/
        SDL_Quit();
        //return (NULL);
      }

      ovrSizei hmdResolution = m_oculusVR.getResolution();
      ovrSizei windowSize = { hmdResolution.w / 2, hmdResolution.h / 2 };

      m_renderContext.init("CHAI3D", 100, 100, windowSize.w, windowSize.h);
      SDL_ShowCursor(SDL_DISABLE);

      if (glewInit() != GLEW_OK)
      {
        m_oculusVR.destroyVR();
        m_renderContext.destroy();
        SDL_Quit();
        //return (NULL);
      }

      if (!m_oculusVR.initVRBuffers(windowSize.w, windowSize.h))
      {
        m_oculusVR.destroyVR();
        m_renderContext.destroy();
        SDL_Quit();
        //return (NULL);
      }

      while (m_running)
      {
        // start rendering
        m_oculusVR.onRenderStart();

        // render frame for each eye
        for (int eyeIndex = 0; eyeIndex < ovrEye_Count; eyeIndex++)
        {
          // retrieve projection and modelview matrix from oculus
          cTransform projectionMatrix, modelViewMatrix;
          m_oculusVR.onEyeRender(eyeIndex, projectionMatrix, modelViewMatrix);

          m_cameraOculus->m_useCustomProjectionMatrix = true;
          m_cameraOculus->m_projectionMatrix = projectionMatrix;

          m_cameraOculus->m_useCustomModelViewMatrix = true;
          m_cameraOculus->m_modelViewMatrix = modelViewMatrix;

          // render world
          ovrSizei size = m_oculusVR.getEyeTextureSize(eyeIndex);
          m_cameraOculus->renderView(size.w, size.h, 0, C_STEREO_LEFT_EYE, false);

          // finalize rendering
          m_oculusVR.onEyeRenderFinish(eyeIndex);
        }

        // update frames
        m_oculusVR.submitFrame();
        m_oculusVR.blitMirror();
        SDL_GL_SwapWindow(m_renderContext.window);
      }

      //// cleanup
      //m_oculusVR.destroyVR();
      //m_renderContext.destroy();

      //// exit SDL
      //SDL_Quit();

	  // update state
	  /*m_running = false;
	  m_finished = true;*/

      // exit thread
      return (NULL);
    }

	//------------------------------------------------------------------------------

	//void* cApplication::dataThread()
	//{
	//	// Open file for data recording
	//	m_dataOutFile.open("outputData.txt");

	//	int i = 0;
	//	while (m_running)
	//	{
	//		m_dataPosition.push_back(m_tool->getDeviceGlobalPos());
	//		m_dataLinVel.push_back(m_tool->getDeviceGlobalLinVel());
	//		m_dataAngVel.push_back(m_tool->getDeviceGlobalAngVel());
	//		m_dataForce.push_back(m_tool->getDeviceGlobalForce());
	//		m_dataTorque.push_back(m_tool->getDeviceGlobalTorque());
	//		m_dataTmp.push_back(++i);
	//	}

	//	// record data to file
	//	std::ostream_iterator<int> it(m_dataOutFile, "\n");
	//	std::copy(m_dataTmp.begin(), m_dataTmp.end(), it);

	//	return(NULL);
	//}

    //------------------------------------------------------------------------------

    void cApplication::paintGL()
    {
      // render world
      m_camera->renderView(m_width, m_height);

      // wait until all GL commands are completed
      glFinish();

      m_graphicRate.signal(1);
    }

    //------------------------------------------------------------------------------

    void cApplication::resizeGL(int a_width,  int a_height)
    {
      m_width = a_width;
      m_height = a_height;
    }

    //------------------------------------------------------------------------------

    int cApplication::start()
    {
      // start haptic thread
      m_thread.start (_hapticThread, CTHREAD_PRIORITY_HAPTICS, this);
	  m_threadOculus.start(_oculusThread, CTHREAD_PRIORITY_GRAPHICS, this);
	  //m_threadData.start(_dataThread, CTHREAD_PRIORITY_GRAPHICS, this);

      return(0);
    }

    //------------------------------------------------------------------------------

    int cApplication::stop()
    {
      m_running = false;

      while (!m_finished)
      {
        cSleepMs(100);
      }

      m_tool->stop ();

	  // cleanup
	  m_oculusVR.destroyVR();
	  m_renderContext.destroy();

	  // exit SDL
	  SDL_Quit();

      return 0;
    }

    //------------------------------------------------------------------------------

    void _hapticThread (void *arg)
    {
      ((cApplication*)arg)->hapticThread();
    }

    //------------------------------------------------------------------------------

    void _oculusThread(void *arg)
    {
      ((cApplication*)arg)->oculusThread();
    }

    //------------------------------------------------------------------------------

	/*void _dataThread(void *arg)
	{
		((cApplication*)arg)->dataThread();
	}*/

	//------------------------------------------------------------------------------

    void cApplication::phase1(const double &maxStiffness)
    {
      std::vector<std::string> plantObjFiles = { "../../../../../../models/cell/plant/v3/cellwall.obj",
      //"C:/3DLearning/models/cell/plant/v3/cytoplasm_inside.obj",
      "../../../../../../models/cell/plant/v3/cytoplasm_outside.obj" };

      std::vector<std::string> animalObjFiles = { "../../../../../../models/cell/animal/v2/cytoplasm_outer.obj" };

      const int numPlantCells = 3;

      // assign haptic properties
      cMaterial matMembrane;
      matMembrane.setStiffness(0.3 * maxStiffness);

      cMultiMesh* tmpMesh = NULL;

      std::vector<std::string>::const_iterator it;
      std::vector<cMultiMesh*>::const_iterator it2;

      const double scale2 = m_scale / 2;

      for (int j = -numPlantCells; j < numPlantCells; j++)
      {
        for (int i = -numPlantCells; i < numPlantCells; i++)
        {
          for (it = plantObjFiles.begin(); it != plantObjFiles.end(); it++)
          {
            tmpMesh = new cMultiMesh();
            m_world->addChild(tmpMesh);

            tmpMesh->loadFromFile(*it);

            tmpMesh->getMesh(0)->setUseTransparency(false);

            tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);
            tmpMesh->translate(0.0, 280 * scale2 / 2 * i, 370 * scale2 / 2 * j);

            // scale object
            tmpMesh->scale(scale2);

            // create collision detector
            tmpMesh->createAABBCollisionDetector(m_toolRadius);

            // Assign material
            tmpMesh->setMaterial(matMembrane);

            tmpMesh->setEnabled(false);

            multiplePlantCell.push_back(tmpMesh);
          }
        }
      }

      for (int j = -numPlantCells; j < numPlantCells; j++)
      {
        for (int i = -numPlantCells; i < numPlantCells; i++)
        {
          for (it = animalObjFiles.begin(); it != animalObjFiles.end(); it++)
          {
            tmpMesh = new cMultiMesh();
            m_world->addChild(tmpMesh);

            tmpMesh->loadFromFile(*it);

            tmpMesh->getMesh(0)->setUseTransparency(false);

            tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);
            tmpMesh->translate(0.0, 280 * scale2 / 2 * i, 370 * scale2 / 2 * j);

            // scale object
            tmpMesh->scale(scale2);

            // create collision detector
            tmpMesh->createAABBCollisionDetector(m_toolRadius);

            // Assign material
            tmpMesh->setMaterial(matMembrane);

            tmpMesh->setEnabled(false);

			//tmpMesh->setUseDisplayList(true);

            multipleAnimalCell.push_back(tmpMesh);
          }
        }
      }

      m_cellType = ANIMAL;
      currentCellVector = multipleAnimalCell;
      for (it2 = currentCellVector.begin(); it2 != currentCellVector.end(); ++it2)
      {
        tmpMesh = *it2;
        tmpMesh->setEnabled(true);
      }
    }

    //------------------------------------------------------------------------------

    void cApplication::phase2(const double &maxStiffness)
    {
      std::vector<std::string> plantObjFiles = { "../../../../../../models/cell/plant/v3/ActinFilaments.obj",
      "../../../../../../models/cell/plant/v3/mitochondrion_outer.obj",
      "../../../../../../models/cell/plant/v3/Peroxisome.obj",
      "../../../../../../models/cell/plant/v3/Thylakoid_Membrane.obj",
      "../../../../../../models/cell/plant/v3/Lysosome_outer.obj",
      "../../../../../../models/cell/plant/v3/golgi.obj",
      "../../../../../../models/cell/plant/v3/cellwall.obj",
      "../../../../../../models/cell/plant/v3/cytoplasm_outside.obj",
      "../../../../../../models/cell/plant/v3/vacuole.obj",
      "../../../../../../models/cell/plant/v3/roughER.obj",
      "../../../../../../models/cell/plant/v3/smoothEP.obj",
      "../../../../../../models/cell/plant/v3/nuclear_outer.obj"
    };

    std::vector<std::string> animalObjFiles = { "../../../../../../models/cell/animal/v2/centriole1.obj",
    "../../../../../../models/cell/animal/v2/centriole2.obj",
    //"../../../../../../models/cell/animal/v2/Chromatin.obj",
    "../../../../../../models/cell/animal/v2/cytoplasm_outer.obj",
    "../../../../../../models/cell/animal/v2/Golgi.obj",
    "../../../../../../models/cell/animal/v2/lysosome1.obj",
    "../../../../../../models/cell/animal/v2/lysosome2.obj",
    "../../../../../../models/cell/animal/v2/microtubes1.obj",
    "../../../../../../models/cell/animal/v2/microtubes2.obj",
    "../../../../../../models/cell/animal/v2/microtubes3.obj",
    "../../../../../../models/cell/animal/v2/microtubes4.obj",
    "../../../../../../models/cell/animal/v2/microtubes5.obj",
    "../../../../../../models/cell/animal/v2/microtubes6.obj",
    "../../../../../../models/cell/animal/v2/microtubes7.obj",
    "../../../../../../models/cell/animal/v2/microtubes8.obj",
    "../../../../../../models/cell/animal/v2/microtubes9.obj",
    "../../../../../../models/cell/animal/v2/mitochondria1.obj",
    "../../../../../../models/cell/animal/v2/mitochondria2.obj",
    "../../../../../../models/cell/animal/v2/mitochondria3.obj",
    "../../../../../../models/cell/animal/v2/mitochondria4.obj",
    "../../../../../../models/cell/animal/v2/mitochondria5.obj",
    "../../../../../../models/cell/animal/v2/mitochondria6.obj",
    "../../../../../../models/cell/animal/v2/mitochondria7.obj",
    "../../../../../../models/cell/animal/v2/nuclear_envelope.obj",
    "../../../../../../models/cell/animal/v2/peroxisome1.obj",
    "../../../../../../models/cell/animal/v2/peroxisome2.obj",
    "../../../../../../models/cell/animal/v2/peroxisome3.obj",
    "../../../../../../models/cell/animal/v2/peroxisome4.obj",
    "../../../../../../models/cell/animal/v2/peroxisome5.obj",
    "../../../../../../models/cell/animal/v2/Rough_ER.obj",
    "../../../../../../models/cell/animal/v2/Smooth_ER.obj"
  };

  // assign haptic properties
  cMaterial matMembrane;
  matMembrane.setStiffness(0.3 * maxStiffness);

  cMultiMesh* tmpMesh = NULL;
  cMultiMesh* tmpMesh2 = NULL;

  std::vector<std::string>::const_iterator it;

  for (it = plantObjFiles.begin(); it != plantObjFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    singlePlantCell.push_back(tmpMesh);
  }

  for (it = animalObjFiles.begin(); it != animalObjFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    singleAnimalCell.push_back(tmpMesh);
  }
}

//------------------------------------------------------------------------------

void cApplication::phase3(const double &maxStiffness)
{
  std::vector<std::string> plantActinFilamentsFiles = { "../../../../../../models/cell/plant/v3/ActinFilaments.obj" };

  std::vector<std::string> plantChloroplastFiles = { "../../../../../../models/cell/plant/v3/Thylakoid_Membrane.obj",
  "../../../../../../models/cell/plant/v3/Chloroplast.obj" };

  std::vector<std::string> plantEndoplasmicReticulumFiles = { "../../../../../../models/cell/plant/v3/roughER.obj",
  "../../../../../../models/cell/plant/v3/smoothEP.obj" };

  std::vector<std::string> plantGolgiFiles = { "../../../../../../models/cell/plant/v3/golgi.obj" };

  std::vector<std::string> plantMitochondrionFiles = { "../../../../../../models/cell/plant/v3/mitochondrion_outer.obj",
  "../../../../../../models/cell/plant/v3/mitochondrion_inner.obj" };

  std::vector<std::string> plantNucleusFiles = { "../../../../../../models/cell/plant/v3/nuclear_outer.obj",
  "../../../../../../models/cell/plant/v3/sliced_Nuclear.obj",
  "../../../../../../models/cell/plant/v3/nuclear_pore.obj",
  "../../../../../../models/cell/plant/v3/Nucleoplasm.obj",
  "../../../../../../models/cell/plant/v3/nucleolus.obj",
  "../../../../../../models/cell/plant/v3/chromatin.obj" };

  std::vector<std::string> plantPeroxisomeFiles = { "../../../../../../models/cell/plant/v3/Peroxisome.obj" };

  std::vector<std::string> plantVacuoleFiles = { "../../../../../../models/cell/plant/v3/vacuole.obj" };


  std::vector<std::string> animalEndoplasmicReticulumFiles = { "../../../../../../models/cell/animal/v2/Rough_ER.obj",
  "../../../../../../models/cell/animal/v2/Smooth_ER.obj" };

  std::vector<std::string> animalCentrioleFiles = { "../../../../../../models/cell/animal/v2/centriole1.obj",
  "../../../../../../models/cell/animal/v2/centriole2.obj" };

  std::vector<std::string> animalGolgiFiles = { "../../../../../../models/cell/animal/v2/Golgi.obj" };

  std::vector<std::string> animalLysosomeFiles = { "../../../../../../models/cell/animal/v2/lysosome1.obj",
  "../../../../../../models/cell/animal/v2/lysosome1.obj" };

  std::vector<std::string> animalMicrotubesFiles = { "../../../../../../models/cell/animal/v2/microtubes1.obj",
  "../../../../../../models/cell/animal/v2/microtubes2.obj",
  "../../../../../../models/cell/animal/v2/microtubes3.obj",
  "../../../../../../models/cell/animal/v2/microtubes4.obj",
  "../../../../../../models/cell/animal/v2/microtubes5.obj",
  "../../../../../../models/cell/animal/v2/microtubes6.obj",
  "../../../../../../models/cell/animal/v2/microtubes7.obj",
  "../../../../../../models/cell/animal/v2/microtubes8.obj",
  "../../../../../../models/cell/animal/v2/microtubes9.obj" };

  std::vector<std::string> animalMitochondrionFiles = { "../../../../../../models/cell/animal/v2/mitochondria1.obj",
  "../../../../../../models/cell/animal/v2/mitochondria2.obj",
  "../../../../../../models/cell/animal/v2/mitochondria3.obj",
  "../../../../../../models/cell/animal/v2/mitochondria4.obj",
  "../../../../../../models/cell/animal/v2/mitochondria5.obj",
  "../../../../../../models/cell/animal/v2/mitochondria6.obj",
  "../../../../../../models/cell/animal/v2/mitochondria7.obj" };

  std::vector<std::string> animalNucleusFiles = { "../../../../../../models/cell/animal/v2/nuclear_envelope.obj",
  //"../../../../../../models/cell/animal/v2/Nuclear_slice.obj",
  "../../../../../../models/cell/animal/v2/Nucleolus.obj",
  "../../../../../../models/cell/animal/v2/Chromatin.obj" };

  std::vector<std::string> animalPeroxisomeFiles = { "../../../../../../models/cell/animal/v2/peroxisome1.obj",
  "../../../../../../models/cell/animal/v2/peroxisome2.obj",
  "../../../../../../models/cell/animal/v2/peroxisome3.obj",
  "../../../../../../models/cell/animal/v2/peroxisome4.obj",
  "../../../../../../models/cell/animal/v2/peroxisome5.obj" };

  // assign haptic properties
  cMaterial matMembrane;
  matMembrane.setStiffness(0.3 * maxStiffness);

  cMultiMesh* tmpMesh = NULL;
  cMultiMesh* tmpMesh2 = NULL;

  std::vector<std::string>::const_iterator it;

  for (it = plantActinFilamentsFiles.begin(); it != plantActinFilamentsFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    plantActinFilaments.push_back(tmpMesh);
  }

  for (it = plantChloroplastFiles.begin(); it != plantChloroplastFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    plantChloroplast.push_back(tmpMesh);;
  }

  for (it = plantEndoplasmicReticulumFiles.begin(); it != plantEndoplasmicReticulumFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    plantEndoplasmicReticulum.push_back(tmpMesh);
  }

  for (it = plantGolgiFiles.begin(); it != plantGolgiFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    plantGolgi.push_back(tmpMesh);
  }

  for (it = plantMitochondrionFiles.begin(); it != plantMitochondrionFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    plantMitochondrion.push_back(tmpMesh);
  }

  for (it = plantNucleusFiles.begin(); it != plantNucleusFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    plantNucleus.push_back(tmpMesh);
  }

  for (it = plantPeroxisomeFiles.begin(); it != plantPeroxisomeFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    plantPeroxisome.push_back(tmpMesh);
  }

  for (it = plantVacuoleFiles.begin(); it != plantVacuoleFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    plantVacoule.push_back(tmpMesh);
  }

  for (it = animalEndoplasmicReticulumFiles.begin(); it != animalEndoplasmicReticulumFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    animalEndoplasmicReticulum.push_back(tmpMesh);
  }

  for (it = animalCentrioleFiles.begin(); it != animalCentrioleFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    animalCentriole.push_back(tmpMesh);;
  }

  for (it = animalGolgiFiles.begin(); it != animalGolgiFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    animalGolgi.push_back(tmpMesh);
  }

  for (it = animalLysosomeFiles.begin(); it != animalLysosomeFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    animalLysosome.push_back(tmpMesh);
  }

  for (it = animalMicrotubesFiles.begin(); it != animalMicrotubesFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    animalMicrotubes.push_back(tmpMesh);
  }

  for (it = animalMitochondrionFiles.begin(); it != animalMitochondrionFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    animalMitochondrion.push_back(tmpMesh);
  }

  for (it = animalNucleusFiles.begin(); it != animalNucleusFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    animalNucleus.push_back(tmpMesh);
  }

  for (it = animalPeroxisomeFiles.begin(); it != animalPeroxisomeFiles.end(); it++)
  {
    tmpMesh = new cMultiMesh();
    m_world->addChild(tmpMesh);

    tmpMesh->loadFromFile(*it);

    tmpMesh->getMesh(0)->setUseTransparency(false);

    tmpMesh->rotateAboutGlobalAxisDeg(cVector3d(0, 0, 1), -90.0);

    // scale object
    tmpMesh->scale(m_scale);

    // create collision detetctor
    tmpMesh->createAABBCollisionDetector(m_toolRadius);

    // Assign material
    tmpMesh->setMaterial(matMembrane);

    tmpMesh->setEnabled(false);

    animalPeroxisome.push_back(tmpMesh);
  }
}

//------------------------------------------------------------------------------

void cApplication::manipulateObject(cMode &state, cTransform &tool_T_object)
{
  // compute transformation from world to tool (haptic device)
  cTransform world_T_tool = m_tool->getDeviceGlobalTransform();

  // get status of user switch
  bool robotButton1 = m_tool->getUserSwitch(0);
  //bool robotButton2 = m_tool->getUserSwitch(1);

  //
  // STATE 1:
  // Idle mode - user presses the user switch
  //
  if ((state == IDLE) && (robotButton1 == true))
  {
    // check if at least one contact has occurred
    if (m_tool->m_hapticPoint->getNumCollisionEvents() > 0)
    {
      // get contact event
      cCollisionEvent* collisionEvent = m_tool->m_hapticPoint->getCollisionEvent(0);

      if (selectedObject != NULL)
		selectedObject->setShowBoundaryBox(false);

      // get object from contact event
      selectedObject = collisionEvent->m_object;

      // get transformation from object
      cTransform world_T_object = selectedObject->getGlobalTransform();

      // compute inverse transformation from contact point to object
      cTransform tool_T_world = world_T_tool;
      tool_T_world.invert();

      // store current transformation tool
      tool_T_object = tool_T_world * world_T_object;

      // update state
      state = SELECTION;

      selectedObject->setShowBoundaryBox(true);

      organelleName = selectedObject->m_name;
    }
    else if (selectedObject != NULL)
    {
      organelleName = "";
      selectedObject->setShowBoundaryBox(false);
      selectedObject = NULL;
    }
  }
  //
  // STATE 2:
  // Selection mode - operator maintains user switch enabled and moves object
  //
  else if ((state == SELECTION) && (robotButton1 == true))
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
    m_tool->setDeviceGlobalForce(0.0, 0.0, 0.0);

    m_tool->initialize();
  }
  //
  // STATE 3:
  // Finalize Selection mode - operator releases user switch.
  //
  else
  {
    state = IDLE;
  }
}

//------------------------------------------------------------------------------