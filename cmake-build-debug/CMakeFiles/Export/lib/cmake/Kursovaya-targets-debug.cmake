#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Kursovaya::Kursovaya" for configuration "Debug"
set_property(TARGET Kursovaya::Kursovaya APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Kursovaya::Kursovaya PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libKursovayad.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Kursovaya::Kursovaya )
list(APPEND _IMPORT_CHECK_FILES_FOR_Kursovaya::Kursovaya "${_IMPORT_PREFIX}/lib/libKursovayad.a" )

# Import target "Kursovaya::demo" for configuration "Debug"
set_property(TARGET Kursovaya::demo APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Kursovaya::demo PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/demo"
  )

list(APPEND _IMPORT_CHECK_TARGETS Kursovaya::demo )
list(APPEND _IMPORT_CHECK_FILES_FOR_Kursovaya::demo "${_IMPORT_PREFIX}/bin/demo" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
