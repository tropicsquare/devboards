// software driver for TPDI interface

// https://tropic-gitlab.corp.sldev.cz/internal/hw-design/ts-tpdi/ --> artifacts --> tpdi_des_spec.pdf

// The only supported serial clock configuration is: CPOL=0, CPHA=1.
// CPOL=0 Clock polarity in non-active state is logic low.
// CPHA=1 Data are shifted out on rising edge, and sampled on falling edge.

#include "tpdi.h"
#include "hardware.h"
#include "gpio.h"

#define _CMD_WRITE 0x00
#define _CMD_READ  0x01

// transfer: CMD(1),ADDR(4),DUMMY(4),DATA(4*x)
// only 32 bit words transfer supported

#define _TPDI_DELAY_MAX (10000)
static u32 _tpdi_delay = 1;

static void _dly(void)
{   // note: TPDI CLK should be 3x slower than system clock
    u32 dly = _tpdi_delay;
    while (dly--)
    {
	    asm("nop");
	    asm("nop");
	    asm("nop");
	    asm("nop");
    }
}

#if (HW_TPDI_DIO0_BIT == 0) && (HW_TPDI_DIO1_BIT == 1) && (HW_TPDI_DIO2_BIT == 2) && (HW_TPDI_DIO3_BIT == 3) 
  // we have HW DIO connected to bits 0..3 on port GPIOB
  // so optimalized direct access is possible
  #define DIO_OPTIMALIZATION_ENABLE
#endif


#if defined DIO_OPTIMALIZATION_ENABLE

inline static void _wr_dio(u8 data)
{
    HW_TPDI_DIO0_PORT->ODR = ((HW_TPDI_DIO0_PORT->ODR & (~0xF)) | data);
}

inline static u8 _rd_dio(void)
{
    return (HW_TPDI_DIO0_PORT->IDR & 0x0F);
}

static void _hw_dio_set_out(void)
{
    HW_TPDI_DIO_DIR_OUT;
    HW_TPDI_DIO0_PORT->CRL = ((HW_TPDI_DIO0_PORT->CRL & (~0xFFFFUL)) | 0x3333);
}

static void _hw_dio_set_in(void)
{
    HW_TPDI_DIO0_PORT->CRL = ((HW_TPDI_DIO0_PORT->CRL & (~0xFFFFUL)) | 0x8888);
    HW_TPDI_DIO_DIR_INP;
}

#else // defined DIO_OPTIMALIZATION_ENABLE

#warning "optimalization not possible"

static void _wr_dio(u8 data)
{
    if (data & 1)
        GPIO_BIT_SET(HW_TPDI_DIO0_PORT, HW_TPDI_DIO0_BIT);
    else
        GPIO_BIT_CLR(HW_TPDI_DIO0_PORT, HW_TPDI_DIO0_BIT);

    if (data & 2)
        GPIO_BIT_SET(HW_TPDI_DIO1_PORT, HW_TPDI_DIO1_BIT);
    else
        GPIO_BIT_CLR(HW_TPDI_DIO1_PORT, HW_TPDI_DIO1_BIT);

    if (data & 4)
        GPIO_BIT_SET(HW_TPDI_DIO2_PORT, HW_TPDI_DIO2_BIT);
    else
        GPIO_BIT_CLR(HW_TPDI_DIO2_PORT, HW_TPDI_DIO2_BIT);

    if (data & 8)
        GPIO_BIT_SET(HW_TPDI_DIO3_PORT, HW_TPDI_DIO3_BIT);
    else
        GPIO_BIT_CLR(HW_TPDI_DIO3_PORT, HW_TPDI_DIO3_BIT);
}

static u8 _rd_dio(void)
{
    u8 data = 0;

    if (GPIO_IN(HW_TPDI_DIO0_PORT, HW_TPDI_DIO0_BIT))
        data |= 1;

    if (GPIO_IN(HW_TPDI_DIO1_PORT, HW_TPDI_DIO1_BIT))
        data |= 2;

    if (GPIO_IN(HW_TPDI_DIO2_PORT, HW_TPDI_DIO2_BIT))
        data |= 4;

    if (GPIO_IN(HW_TPDI_DIO3_PORT, HW_TPDI_DIO3_BIT))
        data |= 8;

    return (data);
}

static void _hw_dio_set_out(void)
{
    HW_TPDI_DIO_DIR_OUT;
    GPIO_MODE(HW_TPDI_DIO0_PORT, HW_TPDI_DIO0_BIT, GPIO_MODE_OUTPUT);
    GPIO_MODE(HW_TPDI_DIO1_PORT, HW_TPDI_DIO1_BIT, GPIO_MODE_OUTPUT);
    GPIO_MODE(HW_TPDI_DIO2_PORT, HW_TPDI_DIO2_BIT, GPIO_MODE_OUTPUT);
    GPIO_MODE(HW_TPDI_DIO3_PORT, HW_TPDI_DIO3_BIT, GPIO_MODE_OUTPUT);
}

static void _hw_dio_set_in(void)
{
    GPIO_MODE(HW_TPDI_DIO0_PORT, HW_TPDI_DIO0_BIT, GPIO_MODE_INPUT);
    GPIO_MODE(HW_TPDI_DIO1_PORT, HW_TPDI_DIO1_BIT, GPIO_MODE_INPUT);
    GPIO_MODE(HW_TPDI_DIO2_PORT, HW_TPDI_DIO2_BIT, GPIO_MODE_INPUT);
    GPIO_MODE(HW_TPDI_DIO3_PORT, HW_TPDI_DIO3_BIT, GPIO_MODE_INPUT);

    HW_TPDI_DIO_DIR_INP;
}
#endif // not defined DIO_OPTIMALIZATION_ENABLE

void tpdi_hw_init(void)
{

    HW_TPDI_OE_DISABLE;
    HW_TPDI_DIO_DIR_INP;
    HW_TPDI_DIO_OE_DISABLE;

    HW_TPDI_OE_INIT;
    HW_TPDI_DIO_OE_INIT;
    HW_TPDI_DIO_DIR_INIT;

    HW_TPDI_CSN_UP;
    HW_TPDI_CLK_DOWN;
    HW_TPDI_CSN_INIT;
    HW_TPDI_CLK_INIT;
    HW_TPDI_RDY_INIT;

    HW_TPDI_DIO0_INIT;
    HW_TPDI_DIO1_INIT;
    HW_TPDI_DIO2_INIT;
    HW_TPDI_DIO3_INIT;

}

void tpdi_hw_connect(void)
{
    HW_TPDI_OE_ENABLE;
    HW_TPDI_DIO_OE_ENABLE;
}

void tpdi_hw_disconnect(void)
{
    HW_TPDI_OE_DISABLE;
    HW_TPDI_DIO_OE_DISABLE;
}

bool tpdi_hw_ready(void)
{
    return (HW_TPDI_RDY_IN ? true : false);
}

inline static u8 _nibble_read(void)
{   // 4 bits read
    u8 result;
    HW_TPDI_CLK_UP;
    _dly();
    HW_TPDI_CLK_DOWN;
    _dly();
    result = _rd_dio();
    return (result);
}

inline static u8 _byte_read(void)
{   // 8 bits read
    u8 result;
    result = _nibble_read() << 4;
    result |= _nibble_read();
    return (result);
}

static u32 _word_read(void)
{   // 32 bits read
    u32 result;
    result = _byte_read();
    result <<= 8;
    result |= _byte_read();
    result <<= 8;
    result |= _byte_read();
    result <<= 8;
    result |= _byte_read();
    return (result);
}

inline static void _nibble_send(u8 data)
{   // 4 bits write
    _wr_dio(data);
    HW_TPDI_CLK_UP;
    _dly();
    HW_TPDI_CLK_DOWN;
    _dly();
}

inline static void _byte_send(u8 data)
{   // 8 bits write
    _nibble_send(data >> 4);
    _nibble_send(data & 0xF);
}

static void _word_send(u32 data)
{   // 32 bits write
    _byte_send(data>>24);
    _byte_send(data>>16);
    _byte_send(data>>8);
    _byte_send(data);
}

static __inline bool _rdy_in(void)
{
    int limit = 100;
    
    while (! HW_TPDI_RDY_IN)
    {
        if (--limit == 0)
            return (false);
        _dly();
    }
    return (true);
}

bool tpdi_data_send(u32 addr, u32 *src, size_t len)
{
    _hw_dio_set_out();
    HW_TPDI_CLK_DOWN;
    HW_TPDI_CSN_DOWN;
    _dly();
    _byte_send(_CMD_WRITE);
    _word_send(addr);
    _word_read(); // dummy word

    while (len--)
    {
        if (! _rdy_in())
        {
            HW_TPDI_CSN_UP;
            return (false);
        }

        _word_send(*src);
        src++;
    }
    _hw_dio_set_in();
    HW_TPDI_CSN_UP;
    return (true);
}

bool tpdi_data_read(u32 *dest, u32 addr, size_t len)
{
    _hw_dio_set_out();
    HW_TPDI_CLK_DOWN;
    HW_TPDI_CSN_DOWN;
    _dly();
    _byte_send(_CMD_READ);
    _word_send(addr);
    _hw_dio_set_in();
    _word_read(); // dummy word
    while (len--)
    {
        if (! _rdy_in())
        {
            HW_TPDI_CSN_UP;
            return (false);
        }

        *dest = _word_read();
        dest++;
    }
    HW_TPDI_CSN_UP;
    return (true);
}

bool tpdi_delay_set(u32 value)
{

    if (value > _TPDI_DELAY_MAX)
        return (false);

    _tpdi_delay = value;
    return (true);
}

u32  tpdi_delay_get(void)
{
    return (_tpdi_delay);
}

