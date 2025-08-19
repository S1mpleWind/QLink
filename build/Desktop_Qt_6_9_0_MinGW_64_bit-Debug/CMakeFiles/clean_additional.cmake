# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\QLink_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\QLink_autogen.dir\\ParseCache.txt"
  "QLink_autogen"
  )
endif()
