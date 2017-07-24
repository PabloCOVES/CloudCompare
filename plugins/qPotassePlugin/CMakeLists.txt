cmake_minimum_required(VERSION 3.0)

#REPLACE ALL 'POTASSE' OCCURENCES BY YOUR PLUGIN NAME
#AND ADAPT THE CODE BELOW TO YOUR OWN NEEDS!

option( INSTALL_QPOTASSE_PLUGIN "Check to install qPOTASSE plugin" OFF )

# CloudCompare 'POTASSE' plugin
if (INSTALL_QPOTASSE_PLUGIN)
    project( QPOTASSE_PLUGIN )
    ADD_DEFINITIONS(-DSCALAR=float)

    ADD_SUBDIRECTORY(potasse)
    include( ../CMakePluginTpl.cmake )

    INCLUDE_DIRECTORIES(potasse/header)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} potasse)
endif()
