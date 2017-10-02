cmake_minimum_required(VERSION 3.0)

#REPLACE ALL 'POTASSE' OCCURENCES BY YOUR PLUGIN NAME
#AND ADAPT THE CODE BELOW TO YOUR OWN NEEDS!

option( INSTALL_QPOTASSE_PLUGIN "Check to install qPOTASSE plugin" OFF )

# CloudCompare 'POTASSE' plugin
if (INSTALL_QPOTASSE_PLUGIN)
    project( QPOTASSE_PLUGIN )

    #load necessary libraries (see qPCV for an example)
    #add_subdirectory (LIB1)
    
    #if the plugin is an 'OpenGL filter', uncomment the line below
    #set( CC_OPENGL_FILTER ON BOOL)
    include( ../CMakePluginTpl.cmake )
    
    FIND_PACKAGE (Eigen3)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} Eigen3::Eigen)
    
    #set dependencies to necessary libraries (see qPCV for an example)
    #target_link_libraries( ${PROJECT_NAME} LIB1 )
    #include_directories( ${LIB1_INCLUDE_DIR} )
endif()
