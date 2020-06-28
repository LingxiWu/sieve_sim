import random
import math
###### define constants ######

### DDR3_32M_8B_x4 ###
#NUM_BANK = 8
#NUM_ROW = 16384
#NUM_COL = 2048
#DEVICE_WIDTH = 4 
### END DDR3_32M_8B_X4 ###

### DDR3_16M_8B_x8 ###
#NUM_BANK = 8
#NUM_ROW = 16384
#NUM_COL = 1024
#DEVICE_WIDTH = 16
### END DDR3_16M_8B_x8 ###

### DDR3_32M_8B_x8 ###
#NUM_BANK = 8
#NUM_ROW = 32768
#NUM_COL = 1024
#DEVICE_WIDTH = 8 
### END DDR3_32M_8B_x8 ###

### DDR3_64M_8B_x4 ###
#NUM_BANK = 8
#NUM_ROW = 32768
#NUM_COL = 2048
#DEVICE_WIDTH = 4 
### END DDR3_64M_8B_x4 ###

### DDR3_8M_8B_x16 ###
#NUM_BANK = 8
#NUM_ROW = 8192
#NUM_COL = 1024
#DEVICE_WIDTH = 16
### END DDR3_8M_8B_x16 ###

### DDR4_4Gb_8B_x16_JD.ini
#NUM_BANK=8
# SALP increase bank #
N_SA = 8
NUM_BANK = 8*N_SA
#NUM_ROW=32768
# SALP row #
NUM_ROW = 32768/N_SA
NUM_COL=1024
DEVICE_WIDTH=16
### END DDR4_4Gb_8b_x16_JD

### other standard params ###
TOTAL_MEGS_OF_MEM = 32768 # MB
NUM_CHAN = 4 # default 1 channl
TRANS_SIZE = 64 # bytes
BUS_WIDTH = 64 # bits
STORAGE_PER_DEVICE = (NUM_ROW * (NUM_COL * DEVICE_WIDTH) * NUM_BANK) / 8 * (2**(-20)) # MB
MEGS_OF_STORAGE_PER_RANK = (STORAGE_PER_DEVICE * (BUS_WIDTH/DEVICE_WIDTH))  
TOTAL_NUM_RANK = (int)(TOTAL_MEGS_OF_MEM / MEGS_OF_STORAGE_PER_RANK) 
RANK_PER_CHAN = TOTAL_NUM_RANK/NUM_CHAN

# get # of ranks/chan
# maximum 2 ranks/chan
#if(TOTAL_NUM_RANK/2>1.0):
#    NUM_CHAN = TOTAL_NUM_RANK/2
#else:
#    NUM_CHAN = 1
#RANK_PER_CHAN = TOTAL_NUM_RANK/NUM_CHAN   


print("storage per device: "+str(STORAGE_PER_DEVICE))
print("Megs per Rank: "+str(MEGS_OF_STORAGE_PER_RANK))
print("# of ranks: "+str(TOTAL_NUM_RANK))
print("ranks per channel: "+str(RANK_PER_CHAN))
NUM_SA = 8
### END OTHER STANDARD PARAMS ###

### file io stuff ###

# DDR4
# fo = open("mase_ddr4_8GB_1chan_4rank_4Gb_8B_x16_HiSeq_accuracy.trc", "w")
# fo = open("mase_ddr4_8GB_1chan_4rank_4Gb_8B_x16_MiSeq_accuracy.trc", "w")
# fo = open("mase_ddr4_8GB_1chan_4rank_4Gb_8B_x16_HiSeq_timing.trc", "w")
# fo = open("mase_ddr4_8GB_1chan_4rank_4Gb_8B_x16_MiSeq_timing.trc", "w")

# fo = open("mase_ddr4_8GB_2chan_2rank_4Gb_8B_x16_HiSeq_accuracy.trc", "w")
# fo = open("mase_ddr4_8GB_2chan_2rank_4Gb_8B_x16_MiSeq_accuracy.trc", "w")
# fo = open("mase_ddr4_8GB_2chan_2rank_4Gb_8B_x16_HiSeq_timing.trc", "w")
# fo = open("mase_ddr4_8GB_2chan_2rank_4Gb_8B_x16_MiSeq_timing.trc", "w")

# fo = open("mase_ddr4_4GB_1chan_2rank_4Gb_8B_x16_HiSeq_accuracy.trc", "w")
# fo = open("mase_ddr4_4GB_1chan_2rank_4Gb_8B_x16_MiSeq_accuracy.trc", "w")

# fo = open("mase_ddr4_16GB_2chan_4rank_4Gb_8B_x16_HiSeq_accuracy.trc", "w")
# fo = open("mase_ddr4_32GB_4chan_4rank_4Gb_8B_x16_HiSeq_accuracy.trc", "w")
# fo = open("mase_ddr4_16GB_2chan_4rank_4Gb_8B_x16_MiSeq_accuracy.trc", "w")
# fo = open("mase_ddr4_16GB_2chan_4rank_4Gb_8B_x16_simBA5.trc", "w")
# fo = open("mase_ddr4_32GB_4chan_4rank_4Gb_8B_x16_Miseq_accuracy.trc", "w")
# fo = open("mase_ddr4_32GB_4chan_4rank_4Gb_8B_x16_simBA5.trc", "w")

#fo = open("mase_salp_8_16GB_4Gb_8B_x16_HiSeq.trc", "w")
#fo = open("mase_salp_8_16GB_4Gb_8B_x16_MiSeq.trc", "w")
#fo = open("mase_salp_8_16GB_4Gb_8B_x16_simba5.trc", "w")

#fo = open("mase_salp_8_32GB_4Gb_8B_x16_HiSeq.trc", "w")

fo = open("mase_paired_1000.trc", "w")


# fo = open("mase_salp_8_32GB_4chan_4rank_4Gb_8B_x16_HiSeq.trc", "w")
# fo = open("mase_salp_8_32GB_4chan_4rank_4Gb_8B_x16_MiSeq_accuracy.trc", "w")
# fo = open("mase_salp_8_32GB_4chan_4rank_4Gb_8B_x16_simba5.trc", "w")

# fo = open("mase_salp_4_4GB_1chan_2rank_4Gb_8B_x16_Hiseq.trc", "w")
# fo = open("mase_salp_4_4GB_1chan_2rank_4Gb_8B_x16_Miseq.trc", "w")
# fo = open("mase_salp_4_4GB_1chan_2rank_4Gb_8B_x16_simba5.trc", "w")

# fo = open("mase_salp_4_8GB_2chan_2rank_4Gb_8B_x16_Hiseq.trc", "w")
# fo = open("mase_salp_4_8GB_2chan_2rank_4Gb_8B_x16_Miseq.trc", "w")
# fo = open("mase_salp_4_8GB_2chan_2rank_4Gb_8B_x16_simba5.trc", "w")

#fo = open("mase_salp_4_16GB_4Gb_8B_x16_Hiseq.trc", "w")
#fo = open("mase_salp_4_16GB_4Gb_8B_x16_Miseq.trc", "w")
#fo = open("mase_salp_4_16GB_4Gb_8B_x16_simba5.trc", "w")

#fo = open("mase_salp_4_32GB_4Gb_8B_x16_Hiseq.trc", "w")
#fo = open("mase_salp_4_32GB_4Gb_8B_x16_Miseq.trc", "w")
#fo = open("mase_salp_4_32GB_4Gb_8B_x16_simba5.trc", "w")


#fo = open("mase_salp_2_4GB_4Gb_8B_x16_Hiseq.trc", "w")
#fo = open("mase_salp_2_4GB_4Gb_8B_x16_Miseq.trc", "w")
#fo = open("mase_salp_2_4GB_4Gb_8B_x16_simba5.trc", "w")

#fo = open("mase_salp_2_8GB_4Gb_8B_x16_Hiseq.trc", "w")
#fo = open("mase_salp_2_8GB_4Gb_8B_x16_Miseq.trc", "w")
#fo = open("mase_salp_2_8GB_4Gb_8B_x16_Simba.trc", "w")

#fo = open("mase_salp_2_16GB_4Gb_8B_x16_Hiseq.trc", "w")
#fo = open("mase_salp_2_16GB_4Gb_8B_x16_Miseq.trc", "w")
#fo = open("mase_salp_2_16GB_4Gb_8B_x16_Simba5.trc", "w")

#fo = open("mase_salp_2_32GB_4Gb_8B_x16_Hiseq.trc", "w")
#fo = open("mase_salp_2_32GB_4Gb_8B_x16_Miseq.trc", "w")
#fo = open("mase_salp_2_32GB_4Gb_8B_x16_Simba5.trc", "w")






# simBA5
# fo = open("mase_DDR4_4GB_1chan_2rank_4Gb_8B_x16_simBa5.trc", "w")
# fo = open("mase_DDR4_8GB_2chan_2rank_4Gb_8B_x16_simBa5.trc", "w")

# DDR3
# fo = open("mase_ddr3_4GB_1chan_2rank_32M_8B_x4_HiSeq_accuracy.trc", "w")

# SALP_8 
# fo = open("mase_salp_8_4GB_1chan_2rank_HiSeq_accuracy.trc", "w")
# fo = open("mase_salp_8_8GB_1chan_4rank_HiSeq_accuracy.trc", "w")
# fo = open("mase_salp_8_8GB_2chan_2rank_HiSeq_accuracy.trc", "w")
# fo = open("mase_salp_8_4GB_1chan_2rank_HiSeq_accuracy.trc", "w")
# fo = open("mase_salp_8_4GB_1chan_2rank_MiSeq_accuracy.trc", "w")
# fo = open("mase_salp_8_4GB_1chan_2rank_simba5.trc", "w")
# fo = open("mase_salp_8_8GB_2chan_2rank_MiSeq_accuracy.trc", "w")
# fo = open("mase_salp_8_8GB_2chan_2rank_simba5.trc", "w")


# SALP_4
# fo = open("mase_salp_4_4GB_1chan_2rank_HiSeq_accuracy.trc", "w")
# fo = open("mase_salp_4_8GB_1chan_4rank_HiSeq_accuracy.trc", "w")
# fo = open("mase_salp_4_8GB_2chan_2rank_HiSeq_accuracy.trc", "w")

# SALP_2
# fo = open("mase_salp_2_4GB_1chan_2rank_HiSeq_accuracy.trc", "w")
# fo = open("mase_salp_2_8GB_1chan_4rank_HiSeq_accuracy.trc", "w")
# fo = open("mase_salp_2_8GB_2chan_2rank_HiSeq_accuracy.trc", "w")


def gen_addr(num_chan, num_bank, num_rank, num_col, num_row, num_addr, num_sa):
    
    ### build chan list
    chan_lst = []
    chan_bits_len = (int)(math.log2(num_chan))
    if chan_bits_len > 0:
        for c in range((int)(num_chan)):
            chan_fmt_str = '0' + str(chan_bits_len) + 'b'
            b = format(c, chan_fmt_str)
            chan_lst.append(b)
    print("chan_list: "+str(chan_lst))  
    print("chan_bits: "+str(chan_bits_len))
    
    ### build bank lst
    bank_lst = []
    bank_bits_len = (int)(math.log2(num_bank))
    for c in range(num_bank):
        bank_fmt_str = '0' + str(bank_bits_len) + 'b'
        b = format(c, bank_fmt_str)
        bank_lst.append(b)
    print("bank_list: "+str(bank_lst))
    print("bank_bits: "+str(bank_bits_len))
    
    ### build rank lst
    rank_lst = []
    rank_bits_len = (int)(math.log2((int)(num_rank)))
    if rank_bits_len>0: 
        for c in range((int)(num_rank)):
            rank_fmt_str = '0' + str(rank_bits_len) + 'b'
            b = format(c, rank_fmt_str)
            rank_lst.append(b)
    print("rank_list: "+str(rank_lst))
    print("rank_bits: "+str(rank_bits_len))
    
    ### build col lst
    col_lst = []
    col_offset = (int)(math.log2(TRANS_SIZE)-math.log2(BUS_WIDTH/8))
    col_bits_len = (int)(math.log2(num_col)) - col_offset
    for c in range(2**col_bits_len):
        col_fmt_str = '0' + str(col_bits_len) + 'b'
        b = format(c, col_fmt_str)
        col_lst.append(b)    
    #print(col_lst)
    print("clo_bits: "+str(col_bits_len))
    
    ### build row lst
    rows_per_sa = num_row / num_sa # num of rows per subarray
    row_lst = []
    row_addr = 0
    row_bits_len = (int)(math.log2(num_row))
    for i in range(num_sa):
        row_fmt_str = '0' + str(row_bits_len) + 'b'
        b = format((int)(row_addr), row_fmt_str)
        row_lst.append(b)
        row_addr += rows_per_sa
    #print(row_lst)    
    print("row_bits: "+str(row_bits_len))
    
    ##### generate num_addr memory request
    row_idx = 0
    rank_idx = 0
    bank_idx = 0
    chan_idx = 0
    for i in range(num_addr):
        # we use scheme 7 for maximum parallelism -> row:col:rank:bank:chan
        if len(rank_lst) > 0:
            row = str(random.choice(row_lst))
            col = str(random.choice(col_lst))
            rank = str(random.choice(rank_lst))
            bank = str(random.choice(bank_lst))
            #print("row " + row)
            #print("col " + row)
            #print("rank " + row)
            #print("bank " + row)
            addr_tmp = row+col+rank+bank
        else:
            addr_tmp = str(random.choice(row_lst) + random.choice(col_lst) + random.choice(bank_lst))
        if len(chan_lst) > 0:
            addr_tmp += str(random.choice(chan_lst))

        # append zeros
        for j in range((int)(math.log2(TRANS_SIZE))):
            addr_tmp += "0"
        #print(addr_tmp)
        # write to file
        fo.write(hex(int(addr_tmp,2))+" READ "+"0"+"\n")
        #print(str(i)+": "+hex(int(addr_tmp,2))+" READ "+"0")
        #print(len(addr_tmp))

hiseq=(10000*(92-31+1))#
miseq=(10000*(157-31+1)) #
HiSeq_Timing=(10000000*(92-31+1))
MiSeq_Timing=(10000000*(157-31+1))
stringMLST = (2*449192*(54-35+1))
phymer = 12*16529
simba5=(10000*(100-31+1))
gen_addr(NUM_CHAN, NUM_BANK, RANK_PER_CHAN, NUM_COL, NUM_ROW, 10000, 8)
