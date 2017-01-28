//==============================================================================
/*
\author    Ozan Tokatli
*/
//==============================================================================


//------------------------------------------------------------------------------
#ifndef CAPPLICATION_H
#define CAPPLICATION_H
//------------------------------------------------------------------------------
#if defined(WIN32) | defined(WIN64)
#pragma warning(disable: 4100)
#endif
//------------------------------------------------------------------------------
#include "chai3d.h"
//------------------------------------------------------------------------------
#include "SDL.h"
#include "OVRRenderContext.h"
#include "OVRDevice.h"
//------------------------------------------------------------------------------
#include <stdlib.h>
#include <chrono>
#include <fstream>
//------------------------------------------------------------------------------

void _hapticThread (void *arg);
void _oculusThread(void *arg);
void _dataThread(void *arg);

//------------------------------------------------------------------------------

class cApplication
{

  //--------------------------------------------------------------------------
  // CONSTRUCTOR & DESTRUCTOR:
  //--------------------------------------------------------------------------

public:

  cApplication();
  virtual ~cApplication();


  //--------------------------------------------------------------------------
  // PUBLIC METHODS:
  //--------------------------------------------------------------------------

public:

  int start();
  int startOculus();
  int stop();

  void* hapticThread();
  void* oculusThread();
  //void* dataThread();


  //--------------------------------------------------------------------------
  // PUBLIC METHODS:
  //--------------------------------------------------------------------------

public:

  void resizeGL(int a_width, int a_height);
  void paintGL();


  //--------------------------------------------------------------------------
  // PUBLIC MEMBERS:
  //--------------------------------------------------------------------------

public:

  // application control
  int m_timerID;
  int m_width;
  int m_height;
  bool m_running;
  bool m_finished;

  // CHAI3D world
  chai3d::cGenericHapticDevicePtr m_device;
  chai3d::cWorld* m_world;
  chai3d::cCamera* m_camera;
  chai3d::cDirectionalLight* m_light;
  chai3d::cToolCursor* m_tool;
  chai3d::cMultiMesh* m_object;
  chai3d::cFrequencyCounter m_graphicRate;
  chai3d::cFrequencyCounter m_hapticRate;
  chai3d::cThread m_thread;
  chai3d::cHapticDeviceInfo info;
  chai3d::cHapticDeviceHandler* handler;

  // Oculus view
  chai3d::cThread m_threadOculus;
  chai3d::cOVRRenderContext m_renderContext;
  chai3d::cOVRDevice m_oculusVR;
  chai3d::cCamera* m_cameraOculus;

  // Data recording
  /*chai3d::cThread m_threadData;
  std::vector<chai3d::cVector3d> m_dataPosition;
  std::vector<chai3d::cVector3d> m_dataLinVel;
  std::vector<chai3d::cVector3d> m_dataAngVel;
  std::vector<chai3d::cVector3d> m_dataForce;
  std::vector<chai3d::cVector3d> m_dataTorque;
  std::vector<int> m_dataTmp;
  std::ofstream m_dataOutFile;*/

  // OBJECTS

  // PHASE 1
  std::vector<chai3d::cMultiMesh*> multiplePlantCell;
  std::vector<chai3d::cMultiMesh*> multipleAnimalCell;

  // PHASE 2
  std::vector<chai3d::cMultiMesh*> singlePlantCell;
  std::vector<chai3d::cMultiMesh*> singleAnimalCell;

  // PHASE 3
  std::vector<chai3d::cMultiMesh*> plantActinFilaments;
  std::vector<chai3d::cMultiMesh*> plantChloroplast;
  std::vector<chai3d::cMultiMesh*> plantEndoplasmicReticulum;
  std::vector<chai3d::cMultiMesh*> plantGolgi;
  std::vector<chai3d::cMultiMesh*> plantMitochondrion;
  std::vector<chai3d::cMultiMesh*> plantNucleus;
  std::vector<chai3d::cMultiMesh*> plantPeroxisome;
  std::vector<chai3d::cMultiMesh*> plantVacoule;

  std::vector<chai3d::cMultiMesh*> animalEndoplasmicReticulum;
  std::vector<chai3d::cMultiMesh*> animalCentriole;
  std::vector<chai3d::cMultiMesh*> animalGolgi;
  std::vector<chai3d::cMultiMesh*> animalLysosome;
  std::vector<chai3d::cMultiMesh*> animalMicrotubes;
  std::vector<chai3d::cMultiMesh*> animalMitochondrion;
  std::vector<chai3d::cMultiMesh*> animalNucleus;
  std::vector<chai3d::cMultiMesh*> animalPeroxisome;

  // Selected object
  chai3d::cGenericObject* selectedObject = NULL;

  // Vector of the current model
  std::vector<chai3d::cMultiMesh*> currentCellVector;

  // Name if the selected object
  std::string organelleName;

  // Background for the application
  chai3d::cBackground* background;
  chai3d::cBackground* background2;

  // Scaling constant for the virtual objects
  const double m_scale = 0.005;

  // Radius of the spherical tool
  const double m_toolRadius = 0.05;

  // display level for collision tree
  int m_collisionTreeDisplayLevel = 0;

  // Transparency level for the selected object
  double m_transparencyLevel = 1.0;

  // Stiffness of the haptics device
  double m_maxStiffness;

  inline const std::string currentDateTime()
  {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &tstruct);

    return buf;
  }

  enum cMode
  {
    IDLE,
    SELECTION
  };

  cMode m_state = IDLE;

  enum cPhase
  {
    PHASE1,
    PHASE2,
    PHASE3
  };

  cPhase m_simulationPhase = PHASE1;

  enum cCellType
  {
    ANIMAL,
    PLANT
  };

  cCellType m_cellType = ANIMAL;

  //--------------------------------------------------------------------------
  // Phases of the application
  //--------------------------------------------------------------------------

public:

  void phase1(const double &maxStiffness);
  void phase2(const double &maxStiffness);
  void phase3(const double &maxStiffness);
  void manipulateObject(cMode &state, chai3d::cTransform &tool_T_object);

};

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
