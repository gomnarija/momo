set(LIB_SOURCES
${SOURCE_DIR}/tokenizer.cpp
${SOURCE_DIR}/reader.cpp
${SOURCE_DIR}/types.cpp
${SOURCE_DIR}/eval.cpp
${SOURCE_DIR}/core.cpp
${SOURCE_DIR}/utils.cpp
${SOURCE_DIR}/log.cpp)
set(LIB_SOURCES ${LIB_SOURCES} PARENT_SCOPE)

set(HEADER_FILES
${SOURCE_DIR}/tokenizer.h
${SOURCE_DIR}/reader.h
${SOURCE_DIR}/momo.h
${SOURCE_DIR}/log.h)
set(HEADER_FILES ${HEADER_FILES} PARENT_SCOPE)
