## This module contains different helper asserts

cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

function(CXXBASICS_ASSERT_WRITABLE_FOLDER  folder_path)
  include(helpers/FnMktemp)

  # Lets attempt to create a random file
  set(CXXBASICS_TMP_FOLDER "${folder_path}")
  mktemp()

  if(NOT DEFINED mktemp_result)
    cberror("Folder \"${folder_path}\" is not writable")
  else()
    # Removing the temporary file
    file(REMOVE "${mktemp_result}")
  endif()
endfunction()
