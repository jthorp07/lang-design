# Step 1 tokenizer executable
set(STEP_TWO_EXE step_two)
add_executable(${STEP_TWO_EXE})
set_target_properties(${STEP_TWO_EXE} PROPERTIES VERSION 0.0.0 SOVERSION 0)
target_sources(${STEP_TWO_EXE} PRIVATE src/step_one.cpp src/tokenizer.cpp)

if (NOT DECLARE_A_STRING_IMP)
    set(DECLARE_A_STRING_IMP "${CMAKE_SOURCE_DIR}/test_data/declare_a_string.imp")
    message(STATUS "Step 2 data file path: ${DECLARE_A_STRING_IMP}")
endif()

# Script Targets
add_custom_target(run_two
        COMMENT "Run with default file"
        MESSAGE(STATUS "Running step two")
        COMMAND $<TARGET_FILE:${STEP_TWO_EXE}> ${DECLARE_A_STRING_IMP}
        DEPENDS ${STEP_TWO_EXE}
        USES_TERMINAL
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
