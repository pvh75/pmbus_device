add_library(freertos_addons INTERFACE)

target_sources(freertos_addons INTERFACE
	./c++/Source/ccondition_variable.cpp
	./c++/Source/cevent_groups.cpp
	./c++/Source/cmem_pool.cpp
	./c++/Source/cmutex.cpp
	./c++/Source/cqueue.cpp
	./c++/Source/cread_write_lock.cpp
	./c++/Source/csemaphore.cpp
	./c++/Source/ctasklet.cpp
	./c++/Source/cthread.cpp
	./c++/Source/ctickhook.cpp
	./c++/Source/ctimer.cpp
	./c++/Source/cworkqueue.cpp
)

target_include_directories(freertos_addons INTERFACE
	./c++/Source/include
)

target_compile_definitions(freertos_addons INTERFACE
	CPP_FREERTOS_NO_CPP_STRINGS
	CPP_FREERTOS_NO_EXCEPTIONS
)
