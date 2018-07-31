# Append coverage to targets and run coverage function support
# inspired by https://github.com/pyarmak/cmake-gtest-coverage-example/blob/master/cmake/modules/CodeCoverage.cmake

#[[ Require coverage macro
    Search for gcov, lcov and genhtml binaries and abort if they are not found.
    Params:
      - _gcovPath : the string where is expanded gcov path (if any)
      - _lcovPath : the string where is expanded lcov path (if any)
      - _genhtmlPath : the string where is expanded genhtml path (if any)
]]
macro (SCC_APP_REQUIRE_COVERAGE _gcovPath _lcovPath _genhtmlPath)
    find_program(${_gcovPath} gcov)
    find_program(${_lcovPath} lcov)
    find_program(${_genhtmlPath} genhtml)

    if(NOT ${_gcovPath})
        message(FATAL_ERROR "gcov not found. Aborting...")
    endif() # NOT ${_gcovPath}

    if(NOT ${_lcovPath})
        message(FATAL_ERROR "lcov not found. Aborting...")
    endif() # NOT ${_lcovPath}

    if(NOT ${_genhtmlPath})
        message(FATAL_ERROR "genhtml not found. Aborting...")
    endif() # NOT ${_genhtmlPath}

    if(NOT CMAKE_COMPILER_IS_GNUCXX)
        # Clang version 3.0.0 and greater now supports gcov as well.
        message(WARNING "Compiler is not GNU gcc! Clang Version 3.0.0 and greater supports gcov as well, but older versions don't.")

        if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            message(FATAL_ERROR "Compiler is not GNU gcc! Aborting...")
        endif()
    endif() # NOT CMAKE_COMPILER_IS_GNUCXX
endmacro () # SCC_APP_REQUIRE_COVERAGE


#[[ Compile with coverage support
    If gcov, lcov and genhtml are found, then add private options to compiler, in order to run with coverage
    Params:
      - _target_to_cover : the target variable to add the target options to
]]
macro (SCC_APP_COMPILE_WITH_COV _target_to_cover)
    SCC_APP_REQUIRE_COVERAGE(GCOV_PATH LCOV_PATH GENHTML_PATH)

    target_compile_options(${_target_to_cover}
        PRIVATE
            $<$<CONFIG:Debug>:-g -O0 --coverage -fprofile-arcs -ftest-coverage>)

endmacro () # SCC_APP_COMPILE_WITH_COV


#[[ Add coverage reports
    If gcov, lcov and genhtml are found, then generate coverage reports after a certain target is run.
    Moreover add the coverage flag to link flags, so make sure your static library builds with coverage support.
    Params:
      - _target_to_cover : the target to analise coverage when running
      - _outputname : the folder output name where to generate html files
]]
function (scc_app_add_coverage_to _target_to_cover _outputname)

    SCC_APP_COMPILE_WITH_COV(${_target_to_cover})

    separate_arguments(splitArgvs UNIX_COMMAND ${ARGV2})

    set_target_properties(${_target_to_cover}
        PROPERTIES LINK_FLAGS --coverage)

    # Setup target
    add_custom_target(${_target_to_cover}_coverage

        # Cleanup lcov
        COMMAND ${LCOV_PATH} --directory . --zerocounters

        # Run tests
        COMMAND ${_target_to_cover} ${splitArgvs}

        # Capturing lcov counters and generating report
        COMMAND ${LCOV_PATH} --directory . --capture --output-file ${_outputname}.info
        COMMAND ${LCOV_PATH} --remove ${_outputname}.info 'build/*' 'tests/*' '/usr/*' --output-file ${_outputname}.info.cleaned
        COMMAND ${GENHTML_PATH} -o ${_outputname} ${_outputname}.info.cleaned
        COMMAND ${CMAKE_COMMAND} -E remove ${_outputname}.info ${_outputname}.info.cleaned

        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report.")

    # Show info where to find the report
    add_custom_command(TARGET ${_target_to_cover}_coverage POST_BUILD
        COMMAND ;
        COMMENT "Open ./${_outputname}/index.html in your browser to view the coverage report.")

endfunction() # setup_target_for_coverage
