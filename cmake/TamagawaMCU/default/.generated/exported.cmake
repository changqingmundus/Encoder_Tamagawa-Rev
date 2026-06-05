set(DEPENDENT_MP_BIN2HEXTamagawaMCU_default_4YQUZzB0 "d:/Microchip/xc16/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFTamagawaMCU_default_4YQUZzB0 ${CMAKE_CURRENT_LIST_DIR}/../../../../out/TamagawaMCU/default.elf)
set(DEPENDENT_TARGET_DIRTamagawaMCU_default_4YQUZzB0 ${CMAKE_CURRENT_LIST_DIR}/../../../../out/TamagawaMCU)
set(DEPENDENT_BYPRODUCTSTamagawaMCU_default_4YQUZzB0 ${DEPENDENT_TARGET_DIRTamagawaMCU_default_4YQUZzB0}/${sourceFileNameTamagawaMCU_default_4YQUZzB0}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRTamagawaMCU_default_4YQUZzB0}/${sourceFileNameTamagawaMCU_default_4YQUZzB0}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXTamagawaMCU_default_4YQUZzB0} ${DEPENDENT_DEPENDENT_TARGET_ELFTamagawaMCU_default_4YQUZzB0} --image ${sourceFileNameTamagawaMCU_default_4YQUZzB0} ${addressTamagawaMCU_default_4YQUZzB0} ${modeTamagawaMCU_default_4YQUZzB0} -mdfp=C:/Users/wsh/.mchp_packs/Microchip/dsPIC33CK-MP_DFP/1.15.423/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRTamagawaMCU_default_4YQUZzB0}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFTamagawaMCU_default_4YQUZzB0})
add_custom_target(
    dependent_produced_source_artifactTamagawaMCU_default_4YQUZzB0 
    DEPENDS ${DEPENDENT_TARGET_DIRTamagawaMCU_default_4YQUZzB0}/${sourceFileNameTamagawaMCU_default_4YQUZzB0}.s
    )
