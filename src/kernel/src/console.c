#include "console.h"
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "ports.h"

// TODO: combine with stdio.h functions

// TODO: read from BIOS data area
#define BASE_PORT 0x3D4

#define X_MAX 80
#define Y_MAX 25

#define BLANK 0x20

#define COLOR_BLACK         0x0
#define COLOR_BLUE          0x1
#define COLOR_GREEN         0x2
#define COLOR_CYAN          0x3
#define COLOR_RED           0x4
#define COLOR_MAGENTA       0x5
#define COLOR_BROWN         0x6
#define COLOR_LIGHT_GRAY    0x7
#define COLOR_DARK_GRAY     0x8
#define COLOR_LIGHT_BLUE    0x9
#define COLOR_LIGHT_GREEN   0xA
#define COLOR_LIGHT_CYAN    0xB
#define COLOR_LIGHT_RED     0xC
#define COLOR_LIGHT_MAGENTA 0xD
#define COLOR_YELLOW        0xE
#define COLOR_WHITE         0xF

#define DEF_TEXT COLOR_LIGHT_GRAY
#define DEF_BACK COLOR_BLACK

static int pos_x = 0;
static int pos_y = 0;
static uint16_t *videoram = (uint16_t *) 0xB8000;

// Encodes the character c with the text and background color.
static inline uint16_t code(char c, uint16_t color_text, uint16_t color_back)
{
    return c | (((color_back << 4) | color_text) << 8);
}

// Moves the cursor after the last written position.
static void update_cursor()
{
    uint16_t pos = pos_y * X_MAX + pos_x;

    outb(BASE_PORT, 0x0E);
    outb(BASE_PORT + 1, pos >> 8);

    outb(BASE_PORT, 0x0F);
    outb(BASE_PORT + 1, pos);
}

// Clears the screen with the default text and background color.
void kclear()
{
    int i;

    for (i = 0; i < Y_MAX * X_MAX; i++)
    {
        videoram[i] = code(BLANK, DEF_TEXT, DEF_BACK);
    }

    pos_x = pos_y = 0;
    update_cursor();
}

// Moves the contents of every line one up and clears the last.
static void scroll()
{
    int i;

    memmove(videoram, videoram + X_MAX, (Y_MAX - 1) * X_MAX * 2 - 1);

    for (i = (Y_MAX - 1) * X_MAX; i < Y_MAX * X_MAX; i++)
    {
        videoram[i] = code(BLANK, DEF_TEXT, DEF_BACK);
    }

    pos_y--;
}

// Prints a char with default text and background color on the screen
// and moves the lines up if the screen is full.
static void print_char(char c)
{
    // If the end of a line is reached, '\n' will be ignored
    // without generating an empty line.
    if (c == '\n' || pos_x >= X_MAX)
    {
        pos_x = 0;
        pos_y++;
    }

    if (pos_y >= Y_MAX)
    {
        scroll();
    }

    if (c != '\n')
    {
        videoram[pos_y * X_MAX + pos_x] = code(c, DEF_TEXT, DEF_BACK);
        pos_x++;
    }
}

// Prints a string to the screen, uses print_char
static void print_string(const char *s)
{
    while (*s != '\0')
    {
        print_char(*s);
        s++;
    }
}

// Prints an integer to the screen converted on a specific base
static void print_int(uint32_t i, int base)
{
    // negative modulo trick
    // "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
    const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char buf[65];
    char *ptr;

    ptr = buf + 64;
    *ptr = '\0';

    do
    {
        *--ptr = digits[i % base];
    } while (i /= base);

    print_string(ptr);
}

// TODO: extend functionality
// Formats a string and prints it to the screen.
void kprintf(const char *format, ...)
{
    va_list ptr;
    va_start(ptr, format);

    while (*format != '\0')
    {
        if (*format == '%')
        {
            switch (*++format)
            {
                case 'o':
                    print_int(va_arg(ptr, uint32_t), 8);
                    break;
                case 'u':
                    print_int(va_arg(ptr, uint32_t), 10);
                    break;
                case 'x':
                    print_int(va_arg(ptr, uint32_t), 16);
                    break;
                case 'c':
                    print_char(va_arg(ptr, int));
                    break;
                case 's':
                    print_string(va_arg(ptr, char*));
                    break;
                case 'p':
                    print_int(va_arg(ptr, uint32_t), 16);
                    break;
                case '\0':
                    print_char('%');
                    format--;
                    break;
                case '%':
                    print_char('%');
                    break;
                default:
                    print_char('%');
                    print_char(*format);
                    break;
            }
        }
        else
        {
            print_char(*format);
        }

        format++;
    }

    va_end(ptr);
    update_cursor();
}
