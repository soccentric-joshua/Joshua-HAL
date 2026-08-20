add_library(accelerometer_lib accelerometer_lib.cpp accelerometer_lib.h)
target_include_directories(accelerometer_lib PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(accelerometer_lib PUBLIC lib_base)
install(TARGETS accelerometer_lib LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES accelerometer_lib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/joshua_hal)
