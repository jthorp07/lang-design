# Step 1 tokenizer executable
set(STEP_ONE_EXE step_one)
add_executable(${STEP_ONE_EXE})
set_target_properties(${STEP_ONE_EXE} PROPERTIES VERSION 0.0.0 SOVERSION 0)
target_sources(${STEP_ONE_EXE} PRIVATE src/step_one.cpp src/tokenizer.cpp)

if (NOT DECLARE_A_STRING_IMP)
    set(DECLARE_A_STRING_IMP "${CMAKE_SOURCE_DIR}/test_data/declare_a_string.imp")
    message(STATUS "Step 1 data file path: ${DECLARE_A_STRING_IMP}")
endif()

# Script Targets
add_custom_target(run_one 
        COMMENT "Run with default file"
        COMMAND $<TARGET_FILE:${STEP_ONE_EXE}> ${DECLARE_A_STRING_IMP}
        DEPENDS ${STEP_ONE_EXE}
        USES_TERMINAL
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
