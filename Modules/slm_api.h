
#ifndef  __DEV_ADAPTER_H
#define  __DEV_ADAPTER_H

#include    "slm_type.h"
#include    "slm_regs.h"
#include    "slm_spi.h"
#include 	"slm_app.h"

//#define VERSION "V1.062"
#define ARRAY_SIZE(x)   (sizeof(x) / sizeof((x)[0]))


#define SLEEP_EN 				  (1)
#define SLEEP_DIS 				  (0)
#define TX_POWER_DEFAULT 		  (26)   //  0(-20dbm)~26(6dbm)
#define TX_MOD_IDX_DEFAULT        (0)
#define RX_SENSITIVITY_DEFAULT 	  (0)  // -87dbm   0(-85dbm)~13(-60dbm)
#define WAKUP_SENSITIVITY_DEFAULT (0)  // high
#define WAKUP_MODE_DEFAULT 		  (1)
#define WAKUP_TIME_DEFAULT 		  (0x8)
#define TX_FREG_DEFAULT 		  (5840)
#define RX_FREG_DEFAULT 		  (5800)
#define RX_BITRATE_DEFAULT         (512) //(256)
#define TX_BITRATE_DEFAULT 	 (256) //(512)
#define CRC_INV_DEFAULT 		  (1)


#define RX_OFF                    (0)
#define TX_EN                     (1)
#define RX_EN                     (2)
                                  
#define IRQ_OFF                   (0)
#define IRQ_ON                    (1)

#define TX_FIFO_LEN               (128)
#define RX_FIFO_LEN               (128)
                                  
/* 1. global variable declaration */

typedef struct
{
    UINT8_T  index;
    UINT8_T  addr;
    UINT8_T  val;
} S_GLOBAL_CFG;



typedef    void (*_user_handle)(void);
//typedef   UINT8_T (*_read_reg)(UINT8_T addr) ;
//typedef    void (*_write_reg)(UINT8_T addr, UINT8_T data);
//typedef    void (*_read_buf) (UINT8_T addr, UINT8_T *pdata, UINT32_T len);
//typedef    void (*_write_buf)(UINT8_T addr, UINT8_T *pdata, UINT32_T len);


extern volatile INT8 test_handle_index;
extern volatile INT8 test_flag ;
/* 2.  function declaration*/
extern _user_handle test_handle_cb[];
extern _user_handle wkup_handle_cb;
//extern _write_reg  write_reg;
//extern _read_reg  read_reg;
extern INT32 tx_idle;
extern INT32 rx_irq_num;
extern UINT8_T rdbuf[129];
extern UINT32_T rxlen;
void version(void);
INT32 wkupMask_set(UINT8_T mode_en , UINT8_T Tmask_s);
void auto_carrier_en(UINT8_T auto_en);
void deepsleep_en(UINT8_T sleep_en);
void sleepMode_en(UINT8_T sleep_en);
void test_mode(UINT8_T test_en);
void tx_test(UINT32_T cnt ,UINT32_T tx_interval);
void rx_test(void);
void wkupSen_test(UINT8_T index);
void rxSen_test(void);
void rx_current_test(void);
void tx0dbm_test(void);
void txMaxdbm_test(void);
void tx_carrier_test(void);
void pn9_modulated_test(UINT16_T txfreq, UINT16_T txrate);
void pn9_modulated_01_test(UINT16_T txfreq, UINT16_T txrate,UINT8_T data);
void pn9_dio_test(UINT16_T rxfreq, UINT16_T rxrate,UINT16_T rxsen);
void get_test_result(void);
void debug_en(UINT8_T debug_en);
void genarator_14k(UINT16_T pwm_cnt, UINT16_T pwm_en);
void user_cb_init(_user_handle wkup, _user_handle user);
void wkup_handle(void);
void irq_user_handle(void);
void main_proc(void);
void MFin_set(UINT8_T level);
void pn9_detect_manual(void);
void pn9_generate_test(UINT16_T txfreq, UINT16_T txrate, UINT16_T txpwr);
void pn9_printf(void);
void bbp_rst(void);


extern 	UINT32_T	 	SLDelayFlag;
extern 	void 			SLdelay_us							(void);
extern 	void 			SLdelay_1us							(UINT8_T num);
extern 	void 			SLdelay_10us_						(void);
extern 	void 			SLdelay_10us						(UINT8_T num);
extern 	void 			SLdelay_1ms							(UINT8_T num);

typedef    		UINT8_T (*_SLTXread_reg)	(UINT8_T addr) ;
typedef    		void (*_SLTXwrite_reg)	(UINT8_T addr, UINT8_T data);
typedef    		void (*_SLTXread_buf) 	(UINT8_T addr, UINT8_T *pdata, UINT32_T len);
typedef    		void (*_SLTXwrite_buf)	(UINT8_T addr, UINT8_T *pdata, UINT32_T len);
extern 				_SLTXwrite_reg  				SLTXwrite_reg;
extern 				_SLTXread_reg  					SLTXread_reg;
INT32 				SLTXrxSen_set						(UINT8_T index);
INT32 				SLTXwkupSen_set					(UINT8_T index);
INT32 				SLTXtxPower_set					(UINT8_T index);
INT32 				SLTXtxmod_idx_set				(UINT8_T index);
INT32 				SLTXwkupMode_set				(UINT8_T mode , UINT8_T Twkup_ms);
void 					SLTXtxFreq_set					(UINT16_T freq);
void 					SLTXrxFreq_set					(UINT16_T freq);
INT32 				SLTXtxBitrate_set				(UINT16_T  kbps);
INT32 				SLTXrxBitrate_set				(UINT16_T  kbps);
void 					SLTXcrcMode_set					(UINT8_T crc_inv_en);
void 					SLTXrf_init							(void);
void 					SLTXpinMux_init					(UINT8_T pinmux);
void 					SLTXclean_stat					(void);
void 					SLTXtx									(UINT8_T *pdata , UINT32_T length);
INT32 				SLTXrx									(UINT8_T *pdata , UINT32_T *plength);
extern void 	SLTXrxtx_switch					(UINT8_T sel);
extern INT32 	SLTXfill_txfifo					(UINT8_T *pdata, UINT32_T *pdatalen);
extern void 	SL1102TX_chip_init			(void);
extern void 	SL1102TX_chip_init_3v3	(void);
extern void 	SL1102TX_Send14K				(UINT8_T cnt,UINT8_T pwm_en);
extern void 	SLTXsend14k							(UINT16_T pwm_cnt);
extern UINT8_T 	SLTXRFTxFunc						(UINT8_T *rftx_buf,UINT32_T rftx_len);

typedef    		UINT8_T (*_SLRXread_reg)	(UINT8_T addr) ;
typedef    		void (*_SLRXwrite_reg)	(UINT8_T addr, UINT8_T data);
typedef    		void (*_SLRXread_buf) 	(UINT8_T addr, UINT8_T *pdata, UINT32_T len);
typedef    		void (*_SLRXwrite_buf)	(UINT8_T addr, UINT8_T *pdata, UINT32_T len);
extern 				_SLRXwrite_reg  				SLRXwrite_reg;
extern 				_SLRXread_reg  					SLRXread_reg;
INT32 				SLRXrxSen_set						(UINT8_T index);
INT32 				SLRXwkupSen_set					(UINT8_T index);
INT32 				SLRXtxPower_set					(UINT8_T index);
INT32 				SLRXtxmod_idx_set				(UINT8_T index);
INT32 				SLRXwkupMode_set				(UINT8_T mode , UINT8_T Twkup_ms);
void 					SLRXtxFreq_set					(UINT16_T freq);
void 					SLRXrxFreq_set					(UINT16_T freq);
INT32 				SLRXtxBitrate_set				(UINT16_T  kbps);
INT32 				SLRXrxBitrate_set				(UINT16_T  kbps);
void 					SLRXcrcMode_set					(UINT8_T crc_inv_en);
void 					SLRXrf_init							(void);
void 					SLRXpinMux_init					(UINT8_T pinmux);
void 					SLRXclean_stat					(void);
void 					SLRXtx									(UINT8_T *pdata , UINT32_T length);
UINT8_T 				SLRXrx									(UINT8_T *pdata , UINT8_T *plength);
extern void 	SLRXrxtx_switch					(UINT8_T sel);
extern INT32 	SLRXfill_txfifo					(UINT8_T *pdata, UINT32_T *pdatalen);
extern void 	SL1102RX_chip_init			(void);
extern void 	SL1102RX_chip_init_3v3	(void);
extern UINT8_T 	SLRXRFRxFunc						(UINT8_T *rfrx_buf,UINT8_T* rfrx_len,UINT8_T rfrx_overtime);

extern	UINT8_T SLBstFunc							(UINT8_T *bst_buf, 	UINT8_T bst_len,  UINT8_T *bst_revbuf,	UINT8_T* bst_revlen,	UINT8_T bst_counter,	UINT8_T bst_overtime);
extern 	UINT8_T SLTransFunc						(UINT8_T *trans_buf,UINT8_T trans_len,UINT8_T *trans_revbuf,UINT8_T *trans_revlen,UINT8_T trans_counter,UINT8_T *out_u8_ResCount,UINT8_T trans_overtime);

void send14k(UINT16_T pwm_cnt);
extern void _irq_en(UINT8_T irq_en);

extern void chip_init(void);
extern void Log(char* headbuf,UINT8_T headlen,UINT8_T* databuf,UINT32_T datalen);

void SLRXChannelSeclect(unsigned char _channel);

u8 BK5823_SendFunc(u8 *rftx_buf,u32 rftx_len);

#endif
//endfile
