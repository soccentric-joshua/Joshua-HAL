add_library(mipi_csi_lib mipi_csi_lib.cpp mipi_csi_lib.h)
target_include_directories(mipi_csi_lib PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mipi_csi_lib PUBLIC lib_base pru_icss_lib)
install(TARGETS mipi_csi_lib LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES mipi_csi_lib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/joshua_hal)
