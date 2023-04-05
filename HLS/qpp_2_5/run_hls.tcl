open_project -reset qpp_2_5

set_top qpp

add_files qpp_hls.cpp
add_files qpp_hls.h
add_files -tb test_data.h
add_files -tb qpp_test.cpp

open_solution "solution1" -flow_target vivado
set_part {xc7z020-clg400-1}

config_interface -m_axi_addr64=0

csim_design
