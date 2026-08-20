add_library(analog_input_lib analog_input_lib.cpp analog_input_lib.h)
target_include_directories(analog_input_lib PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(analog_input_lib PUBLIC lib_base)
install(TARGETS analog_input_lib LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES analog_input_lib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/joshua_hal)
