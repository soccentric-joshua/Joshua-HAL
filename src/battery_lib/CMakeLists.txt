add_library(battery_lib battery_lib.cpp battery_lib.h)
target_include_directories(battery_lib PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(battery_lib PUBLIC lib_base)
install(TARGETS battery_lib LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES battery_lib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/joshua_hal)
