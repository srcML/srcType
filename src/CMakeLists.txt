file(GLOB SRCSAXHANDLER_SOURCE ../../srcSAX/src/srcsax/*.cpp ../../srcSAX/src/cpp/*.cpp)
file(GLOB SRCSAXHANDLER_INCLUDE ../../srcSAX/src/srcsax/*.h* ../../srcSAX/src/cpp/*.h*)
file(GLOB SRCTYPEHANDLER_SOURCE cpp/*.cpp)
file(GLOB SRCTYPEHANDLER_INCLUDE headers/*.hpp)
file(GLOB SRCTYPETEST_SOURCE tests/*.cpp)

add_executable(srctypetest ${SRCTYPEHANDLER_SOURCE} ${SRCTYPEHANDLER_INCLUDE} ${SRCTYPETEST_SOURCE})
add_library(srctype ${SRCTYPEHANDLER_SOURCE} ${SRCTYPEHANDLER_INCLUDE})

target_link_libraries(srctype srcsax ${LIBXML2_LIBRARIES})
target_link_libraries(srctypetest srcsax ${LIBXML2_LIBRARIES})