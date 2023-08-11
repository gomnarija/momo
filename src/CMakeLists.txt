set(SOURCE_FILES 
${SOURCE_DIR}/tokenizer.cpp
${SOURCE_DIR}/reader.cpp
${SOURCE_DIR}/types.cpp
${SOURCE_DIR}/eval.cpp
${SOURCE_DIR}/core.cpp
${SOURCE_DIR}/utils.cpp
${SOURCE_DIR}/log.cpp
${SOURCE_DIR}/momo.cpp)
set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)

set(HEADER_FILES
${SOURCE_DIR}/tokenizer.h
${SOURCE_DIR}/reader.h
${SOURCE_DIR}/momo.h)
set(HEADER_FILES ${HEADER_FILES} PARENT_SCOPE)
