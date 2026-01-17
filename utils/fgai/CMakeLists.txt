add_executable(fgai
  fgai.cxx
  AIBVHPager.cxx
  AIEnvironment.cxx
  AIManager.cxx
  AIObject.cxx
  AIPhysics.cxx
)

target_link_libraries(fgai
  SimGearCore SimGearScene
  vsg::vsg
  ${OPENSCENEGRAPH_LIBRARIES}
  ${OPENGL_LIBRARIES}
)

install(TARGETS fgai RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
