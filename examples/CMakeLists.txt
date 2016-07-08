file(GLOB SRCSAXHANDLER_SOURCE ../srcSAX/src/srcsax/*.cpp ../srcSAX/src/cpp/*.cpp)
file(GLOB SRCSAXHANDLER_INCLUDE ../srcSAX/src/srcsax/*.h* ../srcSAX/src/cpp/*.h*)
file(GLOB SRCTYPEHANDLER_SOURCE ../src/cpp/*.cpp)
file(GLOB SRCTYPEHANDLER_INCLUDE ../src/headers/*.hpp)
file(GLOB SRCTYPEEXAMPLE_SOURCE src/*.cpp)

link_directories(/usr/local/lib/)

add_executable(srctypeexample ${SRCTYPEHANDLER_SOURCE} ${SRCTYPEEXAMPLE_SOURCE})
target_link_libraries(srctypeexample srcsax_static ${LIBXML2_LIBRARIES})