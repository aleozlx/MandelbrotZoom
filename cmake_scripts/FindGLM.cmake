#
# Find GLM
#
# Try to find GLM : OpenGL Mathematics.
# This module defines 
# - GLM_INCLUDE_DIRS
# - GLM_FOUND
#
# The following variables can be set as arguments for the module.
# - GLM_ROOT_DIR : Root library directory of GLM 
#
# References:
# - https://github.com/Groovounet/glm/blob/master/util/FindGLM.cmake
# - https://bitbucket.org/alfonse/gltut/src/28636298c1c0/glm-0.9.0.7/FindGLM.cmake
#

if (WIN32)
 # Find include files
  find_path(
   GLM_INCLUDE_DIR
   NAMES glm/glm.hpp
   PATHS
   $ENV{PROGRAMFILES}/include
   ${GLM_ROOT_DIR}/include
   DOC "The directory where glm/glm.hpp resides")
else()
  # Find include files
  find_path(
   GLM_INCLUDE_DIR
   NAMES glm/glm.hpp
   PATHS
   /usr/include
   /usr/local/include
   /sw/include
   /opt/local/include
   ${GLM_ROOT_DIR}/include
   DOC "The directory where glm/glm.hpp resides")
endif()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(GLM DEFAULT_MSG GLM_INCLUDE_DIR)

