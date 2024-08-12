set(ASSETS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(ASSETS_DEST_DIR ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_SOURCE_DIR}/levels ${ASSETS_DEST_DIR}/levels
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_SOURCE_DIR}/shaders ${ASSETS_DEST_DIR}/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_SOURCE_DIR}/textures ${ASSETS_DEST_DIR}/textures
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_SOURCE_DIR}/config ${ASSETS_DEST_DIR}/config
    COMMENT "Copying assets to build directory"
)
