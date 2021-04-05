macro( createEXE EXE_NAME SOURCES )
	project(${EXE_NAME})
	add_executable(${EXE_NAME} main.cpp ${SOURCES})
	add_dependencies(${EXE_NAME} rizer)
	target_include_directories(${EXE_NAME}
		PUBLIC ../../include
	)
	target_link_libraries(${EXE_NAME} rizer)
	find_package(OpenGL REQUIRED)
	target_link_libraries(${EXE_NAME} ${OPENGL_LIBRARIES})
endmacro()