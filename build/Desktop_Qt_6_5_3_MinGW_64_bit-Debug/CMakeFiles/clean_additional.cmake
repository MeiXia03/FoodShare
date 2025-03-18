# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\foodshare_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\foodshare_autogen.dir\\ParseCache.txt"
  "foodshare_autogen"
  )
endif()
