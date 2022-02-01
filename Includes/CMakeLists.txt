add_subdirectory(minhook)

target_include_directories(Dx11_SC PRIVATE "${CMAKE_CURRENT_LIST_DIR}/minhook/include")