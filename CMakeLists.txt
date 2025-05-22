CMAKE_MINIMUM_REQUIRED(VERSION 3.28)
PROJECT(crazy_cat)

# option(BUILD_CLIENT "builds client" ON)
# option(BUILD_FEED_SERVER "builds feed server" ON)
# option(BUILD_PET_SERVER "builds pet server" ON)
# option(BUILD_STATS_SERVER "builds stats server" ON)

include_directories(${CMAKE_SOURCE_DIR}/enviroments)

# IF(BUILD_CLIENT)
#     add_subdirectory(client)
# endif()

# IF(BUILD_FEED_SERVER)
#     add_subdirectory(feed_the_cat)
# endif()

add_subdirectory(Cat)
add_subdirectory(Client)
add_subdirectory(Handler)
add_subdirectory(Server)
add_subdirectory(feed_the_cat)
