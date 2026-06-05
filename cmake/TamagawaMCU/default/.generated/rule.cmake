# The following functions contains all the flags passed to the different build stages.

set(PACK_REPO_PATH "C:/Users/wsh/.mchp_packs" CACHE PATH "Path to the root of a pack repository.")

function(TamagawaMCU_default_default_XC16_assemble_rule target)
    set(options
        "-g"
        "-mcpu=33CK256MP502"
        "-Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-g,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,--no-relax"
        "-mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33CK-MP_DFP/1.15.423/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG=1"
        PRIVATE "__MPLAB_DEBUGGER_SIMULATOR=1"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target}
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../TamagawaMCU/mcc"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../TamagawaMCU/mcc/user"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../TamagawaMCU/mcc/mcc_generated_files")
endfunction()
function(TamagawaMCU_default_default_XC16_assemblePreproc_rule target)
    set(options
        "-x"
        "assembler-with-cpp"
        "-g"
        "-mcpu=33CK256MP502"
        "-Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-g,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,--no-relax"
        "-mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33CK-MP_DFP/1.15.423/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG=1"
        PRIVATE "__MPLAB_DEBUGGER_SIMULATOR=1"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target}
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../TamagawaMCU/mcc"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../TamagawaMCU/mcc/user"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../TamagawaMCU/mcc/mcc_generated_files")
endfunction()
function(TamagawaMCU_default_default_XC16_compile_rule target)
    set(options
        "-g"
        "-mcpu=33CK256MP502"
        "-ffunction-sections"
        "-mlarge-code"
        "-O0"
        "-msmart-io=1"
        "-Wall"
        "-msfr-warn=off"
        "-mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33CK-MP_DFP/1.15.423/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG"
        PRIVATE "__MPLAB_DEBUGGER_SIMULATOR=1"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target}
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../TamagawaMCU/mcc"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../TamagawaMCU/mcc/user"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../TamagawaMCU/mcc/mcc_generated_files")
endfunction()
function(TamagawaMCU_default_dependentObject_rule target)
    set(options
        "-c"
        "-mcpu=33CK256MP502"
        "-mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33CK-MP_DFP/1.15.423/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
endfunction()
function(TamagawaMCU_default_link_rule target)
    set(options
        "-g"
        "-mcpu=33CK256MP502"
        "-Wl,--script=p33CK256MP502.gld,--local-stack,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,--heap=0,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,memoryfile.xml"
        "-mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33CK-MP_DFP/1.15.423/xc16")
    list(REMOVE_ITEM options "")
    target_link_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG=__DEBUG"
        PRIVATE "__MPLAB_DEBUGGER_SIMULATOR=1"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target}
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../TamagawaMCU/mcc"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../TamagawaMCU/mcc/user"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../TamagawaMCU/mcc/mcc_generated_files")
endfunction()
function(TamagawaMCU_default_bin2hex_rule target)
    add_custom_target(
        TamagawaMCU_default_Bin2Hex ALL
        COMMAND ${MP_BIN2HEX} ${TamagawaMCU_default_image_name} -a -mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33CK-MP_DFP/1.15.423/xc16
        WORKING_DIRECTORY ${TamagawaMCU_default_output_dir}
        BYPRODUCTS "${TamagawaMCU_default_output_dir}/${TamagawaMCU_default_image_base_name}.hex"
        COMMENT "Convert build file to .hex")
    add_dependencies(TamagawaMCU_default_Bin2Hex ${target})
endfunction()
