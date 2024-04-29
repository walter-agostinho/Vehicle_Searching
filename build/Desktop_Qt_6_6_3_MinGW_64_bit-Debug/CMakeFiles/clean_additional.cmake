# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Vehicle_Searching_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Vehicle_Searching_autogen.dir\\ParseCache.txt"
  "Vehicle_Searching_autogen"
  )
endif()
