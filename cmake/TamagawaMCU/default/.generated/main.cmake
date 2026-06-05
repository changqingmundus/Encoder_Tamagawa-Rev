include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(TamagawaMCU_default_library_list )

# Handle files with suffix s, for group default-XC16
if(TamagawaMCU_default_default_XC16_FILE_TYPE_assemble)
add_library(TamagawaMCU_default_default_XC16_assemble OBJECT ${TamagawaMCU_default_default_XC16_FILE_TYPE_assemble})
    TamagawaMCU_default_default_XC16_assemble_rule(TamagawaMCU_default_default_XC16_assemble)
    list(APPEND TamagawaMCU_default_library_list "$<TARGET_OBJECTS:TamagawaMCU_default_default_XC16_assemble>")

endif()

# Handle files with suffix S, for group default-XC16
if(TamagawaMCU_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(TamagawaMCU_default_default_XC16_assemblePreproc OBJECT ${TamagawaMCU_default_default_XC16_FILE_TYPE_assemblePreproc})
    TamagawaMCU_default_default_XC16_assemblePreproc_rule(TamagawaMCU_default_default_XC16_assemblePreproc)
    list(APPEND TamagawaMCU_default_library_list "$<TARGET_OBJECTS:TamagawaMCU_default_default_XC16_assemblePreproc>")

endif()

# Handle files with suffix c, for group default-XC16
if(TamagawaMCU_default_default_XC16_FILE_TYPE_compile)
add_library(TamagawaMCU_default_default_XC16_compile OBJECT ${TamagawaMCU_default_default_XC16_FILE_TYPE_compile})
    TamagawaMCU_default_default_XC16_compile_rule(TamagawaMCU_default_default_XC16_compile)
    list(APPEND TamagawaMCU_default_library_list "$<TARGET_OBJECTS:TamagawaMCU_default_default_XC16_compile>")

endif()

# Handle files with suffix s, for group default-XC16
if(TamagawaMCU_default_default_XC16_FILE_TYPE_dependentObject)
add_library(TamagawaMCU_default_default_XC16_dependentObject OBJECT ${TamagawaMCU_default_default_XC16_FILE_TYPE_dependentObject})
    TamagawaMCU_default_default_XC16_dependentObject_rule(TamagawaMCU_default_default_XC16_dependentObject)
    list(APPEND TamagawaMCU_default_library_list "$<TARGET_OBJECTS:TamagawaMCU_default_default_XC16_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC16
if(TamagawaMCU_default_default_XC16_FILE_TYPE_bin2hex)
add_library(TamagawaMCU_default_default_XC16_bin2hex OBJECT ${TamagawaMCU_default_default_XC16_FILE_TYPE_bin2hex})
    TamagawaMCU_default_default_XC16_bin2hex_rule(TamagawaMCU_default_default_XC16_bin2hex)
    list(APPEND TamagawaMCU_default_library_list "$<TARGET_OBJECTS:TamagawaMCU_default_default_XC16_bin2hex>")

endif()


# Main target for this project
add_executable(TamagawaMCU_default_image_4YQUZzB0 ${TamagawaMCU_default_library_list})

set_target_properties(TamagawaMCU_default_image_4YQUZzB0 PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${TamagawaMCU_default_output_dir}")
target_link_libraries(TamagawaMCU_default_image_4YQUZzB0 PRIVATE ${TamagawaMCU_default_default_XC16_FILE_TYPE_link})

# Add the link options from the rule file.
TamagawaMCU_default_link_rule( TamagawaMCU_default_image_4YQUZzB0)

# Call bin2hex function from the rule file
TamagawaMCU_default_bin2hex_rule(TamagawaMCU_default_image_4YQUZzB0)

