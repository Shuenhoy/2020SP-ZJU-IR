add_executable(ir 
    main.cpp
    index_op.hpp
    phrase.hpp
    boolea.hpp
    topk.hpp
    wildcard.hpp
    spelling_correction.hpp
)
target_link_libraries(ir spdlog::spdlog)
