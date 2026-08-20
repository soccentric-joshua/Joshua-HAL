add_library(pwm_output_lib pwm_output_lib.cpp pwm_output_lib.h)
target_include_directories(pwm_output_lib PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(pwm_output_lib PUBLIC lib_base pru_icss_lib)
install(TARGETS pwm_output_lib LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES pwm_output_lib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/joshua_hal)
