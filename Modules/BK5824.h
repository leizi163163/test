#ifndef	_BK5824_H
#define	_BK5824_H
extern void BK5824_CHANGECHANNEL(unsigned char  in_u8_Channel);
extern void BK5824_ConfigPower(unsigned char in_u8_Level);
extern void BK5824_zb_en(unsigned char b_enable);
extern void BK5824_Rx_En(unsigned char b_enable);
extern void BK5824_Baudrate(void);
extern void BK5824PowerUp(void);
extern void BK5824Init(void);
extern void BK5824_14K_Enable(unsigned char enable);
extern void ClearBk5824Irq(void);
extern void BK5824ModeSet(unsigned char _BK5824Mode);
void BK5824BstMode(void);

extern void BK5823TX_SPI_Init(void);

#endif
