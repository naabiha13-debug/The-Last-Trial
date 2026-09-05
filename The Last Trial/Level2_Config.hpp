#ifndef LEVEL2_CONFIG_HPP
#define LEVEL2_CONFIG_HPP

const int L2_SCREEN_WIDTH = 1000;
const int L2_SCREEN_HEIGHT = 600;
const int L2_VIEWPORT_HEIGHT = 300;

const int L2_BG_TILE_WIDTH = 1000;
const int L2_BG_TILE_HEIGHT = 300;
const int L2_BG_TILE_COUNT = 6;

const int L2_BRIDGE_TILE_COUNT = 52;


// Bridge tile Y position (player's viewport: 300-600)
const int L2_PLAYER_TILE_Y = 394;

// Mirrored bridge tile Y position for bot (bot's viewport: 0-300)
const int L2_BOT_TILE_Y = 92;

// Bot's feet position when standing on a bridge tile (tile top = L2_BOT_TILE_Y + tile height)
const int L2_BOT_GROUND_Y = L2_BOT_TILE_Y + 7;


// =====================================
// HEALTH SYSTEM
// =====================================
const float L2_HEALTH_MAX = 100.0f;
const DWORD L2_HEALTH_DECAY_INTERVAL = 3000;
const float L2_HEALTH_DECAY_AMOUNT = 15.0f;
const float L2_HEALTH_GAIN_AMOUNT = 35.0f;
const float L2_MIN_SPEED_FACTOR = 0.3f;
const int L2_MIN_RUN_SPEED = 2;
const int L2_BASE_RUN_SPEED = 10;
const int L2_BISCUIT_COUNT = 6;
const int L2_BISCUIT_CATCH_RANGE = 40;
const int L2_BISCUIT_HEIGHT_OFFSET = 70;   

#endif