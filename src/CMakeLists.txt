set(BREAKOUT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/texture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sprite.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/resource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game_object.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game_stage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/app.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

set(BREAKOUT_SOURCES ${BREAKOUT_SOURCES} PARENT_SCOPE)
