
# define a function for creating Python virtual environment
function(create_venv venv_dir requirements_path)
    # check if the virtual environment already exists
    if(EXISTS ${venv_dir})
        message(STATUS "Virtual environment already exists in ${venv_dir}, skipping creation.")
        return()
    endif()
    # ensure that the given requirements.txt file exists
    if(NOT EXISTS ${requirements_path})
        message(FATAL_ERROR "Requirements file not found: ${requirements_path}")
    endif()
    # create the virtual environment
    execute_process(
        COMMAND ${Python3_EXECUTABLE} -m venv ${venv_dir}
        RESULT_VARIABLE venv_creation_ret_code
    )
    # report error if return code is non-zero
    if(venv_creation_ret_code)
        message(FATAL_ERROR "Failed to create virtual environment at ${venv_dir}!")
    endif()
    # install dependencies from requirements.txt
    if(WIN32)
        execute_process(
            COMMAND ${venv_dir}/Scripts/pip install -r ${requirements_path}
            RESULT_VARIABLE pip_install_ret_code
        )
    else()
        execute_process(
            COMMAND ${venv_dir}/bin/pip install -r ${requirements_path}
            RESULT_VARIABLE pip_install_ret_code
        )
    endif()

    # report error if return code is non-zero
    if(pip_install_ret_code)
        message(FATAL_ERROR "Failed to install dependencies from ${requirements_path}!")
    endif()
    # print success message
    message(STATUS "Virtual environment setup done at ${venv_dir} with dependencies from ${requirements_path}")
endfunction()

# Step 1: Find system Python to bootstrap venv
find_package(Python3 COMPONENTS Interpreter REQUIRED)

# Step 2: Create virtual environment
create_venv(${CMAKE_CURRENT_SOURCE_DIR}/venv ${CMAKE_CURRENT_SOURCE_DIR}/requirements.txt)

# Step 3: Point Python3_EXECUTABLE to venv interpreter
if(WIN32)
    set(Python3_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/venv/Scripts/python.exe CACHE FILEPATH "Python executable" FORCE)
else()
    set(Python3_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/venv/bin/python3 CACHE FILEPATH "Python executable" FORCE)
    set(Python3_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/venv/)
endif()

# Step 4: Discover Python dev headers/libraries with venv interpreter
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
