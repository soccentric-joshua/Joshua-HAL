add_library(spi_lib spi_lib.cpp spi_lib.h)
target_include_directories(spi_lib PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(spi_lib PUBLIC lib_base)
install(TARGETS spi_lib LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES spi_lib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/joshua_hal)
