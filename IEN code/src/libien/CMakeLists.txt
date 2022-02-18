cmake_minimum_required (VERSION 3.8)

project(libien)

include_directories(../zlib-1.2.11 ../mbedtls-2.16.1/include ../sqlite3-3.29.0 ../libuv-1.29.1/include ../log4cplus-2.0.7/include)

aux_source_directory(. LIB_SRC)
aux_source_directory(./commmon COMMON_SRC)
aux_source_directory(./base BASE_SRC)
aux_source_directory(./thread THREAD_SRC)
aux_source_directory(./protocol PROTOCOL_SRCS)
aux_source_directory(./history HISTORY_SRCS)
aux_source_directory(./history/sqlite-storage SQLITE_STORAGE_SRCS)
aux_source_directory(./interface INTERFACE_SRCS)
aux_source_directory(./sn_client SN_CLIENT_SRCS)
aux_source_directory(./tunnel TUNNEL_SRCS)
aux_source_directory(./connection CONNECTION_SRCS)
set(SRCS 
${COMMON_SRC} 
${BASE_SRC} 
${THREAD_SRC}
${GLOBAL_SRCS}
${PROTOCOL_SRCS}
${HISTORY_SRCS}
${SQLITE_STORAGE_SRCS}
${INTERFACE_SRCS} 
${SN_CLIENT_SRCS}
${TUNNEL_SRCS} 
${CONNECTION_SRCS} 
${LIB_SRC}
)

add_library(${PROJECT_NAME} STATIC ${SRCS})