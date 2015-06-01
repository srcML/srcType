file(GLOB SRCSAXHANDLER_SOURCE ../../srcSAX/src/srcsax/*.cpp ../../srcSAX/src/cpp/*.cpp)
file(GLOB SRCSAXHANDLER_INCLUDE ../../srcSAX/src/srcsax/*.h* ../../srcSAX/src/cpp/*.h*)
file(GLOB SRCTYPEHANDLER_SOURCE cpp/*.cpp)
file(GLOB SRCTYPEHANDLER_INCLUDE headers/*.hpp)

add_library(srcType ${SRCTYPEHANDLER_SOURCE} ${SRCTYPEHANDLER_INCLUDE})

target_link_libraries(srcType srcsax ${LIBXML2_LIBRARIES})