add_library(rs485_lib rs485_lib.cpp rs485_lib.h)
target_include_directories(rs485_lib PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(rs485_lib PUBLIC lib_base)
install(TARGETS rs485_lib LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES rs485_lib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/joshua_hal)
