add_library(bluetooth_lib bluetooth_lib.cpp bluetooth_lib.h)
target_include_directories(bluetooth_lib PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bluetooth_lib PUBLIC lib_base)
install(TARGETS bluetooth_lib LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES bluetooth_lib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/joshua_hal)
