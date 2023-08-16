
#ifndef UI_DRAW_H
#define UI_DRAW_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define UI_COUNTOF(a) (sizeof(a) / sizeof(a[0]))
#define UI_ABS(a)     ((a) > 0 ? (a) : -(a))
#define UI_MIN(a,b)   (((a) < (b)) ? (a) : (b))
#define UI_MAX(a,b)   (((a) > (b)) ? (a) : (b))

typedef struct {
	int16_t x_size;
	int16_t y_size;
	const uint8_t *p_data;
	unsigned char x_align_flag; //0: no align, 1: x must byte(8 bits) align.
}ui_bitmap_t;

typedef struct {
	uint8_t                     pic_num;
	ui_bitmap_t const * const * p_pic;
}animat_pic_t;

typedef enum {
	UI_COLOR_BLACK = 0,
	UI_COLOR_WHITE = 0xFF,
}ui_color_t;


//=================================================================
typedef struct
{
  unsigned char HzCodeIndex[2];
  unsigned char HzDot[32];
}typFONT_GB12;

typedef struct 
{
	uint32_t Width;
	uint32_t Height;
	const uint8_t *pBits;                 // ŒªÕºµÿ÷∑
}sBMP;

typedef enum {
	PEN_BLACK,
	PEN_WHITE,
}PEN_TYPE;

//===================================================================

void GDI_CopyBMP(const sBMP *pBMP,long X,long Y);
void GDI_TextCN(const unsigned char *pText,long X,long Y,PEN_TYPE PenType);



//void ui_draw_point(int x, int y, ui_color_t color);
void ui_draw_horizontal_line(int x, int y, int len);
void ui_draw_vertical_line(int x, int y, int len);
void ui_draw_line(int x0, int y0, int x1, int y1);
void ui_draw_rectangle(int x0, int y0, int x1, int y1);
void ui_draw_fill_rectangle(int x0, int y0, int x1, int y1);
void ui_draw_clear_rectangle(int x0, int y0, int x1, int y1);
void ui_draw_clear_screen(void);
void ui_draw_picture(const ui_bitmap_t *p_bitmap, int x, int y);
void ui_draw_circle(int x, int y, int r);
void ui_draw_fill_circle(int x0, int y0, int r);
void ui_draw_buffer(const uint8_t *ptr, int is_font, int x, int y, int buffer_x_size, int buffer_y_size, unsigned char x_align_flag);
void ui_draw_progress_bar(unsigned int max_value, unsigned int current_value);
void ui_draw_battery(int x, int y, unsigned char battery_capacity);


#ifdef __cplusplus
}
#endif

#endif /* UI_DRAW_H */

