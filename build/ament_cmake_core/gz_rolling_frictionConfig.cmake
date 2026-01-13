# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_gz_rolling_friction_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED gz_rolling_friction_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(gz_rolling_friction_FOUND FALSE)
  elseif(NOT gz_rolling_friction_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(gz_rolling_friction_FOUND FALSE)
  endif()
  return()
endif()
set(_gz_rolling_friction_CONFIG_INCLUDED TRUE)

# output package information
if(NOT gz_rolling_friction_FIND_QUIETLY)
  message(STATUS "Found gz_rolling_friction: 0.1.0 (${gz_rolling_friction_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'gz_rolling_friction' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT gz_rolling_friction_DEPRECATED_QUIET)
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(gz_rolling_friction_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${gz_rolling_friction_DIR}/${_extra}")
endforeach()
