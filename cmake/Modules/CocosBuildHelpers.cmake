include(CMakeParseArguments)

# copy resource `FILES` and `FOLDERS` to TARGET_FILE_DIR/Resources
function(cocos_copy_target_res cocos_target)
    set(oneValueArgs COPY_TO)
    set(multiValueArgs FILES FOLDERS DIRFILES)
    cmake_parse_arguments(opt "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    add_custom_command(TARGET ${cocos_target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "copying resources..."
    )    
    # copy files
    foreach(cc_file ${opt_FILES})
        get_filename_component(file_name ${cc_file} NAME)
        add_custom_command(TARGET ${cocos_target} POST_BUILD
            #COMMAND ${CMAKE_COMMAND} -E echo "copy file into Resources: ${file_name} ..."
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${cc_file} "${opt_COPY_TO}/${file_name}"
        )
    endforeach()

    foreach(cc_file ${opt_DIRFILES})
        get_filename_component(file_name ${cc_file} NAME)
        if(IS_DIRECTORY ${cc_file})
            add_custom_command(TARGET ${cocos_target} POST_BUILD
                #COMMAND ${CMAKE_COMMAND} -E echo "copy file into Resources: ${file_name} ..."
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${cc_file} "${opt_COPY_TO}/${file_name}"
            )
        else()
            add_custom_command(TARGET ${cocos_target} POST_BUILD
                #COMMAND ${CMAKE_COMMAND} -E echo "copy file into Resources: ${file_name} ..."
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${cc_file} "${opt_COPY_TO}/${file_name}"
            )
        endif()
    endforeach()

    
    # copy folders files
    foreach(cc_folder ${opt_FOLDERS})
        file(GLOB_RECURSE folder_files "${cc_folder}/*")
        get_filename_component(folder_abs_path ${cc_folder} ABSOLUTE)
        foreach(res_file ${folder_files})
            get_filename_component(res_file_abs_path ${res_file} ABSOLUTE)
            file(RELATIVE_PATH res_file_relat_path ${folder_abs_path} ${res_file_abs_path})
            add_custom_command(TARGET ${cocos_target} POST_BUILD
                #COMMAND ${CMAKE_COMMAND} -E echo "copy file into Resources: ${res_file_relat_path} ..."
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${res_file} "${opt_COPY_TO}/${res_file_relat_path}"
            )
        endforeach()
    endforeach()
endfunction()

# mark `FILES` and files in `FOLDERS` as resource files, the destination is `RES_TO` folder
# save all marked files in `res_out`
function(cocos_mark_multi_resources res_out)
    set(oneValueArgs RES_TO)
    set(multiValueArgs FILES FOLDERS)
    cmake_parse_arguments(opt "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(tmp_file_list)
    foreach(cc_file ${opt_FILES})
        get_filename_component(cc_file_abs ${cc_file} ABSOLUTE)
        get_filename_component(file_dir ${cc_file_abs} DIRECTORY)
        cocos_mark_resources(FILES ${cc_file_abs} BASEDIR ${file_dir} RESOURCEBASE ${opt_RES_TO})
    endforeach()
    list(APPEND tmp_file_list ${opt_FILES})

    foreach(cc_folder ${opt_FOLDERS})
        get_filename_component(dir_name ${cc_folder} NAME)
        file(GLOB_RECURSE folder_files "${cc_folder}/*")
        list(APPEND tmp_file_list ${folder_files})
        cocos_mark_resources(FILES ${folder_files} BASEDIR ${cc_folder} RESOURCEBASE ${opt_RES_TO}/${dir_name})
    endforeach()
    set(${res_out} ${tmp_file_list} PARENT_SCOPE)
endfunction()

# get all linked libraries including transitive ones, recursive
function(search_depend_libs_recursive cocos_target all_depends_out)
    set(all_depends_inner)
    set(targets_prepare_search ${cocos_target})
    while(true)
        foreach(tmp_target ${targets_prepare_search})
            get_target_property(target_type ${tmp_target} TYPE)
            if(${target_type} STREQUAL "SHARED_LIBRARY" OR ${target_type} STREQUAL "STATIC_LIBRARY" OR ${target_type} STREQUAL "MODULE_LIBRARY" OR ${target_type} STREQUAL "EXECUTABLE")
                #interface-only libraries do not support certain properties such as LINK_LIBRARIES
                get_target_property(tmp_depend_libs ${tmp_target} LINK_LIBRARIES)
                list(REMOVE_ITEM targets_prepare_search ${tmp_target})
                list(APPEND tmp_depend_libs ${tmp_target})
                foreach(depend_lib ${tmp_depend_libs})
                    if(TARGET ${depend_lib})
                        list(APPEND all_depends_inner ${depend_lib})
                        if(NOT (depend_lib STREQUAL tmp_target))
                            list(APPEND targets_prepare_search ${depend_lib})
                        endif()
                    endif()
                endforeach()
            else()
                list(REMOVE_ITEM targets_prepare_search ${tmp_target})
            endif()
        endforeach()
        list(LENGTH targets_prepare_search targets_prepare_search_size)
        if(targets_prepare_search_size LESS 1)
            break()
        endif()
    endwhile(true)
    set(${all_depends_out} ${all_depends_inner} PARENT_SCOPE)
endfunction()

# mark `FILES` as resources, files will be put into sub-dir tree depend on its absolute path
function(cocos_mark_resources)
    set(oneValueArgs BASEDIR RESOURCEBASE)
    set(multiValueArgs FILES)
    cmake_parse_arguments(opt "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT opt_RESOURCEBASE)
        set(opt_RESOURCEBASE Resources)
    endif()

    get_filename_component(BASEDIR_ABS ${opt_BASEDIR} ABSOLUTE)
    foreach(RES_FILE ${opt_FILES} ${opt_UNPARSED_ARGUMENTS})
        get_filename_component(RES_FILE_ABS ${RES_FILE} ABSOLUTE)
        file(RELATIVE_PATH RES ${BASEDIR_ABS} ${RES_FILE_ABS})
        get_filename_component(RES_LOC ${RES} PATH)
        set_source_files_properties(${RES_FILE} PROPERTIES
                                    MACOSX_PACKAGE_LOCATION ${opt_RESOURCEBASE}/${RES_LOC}
                                    HEADER_FILE_ONLY 1
                                    )
        if(XCODE OR VS)
            string(REPLACE "/" "\\" ide_source_group "${opt_RESOURCEBASE}/${RES_LOC}")
            source_group("${ide_source_group}" FILES ${RES_FILE_ABS})
        endif()
    endforeach()
endfunction()

# mark the code sources of `cocos_target` into sub-dir tree
function(cocos_mark_code_files cocos_target)
    set(oneValueArgs GROUPBASE)
    cmake_parse_arguments(opt "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if(NOT opt_GROUPBASE)
        set(root_dir ${CMAKE_CURRENT_SOURCE_DIR})
    else()
        set(root_dir ${opt_GROUPBASE})
        message(STATUS "target ${cocos_target} code group base is: ${root_dir}")
    endif()

    get_property(file_list TARGET ${cocos_target} PROPERTY SOURCES)

    foreach(single_file ${file_list})
        source_group_single_file(${single_file} GROUP_TO "Source Files" BASE_PATH "${root_dir}")
    endforeach()

endfunction()

# source group one file
# cut the `single_file` absolute path from `BASE_PATH`, then mark file to `GROUP_TO`
function(source_group_single_file single_file)
    set(oneValueArgs GROUP_TO BASE_PATH)
    cmake_parse_arguments(opt "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    # get relative_path
    get_filename_component(abs_path ${single_file} ABSOLUTE)
    file(RELATIVE_PATH relative_path_with_name ${opt_BASE_PATH} ${abs_path})
    get_filename_component(relative_path ${relative_path_with_name} PATH)
    # set source_group, consider sub source group
    string(REPLACE "/" "\\" ide_file_group "${opt_GROUP_TO}/${relative_path}")
    source_group("${ide_file_group}" FILES ${single_file})
endfunction()

# setup a cocos application
function(setup_cocos_app_config app_name)
    # put all output app into bin/${app_name}

    if((XCODE OR MSVC) AND OUTPUT_DIRECTORY)
        set_target_properties(${app_name} PROPERTIES 
            RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}"
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}"
            )
    else()
        set_target_properties(${app_name} PROPERTIES 
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${app_name}")
    endif()

    if(APPLE)
        # output macOS/iOS .app
        set_target_properties(${app_name} PROPERTIES MACOSX_BUNDLE 1)
        #use_cocos2dx_libs_depend(${app_name})
    elseif(MSVC)
        # visual studio default is Console app, but we need Windows app
        set_property(TARGET ${app_name} APPEND PROPERTY LINK_FLAGS "/SUBSYSTEM:WINDOWS")
    endif()
    # auto mark code files for IDE when mark app
    if(XCODE OR VS)
        cocos_mark_code_files(${app_name})
    endif()
    
endfunction()

# if cc_variable not set, then set it cc_value
macro(cocos_fake_set cc_variable cc_value)
    if(NOT DEFINED ${cc_variable})
        set(${cc_variable} ${cc_value})
    endif()
endmacro()

# set Xcode property for application, include all depend target
macro(cocos_config_app_xcode_property cocos_app)
    set(depend_libs)
    search_depend_libs_recursive(${cocos_app} depend_libs)
    foreach(depend_lib ${depend_libs})
        if(TARGET ${depend_lib})
            cocos_config_target_xcode_property(${depend_lib})
        endif()
    endforeach()
endmacro()

# custom Xcode property for iOS target
macro(cocos_config_target_xcode_property cocos_target)
    if(IOS)
        set_xcode_property(${cocos_target} IPHONEOS_DEPLOYMENT_TARGET "8.0")
        set_xcode_property(${cocos_target} ENABLE_BITCODE "NO")
        set_xcode_property(${cocos_target} ONLY_ACTIVE_ARCH "YES")
    endif()
endmacro()

# This little macro lets you set any XCode specific property, from ios.toolchain.cmake
function(set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
    set_property(TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY} ${XCODE_VALUE})
endfunction(set_xcode_property)
