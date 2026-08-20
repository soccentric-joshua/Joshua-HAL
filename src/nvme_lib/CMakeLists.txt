add_library(nvme_lib nvme_lib.cpp nvme_lib.h)
target_include_directories(nvme_lib PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(nvme_lib PUBLIC lib_base)
install(TARGETS nvme_lib LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES nvme_lib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/joshua_hal)
