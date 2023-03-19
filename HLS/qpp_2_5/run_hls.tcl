open_project -reset qpp_2_5

set_top qpp

add_files qpp_hls.cpp
add_files qpp_hls.h
add_files -tb test_data.h -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb qpp_test.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"

open_solution "solution1" -flow_target vivado
set_part {xc7z020-clg400-1}


