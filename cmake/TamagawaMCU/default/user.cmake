# 全域添加包含路徑，底下的所有 Target 都能直接使用
include_directories(
    "${CMAKE_CURRENT_LIST_DIR}/mcc"
    "${CMAKE_CURRENT_LIST_DIR}/mcc/mcc_generated_files"
)