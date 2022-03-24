#pragma once

#include "resource.h"
#include "peace.h"
#include "Zpeace.h"
#include "Speace.h"
#include "Opeace.h"
#include "Ipeace.h"
#include "Jpeace.h"
#include "Lpeace.h"
#include "Tpeace.h"

// private Resource
#define MAX_LOADSTRING 100
#define MARGIN 10
#define BOARD_LINE_WIDTH 1
#define BOARD_HEIGHT 21
#define BOARD_WIDTH 10
#define BLOCK_SIZE 30
#define CLIENT_WIDTH (MARGIN*2 + (BOARD_WIDTH * BLOCK_SIZE))
#define CLIENT_HEIGHT (MARGIN*2 + (BOARD_HEIGHT * BLOCK_SIZE))
#define TIMER_SPAN 10

extern int colors[][2] = {
    {RGB(255,100,0),RGB(255,0,0)},
    {RGB(100,255,0),RGB(0,255,0)},
    {RGB(255,255,0),RGB(100,100,0)},
    {RGB(170,202,255),RGB(50,50,200)},
    {RGB(0,0,255),RGB(150,150,200)},
    {RGB(240,128,128),RGB(200,50,100)},
    {RGB(167,87,168),RGB(200,0,255)}
};
