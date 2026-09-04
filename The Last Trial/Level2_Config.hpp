#ifndef LEVEL2_CONFIG_HPP
#define LEVEL2_CONFIG_HPP

const int L2_SCREEN_WIDTH = 1000;
const int L2_SCREEN_HEIGHT = 600;
const int L2_VIEWPORT_HEIGHT = 300;

const int L2_BG_TILE_WIDTH = 1000;
const int L2_BG_TILE_HEIGHT = 300;
const int L2_BG_TILE_COUNT = 5;

// Bridge tile Y position (player's viewport: 300-600)
const int L2_PLAYER_TILE_Y = 394;

// Mirrored bridge tile Y position for bot (bot's viewport: 0-300)
const int L2_BOT_TILE_Y = 92;

// Bot's feet position when standing on a bridge tile (tile top = L2_BOT_TILE_Y + tile height)
const int L2_BOT_GROUND_Y = L2_BOT_TILE_Y + 7;



#endif