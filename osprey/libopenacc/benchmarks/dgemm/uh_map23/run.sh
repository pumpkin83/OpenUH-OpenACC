nvprof ./pgi_mm 8192
#nvprof --print-summary --events  global_ld_mem_divergence_replays,global_st_mem_divergence_replays,inst_issued1,inst_issued2,inst_executed ./mm 4096
#nvprof --metrics achieved_occupancy,l2_l1_read_hit_rate,gld_transactions ./mm_2 1024
