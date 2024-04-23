set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ppc)

set(GNU_MACHINE "powerpc-linux-gnu" CACHE STRING "GNU compiler triple")

set(CMAKE_INSTALL_PREFIX /usr/arm-linux-gnueabi)

set(tools /usr)
set(CMAKE_C_COMPILER ${tools}/bin/arm-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/arm-linux-gnueabi-g++)
