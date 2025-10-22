set(CmakeConfig.cmake $ENV{MBG_Config})
include(${CmakeConfig.cmake})
message(STATUS "Audio Server CMAKE_BINARY_DIR  is: ${CMAKE_BINARY_DIR}")

set(CMAKE_PREFIX_PATH $ENV{Qt5_12})  # Adjust to your Qt install path

# Find Qt
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Network)

# Local header files here
SET(TARGET_H
    MainWindow.h
    Connection.h
    DataGeneratorThread.h
    DataSourceStructure.h
    )

# Local source files here
SET(TARGET_SRC
    MainWindow.cpp
    Connection.cpp
    DataGeneratorThread.cpp
    main.cpp
    )

add_executable(ByteCourier ${TARGET_SRC} ${TARGET_H})

# Link Qt libraries
target_link_libraries(ByteCourier PUBLIC Qt5::Core Qt5::Widgets Qt5::Network)

#The binary will be copied to the bin folder
#The compiler is Visual Studio 16 2019
#The binary is build under Debug configuration because MSVC has multiple build configurations.
add_custom_command(TARGET ByteCourier POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/ClientServer/ByteCourier.exe"
    "${CMAKE_SOURCE_DIR}/bin/ByteCourier.exe" USES_TERMINAL COMMAND_EXPAND_LISTS)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/images DESTINATION ${CMAKE_SOURCE_DIR}/bin)
