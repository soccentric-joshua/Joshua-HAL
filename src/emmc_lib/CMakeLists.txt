add_library(emmc_lib emmc_lib.cpp emmc_lib.h)
target_include_directories(emmc_lib PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(emmc_lib PUBLIC lib_base)
install(TARGETS emmc_lib LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES emmc_lib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/joshua_hal)
