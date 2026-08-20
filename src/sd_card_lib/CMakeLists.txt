add_library(sd_card_lib sd_card_lib.cpp sd_card_lib.h)
target_include_directories(sd_card_lib PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sd_card_lib PUBLIC lib_base)
install(TARGETS sd_card_lib LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES sd_card_lib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/joshua_hal)
