#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

#include "../util.h"

typedef uint32_t sft_color;

typedef struct
{
    sft_color* pixels;
    uint32_t width;
    uint32_t height;
} sft_image;


static const uint64_t _sft_font[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000, // Space
    0b00001000'00001000'00001000'00001000'00000000'00001000'00000000'00000000, // !
    0b00101000'00101000'00000000'00000000'00000000'00000000'00000000'00000000, // "
    0b00000000'00101000'01111100'00101000'01111100'00101000'00000000'00000000, // #
    0b00001000'00011110'00101000'00011100'00001010'00111100'00001000'00000000, // $
    0b01100000'10010100'01101000'00010110'00101001'00000110'00000000'00000000, // %
    0b00011100'00100000'00100000'00011001'00100110'00011001'00000000'00000000, // &
    0b00001000'00001000'00000000'00000000'00000000'00000000'00000000'00000000, // '
    0b00001000'00010000'00100000'00100000'00010000'00001000'00000000'00000000, // (
    0b00010000'00001000'00000100'00000100'00001000'00010000'00000000'00000000, // )
    0b00101010'00011100'00111110'00011100'00101010'00000000'00000000'00000000, // *
    0b00000000'00001000'00001000'00111110'00001000'00001000'00000000'00000000, // +
    0b00000000'00000000'00000000'00000000'00000000'00001000'00010000'00000000, // ,
    0b00000000'00000000'00000000'00111100'00000000'00000000'00000000'00000000, // -
    0b00000000'00000000'00000000'00000000'00000000'00001000'00000000'00000000, // .
    0b00000010'00000100'00001000'00010000'00100000'01000000'00000000'00000000, // /

    0b00011000'00100100'01000010'01000010'00100100'00011000'00000000'00000000, // 0
    0b00001000'00011000'00001000'00001000'00001000'00011100'00000000'00000000, // 1
    0b00111100'01000010'00000100'00011000'00100000'01111110'00000000'00000000, // 2
    0b00111100'01000010'00000100'00011000'01000010'00111100'00000000'00000000, // 3
    0b00001000'00011000'00101000'01001000'01111100'00001000'00000000'00000000, // 4
    0b01111110'01000000'01111100'00000010'01000010'00111100'00000000'00000000, // 5
    0b00111100'01000000'01111100'01000010'01000010'00111100'00000000'00000000, // 6
    0b01111110'00000100'00001000'00010000'00100000'01000000'00000000'00000000, // 7
    0b00111100'01000010'00111100'01000010'01000010'00111100'00000000'00000000, // 8
    0b00111100'01000010'01000010'00111110'00000010'00111100'00000000'00000000, // 9

    0b00000000'00000000'00001000'00000000'00000000'00001000'00000000'00000000, // :
    0b00000000'00000000'00001000'00000000'00000000'00001000'00010000'00000000, // ;
    0b00000000'00000110'00011000'01100000'00011000'00000110'00000000'00000000, // <
    0b00000000'00000000'01111110'00000000'01111110'00000000'00000000'00000000, // =
    0b00000000'01100000'00011000'00000110'00011000'01100000'00000000'00000000, // >
    0b00111000'01000100'00000100'00011000'00000000'00010000'00000000'00000000, // ?
    0b00000000'00111100'01000100'10011100'10010100'01011100'00100000'00011100, // @

    0b00011000'00011000'00100100'00111100'01000010'01000010'00000000'00000000, // A
    0b01111000'01000100'01111000'01000100'01000100'01111000'00000000'00000000, // B
    0b00111000'01000100'10000000'10000000'01000100'00111000'00000000'00000000, // C
    0b01111000'01000100'01000100'01000100'01000100'01111000'00000000'00000000, // D
    0b01111100'01000000'01111000'01000000'01000000'01111100'00000000'00000000, // E
    0b01111100'01000000'01111000'01000000'01000000'01000000'00000000'00000000, // F
    0b00111000'01000100'10000000'10011100'01000100'00111000'00000000'00000000, // G
    0b01000010'01000010'01111110'01000010'01000010'01000010'00000000'00000000, // H
    0b00111110'00001000'00001000'00001000'00001000'00111110'00000000'00000000, // I
    0b00011100'00000100'00000100'00000100'01000100'00111000'00000000'00000000, // J
    0b01000100'01001000'01010000'01110000'01001000'01000100'00000000'00000000, // K
    0b01000000'01000000'01000000'01000000'01000000'01111110'00000000'00000000, // L
    0b01000001'01100011'01010101'01001001'01000001'01000001'00000000'00000000, // M
    0b01000010'01100010'01010010'01001010'01000110'01000010'00000000'00000000, // N
    0b00011100'00100010'00100010'00100010'00100010'00011100'00000000'00000000, // O
    0b01111000'01000100'01111000'01000000'01000000'01000000'00000000'00000000, // P
    0b00011100'00100010'00100010'00100010'00100010'00011100'00000010'00000000, // Q
    0b01111000'01000100'01111000'01010000'01001000'01000100'00000000'00000000, // R
    0b00011100'00100010'00010000'00001100'00100010'00011100'00000000'00000000, // S
    0b01111111'00001000'00001000'00001000'00001000'00001000'00000000'00000000, // T
    0b01000010'01000010'01000010'01000010'01000010'00111100'00000000'00000000, // U
    0b10000001'01000010'01000010'00100100'00100100'00011000'00000000'00000000, // V
    0b01000001'01000001'01001001'01010101'01100011'01000001'00000000'00000000, // W
    0b01000010'00100100'00011000'00011000'00100100'01000010'00000000'00000000, // X
    0b01000001'00100010'00010100'00001000'00001000'00001000'00000000'00000000, // Y
    0b01111110'00000100'00001000'00010000'00100000'01111110'00000000'00000000, // Z

    0b00111000'00100000'00100000'00100000'00100000'00111000'00000000'00000000, // [
    0b01000000'00100000'00010000'00001000'00000100'00000010'00000000'00000000, // \ 
    0b00111000'00001000'00001000'00001000'00001000'00111000'00000000'00000000, // ]
    0b00010000'00101000'00000000'00000000'00000000'00000000'00000000'00000000, // ^
    0b00000000'00000000'00000000'00000000'00000000'01111110'00000000'00000000, // _
    0b00010000'00001000'00000000'00000000'00000000'00000000'00000000'00000000, // `

    0b00000000'00111100'00000010'00111110'01000110'00111010'00000000'00000000, // a
    0b01000000'01000000'01111100'01000010'01100010'01011100'00000000'00000000, // b
    0b00000000'00000000'00011100'00100000'00100000'00011100'00000000'00000000, // c
    0b00000010'00000010'00111110'01000010'01000110'00111010'00000000'00000000, // d
    0b00000000'00111100'01000010'01111110'01000000'00111100'00000000'00000000, // e
    0b00000000'00011000'00010000'00111000'00010000'00010000'00000000'00000000, // f
    0b00000000'00000000'00110100'01001100'01000100'00110100'00000100'00111000, // g
    0b00100000'00100000'00111000'00100100'00100100'00100100'00000000'00000000, // h
    0b00001000'00000000'00001000'00001000'00001000'00001000'00000000'00000000, // i
    0b00001000'00000000'00011000'00001000'00001000'00001000'00001000'01110000, // j
    0b00100000'00100000'00100100'00101000'00110000'00101100'00000000'00000000, // k
    0b00010000'00010000'00010000'00010000'00010000'00011000'00000000'00000000, // l
    0b00000000'00000000'01100110'01011010'01000010'01000010'00000000'00000000, // m
    0b00000000'00000000'00101110'00110010'00100010'00100010'00000000'00000000, // n
    0b00000000'00000000'00111100'01000010'01000010'00111100'00000000'00000000, // o
    0b00000000'00000000'01011100'01100010'01000010'01111100'01000000'01000000, // p
    0b00000000'00000000'00111010'01000110'01000010'00111110'00000010'00000010, // q
    0b00000000'00000000'00101100'00110010'00100000'00100000'00000000'00000000, // r
    0b00000000'00011100'00100000'00011000'00000100'00111000'00000000'00000000, // s
    0b00000000'00010000'00111100'00010000'00010000'00011000'00000000'00000000, // t
    0b00000000'00000000'00100010'00100010'00100110'00011010'00000000'00000000, // u
    0b00000000'00000000'01000010'01000010'00100100'00011000'00000000'00000000, // v
    0b00000000'00000000'10000001'10000001'01011010'01100110'00000000'00000000, // w
    0b00000000'00000000'01000010'00100100'00011000'01100110'00000000'00000000, // x
    0b00000000'00000000'01000010'00100010'00010100'00001000'00010000'01100000, // y
    0b00000000'00000000'00111100'00001000'00010000'00111100'00000000'00000000, // z

    0b00011100'00010000'00110000'00110000'00010000'00011100'00000000'00000000, // {
    0b00001000'00001000'00001000'00001000'00001000'00001000'00001000'00000000, // |
    0b00111000'00001000'00001100'00001100'00001000'00111000'00000000'00000000, // }
    0b00000000'00000000'00000000'00110010'01001100'00000000'00000000'00000000, // ~

    0
};


/**
* \brief Returns an image without filling with color
* \param width The width of the image
* \param height The height of the image
* \warning Must be freed by user. 
* \warning NULL returned if malloc failed
* \warning pixels array NULL if width * height is zero
*/
sft_image* sft_image_create(uint32_t width, uint32_t height);

/**
* \brief Resizes image to new size
* \param image Target image to be resized
* \param width The new width of the image
* \param height The new height of the image
* \warning pixels array NULL if malloc failed
* \warning pixels array NULL if width * height is zero
* \warning Must be freed by user.
*/
void sft_image_resize(sft_image* image, uint32_t width, uint32_t height);


/**
* \brief Fills the entire image with a specified color
* \param image Target image to be filled
* \param color Color to fill image with
*/
void sft_image_fill(sft_image* image, sft_color color);


/**
* \brief Frees image memory
* \param image Target image to be freed
* \warning Image cannot be used after deleting
*/
void sft_image_delete(sft_image* image);

/**
* \brief Copies pixels from a source image to a destination image
* \param dest Destination image to draw to
* \param src Source image to get pixels from
* \param srcX Leftmost position to get pixels from source
* \param srcY Topmost position to get pixels from source
* \param srcW Width of copied image
* \param srcW Height of copied image
* \param destX Leftmost position to copy to on destination image
* \param destY Topmost position to copy to on destination image
*/
void sft_image_drawImage(sft_image* dest, const sft_image* src,
    int32_t srcX, int32_t srcY, uint32_t srcW, uint32_t srcH,
    int32_t destX, int32_t destY);

/**
* \brief Fills pixels in rectangle with a color
* \param dest Destination image to draw to
* \param x Leftmost position of rectangle
* \param y Topmost positionof rectangle
* \param w Width of rectangle
* \param h Height of rectangle
* \param color Color to fill rectangle with
*/
void sft_image_drawRect(sft_image* dest, 
    int32_t x, int32_t y, uint32_t w, uint32_t h, sft_color color);

/**
* \brief Outlines pixels in rectangle with a color
* \param dest Destination image to draw to
* \param x Leftmost position of rectangle
* \param y Topmost positionof rectangle
* \param w Width of rectangle
* \param h Height of rectangle
* \param color Color to outline rectangle with
*/
void sft_image_outlineRect(sft_image* dest,
    int32_t x, int32_t y, uint32_t w, uint32_t h, sft_color color);

/**
* \brief Internal function used by draws. Crops rectangles to fit in destination images
* \param x Leftmost position
* \param y Topmost position
* \param w Width
* \param h Height
* \param width Destination image width
* \param height Destination image height
*/
inline void _sft_image_adjustRect(int32_t* x, int32_t* y, uint32_t* w, uint32_t* h, 
    uint64_t width, uint64_t height);

/**
* \brief Draws a string to an image
* \param dest The image to draw to
* \param text The string to draw
* \param x The leftmost position
* \param y the topmost position
* \param fontSize The font size
* \param color The color of the text
*/
void sft_image_drawText(sft_image* dest, const char* text,
    int32_t x, int32_t y, uint32_t fontSize, sft_color color);

/**
* \brief Draws a string to an image
* \param dest The image to draw to
* \param x The leftmost position
* \param y the topmost position
* \param fontSize The font size
* \param color The color of the text
* \param fmt formatted string
*/
void sft_image_drawTextF(sft_image* dest, int32_t x, int32_t y, 
    uint32_t fontSize, sft_color color, const char* fmt, ...);


/**
* \brief Mostly for internal use, but could be useful. 
Draws a character to an image
* \param dest The image to draw to
* \param ch The character to draw
* \param x The leftmost position
* \param y the topmost position
* \param fontSize The font size
* \param color The color of the text
*/
void sft_image_drawChar(sft_image* dest, char ch,
    int32_t x, int32_t y, uint32_t fontSize, sft_color color);

#ifdef __cplusplus
}
#endif