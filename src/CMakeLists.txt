file(GLOB SRCSAXHANDLER_SOURCE ../../srcSAX/src/srcsax/*.cpp ../../srcSAX/src/cpp/*.cpp)
file(GLOB SRCSAXHANDLER_INCLUDE ../../srcSAX/src/srcsax/*.h* ../../srcSAX/src/cpp/*.h*)
file(GLOB SRCTYPEHANDLER_SOURCE cpp/*.cpp)
file(GLOB SRCTYPEHANDLER_INCLUDE headers/*.hpp)
file(GLOB SRCTYPETEST_SOURCE tests/*.cpp)

link_directories(/usr/local/lib/)

add_library(srctype ${SRCTYPEHANDLER_SOURCE} ${SRCTYPEHANDLER_INCLUDE})
add_executable(srctypetest ${SRCTYPEHANDLER_SOURCE} ${SRCTYPEHANDLER_INCLUDE} ${SRCTYPETEST_SOURCE})

target_link_libraries(srctype srcsax_static ${LIBXML2_LIBRARIES})
target_link_libraries(srctypetest srcsax_static srcml ${LIBXML2_LIBRARIES})