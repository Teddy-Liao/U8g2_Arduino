/*

  u8x8_d_il3820_296x128.c

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2017, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
  il3820: 200x300x1
  
  command 
    0x22: assign actions
    0x20: execute actions
  
  action for command 0x022 are (more or less guessed)
    bit 7:	Enable Clock
    bit 6:	Enable Charge Pump
    bit 5:	Load Temparture Value (???)
    bit 4:	Load LUT (???)
    bit 3:	Initial Display (???)
    bit 2:	Pattern Display --> Requires about 945ms with the LUT from below
    bit 1:	Disable Charge Pump
    bit 0:	Disable Clock
    
    Disable Charge Pump and Clock require about 267ms
    Enable Charge Pump and Clock require about 10ms

  Notes:
    - Introduced a refresh display message, which copies RAM to display
    - Charge pump and clock are only enabled for the transfer RAM to display
    - U8x8 will not really work because of the two buffers in the SSD1606, however U8g2 should be ok.

*/


#include "u8x8.h"


#define L(a,b,c,d) (((a)<<6)|((b)<<4)|((c)<<2)|(d))


/* http://www.waveshare.com/wiki/File:2.9inch_e-Paper_Module_code.7z */
static const uint8_t u8x8_d_il3820_296x128_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  
  
  

  U8X8_CA(0x10, 0x00),	/* Deep Sleep mode Control: Disable */
  U8X8_C(0x01),
    U8X8_A(295 % 256), U8X8_A(295/256), U8X8_A(0),
  
  
  U8X8_CA(0x03, 0x00), 	/* Gate Driving voltage: 15V (lowest value)*/
  U8X8_CA(0x04, 0x0a), 	/* Source Driving voltage: 15V (mid value and POR)*/
  
  U8X8_CA(0x11, 0x01),	/* Define data entry mode, x&y inc, x first*/
  
  U8X8_CAA(0x44, 0, 29),	/* RAM x start & end, 32*4=128 */
  U8X8_CAAAA(0x45, 0, 0, 295&255, 295>>8),	/* RAM y start & end, 0..295 */
  
  U8X8_CA(0x4e, 0),	/* set x pos, 0..29? */
  U8X8_CAA(0x4f, 100&255, 100>>8),	/* set y pos, 0...320??? */

  U8X8_CA(0x22, 0xc0),	/* display update seq. option: enable clk, enable CP, .... todo: this is never activated */
  
  U8X8_C(0x32),	/* write LUT register*/

  
  /* according to the command table, the lut has 240 bits (=30 bytes * 8 bits) */
  
  /* Waveform part of the LUT (20 bytes) */
  /* bit 7/6: 1 - 1 transition */
  /* bit 5/4: 1 - 0 transition */
  /* bit 3/2: 0 - 1 transition */
  /* bit 1/0: 0 - 0 transition */
  /* 	00 – VSS */
  /* 	01 – VSH */
  /* 	10 – VSL */
  /* 	11 – NA */
  
/*
LUT for the 296x128 device (IL3820)
LUT (cmd: 0x032 has 30 bytes)
section 6.8 of the datasheet mentions 256 bits = 32 bytes for the LUT
chapter 7 tells 30 bytes

according to section 6.8:
20 bytes waveform
10 bytes timing
1 byte named as VSH/VSL
1 empty byte

*/
/*
  static unsigned char LUTDefault_full[] = 
  0x32,
  0x02,0x02,0x01,0x11,0x12,  0x12,0x22,0x22,0x66,0x69,
  0x69,0x59,0x58,0x99,0x99,  0x88,0x00,0x00,0x00,0x00,
  0xF8,0xB4,0x13,0x51,0x35,  0x51,0x51,0x19,0x01,0x00, 0x00};
  static unsigned char LUTDefault_part[] = {
  0x32,
  0x10,0x18,0x18,0x08,0x18,  0x18,0x08,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,
  0x00,0x13,0x14,0x44,0x12,  0x00,0x00,0x00,0x00,0x00,0x00};
*/
  
  /* original values */
  U8X8_A(0x02),
  U8X8_A(0x02),
  U8X8_A(0x01),
  U8X8_A(0x11),  
  U8X8_A(0x12),
  
  U8X8_A(0x12),
  U8X8_A(0x22),
  U8X8_A(0x22),
  U8X8_A(0x66),  
  U8X8_A(0x69),
  
  U8X8_A(0x69),
  U8X8_A(0x59),
  U8X8_A(0x58),
  U8X8_A(0x99),
  U8X8_A(0x99),
  
  U8X8_A(0x88),
  U8X8_A(0x00),
  U8X8_A(0x00),
  U8X8_A(0x00),
  U8X8_A(0x00),
  
  /* Timing part of the LUT, 20 Phases with 4 bit each: 10 bytes */
  U8X8_A(0xF8),
  U8X8_A(0xB4),
  U8X8_A(0x13),
  U8X8_A(0x51),
  U8X8_A(0x35),
  U8X8_A(0x51),
  U8X8_A(0x51),
  U8X8_A(0x19),
  U8X8_A(0x01),
  U8X8_A(0x00),
  
  //U8X8_A(0x00),
  
  

  U8X8_CA(0x2c, 0xa8),	/* write vcom value*/
  U8X8_CA(0x3a, 0x1a),	/* dummy lines */
  U8X8_CA(0x3b, 0x08),	/* gate time */
  U8X8_CA(0x3c, 0x33),	/* select boarder waveform */
  U8X8_CA(0x22, 0xc4),	/* display update seq. option: clk -> CP -> LUT -> initial display -> pattern display */


  U8X8_CA(0x11, 0x07),	/* Define data entry mode, x&y inc, x first*/

  U8X8_CAA(0x44, 0, 29),	/* RAM x start & end, 32*4=128 */
  U8X8_CAAAA(0x45, 0, 0, 295&255, 295>>8),	/* RAM y start & end, 0..295 */
  
  U8X8_CA(0x4e, 0),	/* set x pos, 0..29? */
  U8X8_CAA(0x4f, 0, 0),	/* set y pos, 0...320??? */

  U8X8_C(0x024),		/* data write */

  U8X8_D1(0x000),
  U8X8_D1(0x000),
  U8X8_D1(0x081),
  U8X8_D1(0x081),

  U8X8_D1(0x0c3),
  U8X8_D1(0x0c3),
  U8X8_D1(0x0e7),
  U8X8_D1(0x0e7),



  U8X8_D1(0x00f),
  U8X8_D1(0x00f),
  U8X8_D1(0x00f),
  U8X8_D1(0x00f),

  U8X8_D1(0x00f),
  U8X8_D1(0x00f),
  U8X8_D1(0x00f),
  U8X8_D1(0x00f),

  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_il3820_to_display_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  
  U8X8_CA(0x22, 0xc4),	/* display update seq. option: clk -> CP -> LUT -> initial display -> pattern display */
  U8X8_C(0x20),	/* execute sequence */
  
  U8X8_DLY(250),	/* delay for 2000ms. This has to be tested, maybe it can be reduced */
  U8X8_DLY(250),
  U8X8_DLY(250),
  U8X8_DLY(250),
  
  U8X8_DLY(250),
  U8X8_DLY(250),
  U8X8_DLY(250),
  U8X8_DLY(250),
  
  U8X8_CA(0x22, 0x03),	/* disable clock and charge pump */
  U8X8_DLY(250),
  U8X8_DLY(250),
  U8X8_DLY(100),  
  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_il3820_296x128_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_il3820_296x128_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_il3820_296x128_flip0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_il3820_296x128_flip1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};


static uint8_t *u8x8_convert_tile_for_il3820(uint8_t *t)
{
  uint8_t i;
  static uint8_t buf[8];
  uint8_t *pbuf = buf;

  for( i = 0; i < 8; i++ )
  {
    *pbuf++ = ~(*t++);
  }
  return buf;
}

static void u8x8_d_il3820_draw_tile(u8x8_t *u8x8, uint8_t arg_int, void *arg_ptr) U8X8_NOINLINE;
static void u8x8_d_il3820_draw_tile(u8x8_t *u8x8, uint8_t arg_int, void *arg_ptr)
{
  uint16_t x;
  uint8_t c, page;
  uint8_t *ptr;
  u8x8_cad_StartTransfer(u8x8);

  page = u8x8->display_info->tile_height;
  page --;
  page -= (((u8x8_tile_t *)arg_ptr)->y_pos);
  
  //page = (((u8x8_tile_t *)arg_ptr)->y_pos);

  x = ((u8x8_tile_t *)arg_ptr)->x_pos;
  x *= 8;
  x += u8x8->x_offset;

  //u8x8_cad_SendCmd(u8x8, 0x00f );	/* scan start */
  //u8x8_cad_SendArg(u8x8, 0);

  //u8x8_cad_SendCmd(u8x8, 0x011 );	/* cursor increment mode */
  //u8x8_cad_SendArg(u8x8, 3);

  //u8x8_cad_SendCmd(u8x8, 0x045 );	/* window start column */
  //u8x8_cad_SendArg(u8x8, 0);
  //u8x8_cad_SendArg(u8x8, 0);
  //u8x8_cad_SendArg(u8x8, (296-1)&255);		/* end of display */
  //u8x8_cad_SendArg(u8x8, (296-1)>>8);

  //u8x8_cad_SendCmd(u8x8, 0x044 );	/* window end page */
  //u8x8_cad_SendArg(u8x8, page);
  //u8x8_cad_SendArg(u8x8, page+1);

  u8x8_cad_SendCmd(u8x8, 0x04f );	/* set cursor column */
  u8x8_cad_SendArg(u8x8, x&255);
  u8x8_cad_SendArg(u8x8, x>>8);

  u8x8_cad_SendCmd(u8x8, 0x04e );	/* set cursor row */
  u8x8_cad_SendArg(u8x8, page);

  u8x8_cad_SendCmd(u8x8, 0x024 );
  
  do
  {
    c = ((u8x8_tile_t *)arg_ptr)->cnt;
    ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
    do
    {
      u8x8_cad_SendData(u8x8, 8, u8x8_convert_tile_for_il3820(ptr));
      ptr += 8;
      x += 8;
      c--;
    } while( c > 0 );
    
    arg_int--;
  } while( arg_int > 0 );
  
  u8x8_cad_EndTransfer(u8x8);
}


static uint8_t u8x8_d_il3820_296x128_generic(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
    /* handled by the calling function
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_il3820_296x128_display_info);
      break;
    */
    case U8X8_MSG_DISPLAY_INIT:

      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_il3820_296x128_init_seq);    

      //u8x8_cad_SendSequence(u8x8, u8x8_d_il3820_to_display_seq);    
      //for(;;)
	//;
    
#ifdef NOT_USED
      /* STRATEGY FOR SSD1606 */
      /* special code for the SSD1606... */
      /* ensure that the initial buffer is clear and all eInk is set to white */
      /* this is done here, because the LUT will be of that kind, that it uses the previous color */
      /* make everything black */
      u8x8_FillDisplay(u8x8);		
      /* write content to the display */
      u8x8_RefreshDisplay(u8x8);
      /* now make everything clear */
      u8x8_FillDisplay(u8x8);		
      /* write content to the display */
      u8x8_RefreshDisplay(u8x8);
      /* now make everything clear */
      u8x8_ClearDisplay(u8x8);		
      /* write content to the display */
      u8x8_RefreshDisplay(u8x8);
#endif

      u8x8_ClearDisplay(u8x8);		
      /* write content to the display */
      u8x8_RefreshDisplay(u8x8);
    
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
/*
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_il3820_296x128_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_il3820_296x128_powersave1_seq);
*/
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
/*
      if ( arg_int == 0 )
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_il3820_296x128_flip0_seq);
	u8x8->x_offset = u8x8->display_info->default_x_offset;
      }
      else
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_il3820_296x128_flip1_seq);
	u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }
*/
      break;
#ifdef U8X8_WITH_SET_CONTRAST
/*
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_EndTransfer(u8x8);
*/
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_d_il3820_draw_tile(u8x8, arg_int, arg_ptr);
      break;
    case U8X8_MSG_DISPLAY_REFRESH:
      u8x8_cad_SendSequence(u8x8, u8x8_d_il3820_to_display_seq);    
      break;
    default:
      return 0;
  }
  return 1;
}


static const u8x8_display_info_t u8x8_il3820_296x128_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 120,
  /* pre_chip_disable_wait_ns = */ 60,
  /* reset_pulse_width_ms = */ 100, 	
  /* post_reset_wait_ms = */ 100, 
  /* sda_setup_time_ns = */ 50,		/* IL3820 */
  /* sck_pulse_width_ns = */ 125,	/* IL3820: 125ns, clock cycle = 250ns */
  /* sck_clock_hz = */ 4000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 2,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 40,
  /* write_pulse_width_ns = */ 150,	
  /* tile_width = */ 37,		/* 37*8 = 296 */
  /* tile_hight = */ 16,		/* 16*8 = 128 */	
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 296,
  /* pixel_height = */ 128
};

uint8_t u8x8_d_il3820_296x128(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    if ( msg == U8X8_MSG_DISPLAY_SETUP_MEMORY )
    {
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_il3820_296x128_display_info);
      return 1;
    }
    return u8x8_d_il3820_296x128_generic(u8x8, msg, arg_int, arg_ptr);
}


