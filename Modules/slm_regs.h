/*
*************************************************************
 Copyright (C), 2016-2020 smartlinkMicro. Co., Ltd.  All rights reserved.
 Filename:
 Complier:    ARM-MDK 5.1.2
 Target OS:   UCOS-ii
 Target HW:  athena
 Author:        htyi
 Version :     V1.0.0
 Date:          2016.4.16
 Description:
 History:
     <author>     <time>     <version >   <desc>

*************************************************************
*/
#ifndef  __SLM_REGS_H
#define  __SLM_REGS_H


#define    RX_FIFO         0x00
#define    TX_FIFO         0x01
#define    TXFIFO_STA0     0x02
#define    TXFIFO_STA1     0x03
#define    RXFIFO_STA0     0x04
#define    RXFIFO_STA1     0x05
#define    IRQ_STAT        0x06
#define    IRQ_EN          0x07
#define    TimingCTRL0     0x08
#define    TimingCTRL1     0x09
#define    TimingCTRL2     0x0a
#define    TimingCTRL3     0x0b

#define    SYS_CNTRL0      0x0C
#define    TEST0           0x0D         
#define    RXFIFO_D0       0x0E
#define    RXFIFO_D1       0x0F
#define    TXFIFO_D0       0x10
#define    TXFIFO_D1       0x11
#define    RXFIFO_BASE     0x12
#define    TXFIFO_BASE     0x13

#define    WKUP_MASK       0x14

#define    ONRST_TIMER     0x17
#define    WKUP_CTRL0      0x18
#define    WKUP_CTRL1      0x1A
#define    WKUP_CTRL2      0x1B
#define    WKUP_CTRL3      0x1C

#define    RX_DEMOD_OT0    0x20
#define    RX_DEMOD_OT1    0x21
#define    RX_DEMOD_OT2    0x22
#define    RX_DEMOD_OT3    0x23

#define    RX_RSSI         0x24

#define    RX_AGC_POWER    0x25
#define    RX_DET_POWER    0x26
#define    RX_ERR_FLAG     0x27
#define    RX_SYS_CTRL1    0x28

#define    RX_AFE_CFG0     0x29
#define    POWER_NUM       0x38
#define    TXMODM_CTRL0    0x3e
#define    TXMODM_CTRL1    0x3f
#define    TXMODM_CTRL2    0x40
#define    DATALEN_MAX0    0x42
#define    DATALEN_MAX1    0x43
#define    DIO_CTRL        0x44
#define    SW_RST          0x45

#define    RX_AFE_STATE    0x46
#define    RX_VGA_MAX      0x47

#define    CRC01           0x48
#define    CRC02           0x49
#define    CRC11           0x4A
#define    CRC12           0x4B

#define    MFIO_CFG        0x4E
#define    BER_CNTR0       0x50
#define    BER_CNTR1       0x51
#define    BER_CNTR2       0x52
#define    BER_CNTR3       0x53
#define    BER_CNTR4       0x54
#define    BER_CNTR5       0x55
#define    BER_CNTR6       0x56
#define    BER_CNTR7       0x57
#define    CHIID           0x60


#define    RX_CFG0         0x66
#define    RX_CFG1         0x67
#define    RX_CFG2         0x68
#define    TX_CFG0         0x69
#define    TX_CFG1         0x6B

#define    RX_CFG3         0x6E
#define    WKSNS_CFG1      0x6F



#define    PLL_CTRL0       0x72   // tx div_int
#define    PLL_CTRL1       0x73
#define    PLL_CTRL2       0x74    //tx div_frac 
#define    PLL_CTRL3       0x75   // rx div_int
#define    PLL_CTRL4       0x76
#define    PLL_CTRL5       0x77  // rx div_frac
#define    ATEST_CFG       0x78

#define    VCO_CFG0        0x79
#define    VCO_CFG1        0x7A
#define    VCO_CFG2        0x7B
#define    AN_TEST_CFG     0x7C

#endif
//endfile
