add_library(common
                index.hpp
                concepts.hpp
                common.hpp
                dictionary.hpp
                doc_inv_index.hpp
                kgram.hpp
                preprocess.hpp
                doc_info.hpp
                vec.hpp
                lead_follow_inv_index.hpp
                find_in_dict.hpp
                vec_of_tokens.hpp
                common_serialization.hpp
                wildcard_match.hpp
                debug.hpp
                cos_dist.hpp
                dummy.cpp
                json.hpp)

target_link_libraries(common spdlog::spdlog)