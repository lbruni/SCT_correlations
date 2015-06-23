FIND_PACKAGE( ROOT REQUIRED)





AUX_SOURCE_DIRECTORY( src library_sources )
AUX_SOURCE_DIRECTORY( src/internal library_sources_internals )
SOURCE_GROUP(internals FILES ${library_sources_internals})
SET(ProjectDict_name SCT_plotsDict.cxx)


INCLUDE_DIRECTORIES(  ${ROOT_INCLUDE_DIR} )

# fill list of header files 
set(Project_HEADERS 
  ${PROJECT_SOURCE_DIR}/sct_plot/inc/sct_plots.h
  ${PROJECT_SOURCE_DIR}/sct_plot/inc/S_Axis.h
 ${PROJECT_SOURCE_DIR}/sct_plot/inc/s_cuts.h
${PROJECT_SOURCE_DIR}/sct_plot/inc/s_DrawOption.h
${PROJECT_SOURCE_DIR}/sct_plot/inc/s_plane.h
${PROJECT_SOURCE_DIR}/sct_plot/inc/s_plot.h
${PROJECT_SOURCE_DIR}/sct_plot/inc/s_plot_collection.h
${PROJECT_SOURCE_DIR}/sct_plot/inc/s_plot_prob.h
${PROJECT_SOURCE_DIR}/sct_plot/inc/s_treeCollection.h
  ${PROJECT_SOURCE_DIR}/sct_plot/inc/sct_plots_LinkDef.h
  )

SET(MY_INCLUDE_DIRECTORIES ./sct_plot/inc ./inc  ${ROOT_INCLUDE_DIR} )

# set everything needed for the root dictonary and create the
# dictionary
set(Project_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/${ProjectDict_name}) 

ROOT_GENERATE_DICTIONARY_OLD("${Project_HEADERS}" "${Project_DICTIONARY}" "${MY_INCLUDE_DIRECTORIES}")
ADD_LIBRARY( ${PROJECT_NAME} SHARED ${library_sources} ${library_sources_internals} ${Project_DICTIONARY} )



SOURCE_GROUP(Headers  ./inc/*.h)

target_link_libraries( ${PROJECT_NAME} ${ROOT_LIBRARIES} ${ADDITIONAL_LIBRARIES})


INSTALL(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
