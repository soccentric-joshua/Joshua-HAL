add_library(mipi_dsi_lib mipi_dsi_lib.cpp mipi_dsi_lib.h)
target_include_directories(mipi_dsi_lib PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mipi_dsi_lib PUBLIC lib_base)
install(TARGETS mipi_dsi_lib LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES mipi_dsi_lib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/joshua_hal)
