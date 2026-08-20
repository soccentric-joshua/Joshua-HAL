add_library(i2c_lib i2c_lib.cpp i2c_lib.h)
target_include_directories(i2c_lib PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(i2c_lib PUBLIC lib_base)
install(TARGETS i2c_lib LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES i2c_lib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/joshua_hal)
