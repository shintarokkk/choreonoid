# @author Shin'ichiro Nakaoka

#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
#set_source_files_properties(SimulatorItem.cpp PROPERTIES COMPILE_FLAGS "-O0 -g")

set(sources
  BodyPlugin.cpp
  BodyItem.cpp
  WorldItem.cpp
  BodyMotionItem.cpp
  ZMPSeqItem.cpp
  MultiDeviceStateSeqItem.cpp
  WorldLogFileItem.cpp
  SimulatorItem.cpp
  SubSimulatorItem.cpp
  ControllerItem.cpp
  SimpleControllerItem.cpp
  BodyMotionControllerItem.cpp
  SimulationScriptItem.cpp
  AISTSimulatorItem.cpp
  GLVisionSimulatorItem.cpp
  SensorVisualizerItem.cpp
  BodyTrackingCameraItem.cpp
  BodyMotionEngine.cpp
  KinematicFaultChecker.cpp
  SplineFilterDialog.cpp
  EditableSceneBody.cpp
  BodyBar.cpp
  LeggedBodyBar.cpp
  KinematicsBar.cpp
  SimulationBar.cpp
  LinkTreeWidget.cpp
  LinkSelectionView.cpp
  LinkPropertyView.cpp
  BodyLinkView.cpp
  JointSliderView.cpp
  JointStateView.cpp
  BodyStateView.cpp
  JointGraphView.cpp
  LinkGraphView.cpp
  HrpsysFileIO.cpp
  CollisionSeq.cpp
  CollisionSeqItem.cpp
  CollisionSeqEngine.cpp
  )

set(headers
  BodyItem.h
  WorldItem.h
  BodyMotionItem.h
  ZMPSeqItem.cpp
  MultiDeviceStateSeqItem.h
  WorldLogFileItem.h
  SimulatorItem.h
  SubSimulatorItem.h
  ControllerItem.h
  SimpleControllerItem.h
  SimulationScriptItem.h
  AISTSimulatorItem.h
  GLVisionSimulatorItem.h
  SensorVisualizerItem.h
  BodyTrackingCameraItem.h
  KinematicFaultChecker.h
  BodyBar.h
  KinematicsBar.h
  SimulationBar.h
  LinkTreeWidget.h
  LinkSelectionView.h
  EditableSceneBody.h
  CollisionSeq.h
  CollisionSeqItem.h
  CollisionSeqEngine.h
  AISTSimulatorItem.h
  exportdecl.h
  )

set(target CnoidBodyPlugin)

make_gettext_mofiles(${target} mofiles)

QTX_ADD_RESOURCES(RC_SRCS BodyPlugin.qrc)
QTX_WRAP_CPP(qtsources LinkTreeWidget.h)
set(sources ${sources} ${qtsources})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_cnoid_plugin(${target} SHARED ${sources} ${headers} ${mofiles} ${RC_SRCS})

set(boost_libraries ${Boost_IOSTREAMS_LIBRARY})
if(MSVC)
  set(boost_libraries ${boost_libraries} ${Boost_BZIP2_LIBRARY} ${Boost_ZLIB_LIBRARY})
endif()

target_link_libraries(${target} CnoidBase CnoidBody ${boost_libraries})
apply_common_setting_for_plugin(${target} "${headers}")

if(ENABLE_PYTHON)
  if(USE_PYBIND11)
    add_subdirectory(pybind11)
  elseif(USE_BOOST_PYTHON)
    add_subdirectory(boostpython)
  endif()
endif()

add_subdirectory(lua)
