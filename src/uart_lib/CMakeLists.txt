add_library(uart_lib uart_lib.cpp uart_lib.h)
target_include_directories(uart_lib PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(uart_lib PUBLIC lib_base)
install(TARGETS uart_lib LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES uart_lib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/joshua_hal)
