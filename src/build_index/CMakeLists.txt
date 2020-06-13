add_executable(ir_build_index main.cpp raw_inv_index.hpp kgram.hpp doc_info.hpp lead_follow.hpp raw.hpp)
target_link_libraries(ir_build_index spdlog::spdlog)