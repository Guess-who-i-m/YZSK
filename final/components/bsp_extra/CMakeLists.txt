file(GLOB_RECURSE SRCS src/*.c)

set(priv_requires "esp-box-3")

idf_component_register(
    SRCS ${SRCS}
    INCLUDE_DIRS "include"
    PRIV_REQUIRES ${priv_requires})
# )
