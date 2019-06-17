# Credits: https://github.com/nicolastagliani/conan-tinyxml2/issues/3
#
include( FindPackageHandleStandardArgs )

find_path( tinyxml2_INCLUDE_DIR
  NAMES
    tinyxml2.h
  PATHS
    ${CONAN_INCLUDE_DIRS_TINYXML2}
)

find_library( tinyxml2_LIBRARY
  NAMES
    ${CONAN_LIBS_TINYXML2}
  PATHS
    ${CONAN_LIB_DIRS_TINYXML2}
)

find_package_handle_standard_args( tinyxml2 DEFAULT_MSG
  tinyxml2_INCLUDE_DIR
)

if( tinyxml2_FOUND )
  set( tinyxml2_INCLUDE_DIRS ${tinyxml2_INCLUDE_DIR} )
  set( tinyxml2_LIBRARIES ${tinyxml2_LIBRARY} )

  get_filename_component( tinyxml2_CONFIG_PATH ${CONAN_TINYXML2_ROOT} DIRECTORY )
  get_filename_component( tinyxml2_HASH ${CONAN_TINYXML2_ROOT} NAME )
  get_filename_component( tinyxml2_CONFIG_PATH ${tinyxml2_CONFIG_PATH} DIRECTORY )
  set( tinyxml2_CONFIG_PATH  ${tinyxml2_CONFIG_PATH}/build/${tinyxml2_HASH} )
  set( tinyxml2_CONFIG_FILENAME tinyxml2Config.cmake )

  find_file( tinyxml2_CONFIG_DIR
      ${tinyxml2_CONFIG_FILENAME}
    HINTS
      ${tinyxml2_CONFIG_PATH}
  )

  if( tinyxml2_CONFIG_DIR-NOTFOUND )
    set( tinyxml2_CONFIG "" )
  else()
    set( tinyxml2_CONFIG ${tinyxml2_CONFIG_DIR} )
  endif()

  mark_as_advanced(
    tinyxml2_INCLUDE_DIR
    tinyxml2_LIBRARY
    tinyxml2_DIR
    tinyxml2_CONFIG
  )
else()
  set( tinyxml2_DIR "" CACHE STRING
    "An optional hint to a tinyxml2 directory"
  )
endif()
