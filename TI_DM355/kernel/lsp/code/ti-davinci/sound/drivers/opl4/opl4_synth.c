/*
 * OPL4 MIDI synthesizer functions
 *
 * Copyright (c) 2003 by Clemens Ladisch <clemens@ladisch.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed and/or modified under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "opl4_local.h"
#include <linux/delay.h>
#include <asm/io.h>
#include <sound/asoundef.h>

/* GM2 controllers */
#ifndef MIDI_CTL_RELEASE_TIME
#define MIDI_CTL_RELEASE_TIME	0x48
#define MIDI_CTL_ATTACK_TIME	0x49
#define MIDI_CTL_DECAY_TIME	0x4b
#define MIDI_CTL_VIBRATO_RATE	0x4c
#define MIDI_CTL_VIBRATO_DEPTH	0x4d
#define MIDI_CTL_VIBRATO_DELAY	0x4e
#endif

/*
 * This table maps 100/128 cents to F_NUMBER.
 */
static const s16 snd_opl4_pitch_map[0x600] = {
	0x000,0x000,0x001,0x001,0x002,0x002,0x003,0x003,
	0x004,0x004,0x005,0x005,0x006,0x006,0x006,0x007,
	0x007,0x008,0x008,0x009,0x009,0x00a,0x00a,0x00b,
	0x00b,0x00c,0x00c,0x00d,0x00d,0x00d,0x00e,0x00e,
	0x00f,0x00f,0x010,0x010,0x011,0x011,0x012,0x012,
	0x013,0x013,0x014,0x014,0x015,0x015,0x015,0x016,
	0x016,0x017,0x017,0x018,0x018,0x019,0x019,0x01a,
	0x01a,0x01b,0x01b,0x01c,0x01c,0x01d,0x01d,0x01e,
	0x01e,0x01e,0x01f,0x01f,0x020,0x020,0x021,0x021,
	0x022,0x022,0x023,0x023,0x024,0x024,0x025,0x025,
	0x026,0x026,0x027,0x027,0x028,0x028,0x029,0x029,
	0x029,0x02a,0x02a,0x02b,0x02b,0x02c,0x02c,0x02d,
	0x02d,0x02e,0x02e,0x02f,0x02f,0x030,0x030,0x031,
	0x031,0x032,0x032,0x033,0x033,0x034,0x034,0x035,
	0x035,0x036,0x036,0x037,0x037,0x038,0x038,0x038,
	0x039,0x039,0x03a,0x03a,0x03b,0x03b,0x03c,0x03c,
	0x03d,0x03d,0x03e,0x03e,0x03f,0x03f,0x040,0x040,
	0x041,0x041,0x042,0x042,0x043,0x043,0x044,0x044,
	0x045,0x045,0x046,0x046,0x047,0x047,0x048,0x048,
	0x049,0x049,0x04a,0x04a,0x04b,0x04b,0x04c,0x04c,
	0x04d,0x04d,0x04e,0x04e,0x04f,0x04f,0x050,0x050,
	0x051,0x051,0x052,0x052,0x053,0x053,0x054,0x054,
	0x055,0x055,0x056,0x056,0x057,0x057,0x058,0x058,
	0x059,0x059,0x05a,0x05a,0x05b,0x05b,0x05c,0x05c,
	0x05d,0x05d,0x05e,0x05e,0x05f,0x05f,0x060,0x060,
	0x061,0x061,0x062,0x062,0x063,0x063,0x064,0x064,
	0x065,0x065,0x066,0x066,0x067,0x067,0x068,0x068,
	0x069,0x069,0x06a,0x06a,0x06b,0x06b,0x06c,0x06c,
	0x06d,0x06d,0x06e,0x06e,0x06f,0x06f,0x070,0x071,
	0x071,0x072,0x072,0x073,0x073,0x074,0x074,0x075,
	0x075,0x076,0x076,0x077,0x077,0x078,0x078,0x079,
	0x079,0x07a,0x07a,0x07b,0x07b,0x07c,0x07c,0x07d,
	0x07d,0x07e,0x07e,0x07f,0x07f,0x080,0x081,0x081,
	0x082,0x082,0x083,0x083,0x084,0x084,0x085,0x085,
	0x086,0x086,0x087,0x087,0x088,0x088,0x089,0x089,
	0x08a,0x08a,0x08b,0x08b,0x08c,0x08d,0x08d,0x08e,
	0x08e,0x08f,0x08f,0x090,0x090,0x091,0x091,0x092,
	0x092,0x093,0x093,0x094,0x094,0x095,0x096,0x096,
	0x097,0x097,0x098,0x098,0x099,0x099,0x09a,0x09a,
	0x09b,0x09b,0x09c,0x09c,0x09d,0x09d,0x09e,0x09f,
	0x09f,0x0a0,0x0a0,0x0a1,0x0a1,0x0a2,0x0a2,0x0a3,
	0x0a3,0x0a4,0x0a4,0x0a5,0x0a6,0x0a6,0x0a7,0x0a7,
	0x0a8,0x0a8,0x0a9,0x0a9,0x0aa,0x0aa,0x0ab,0x0ab,
	0x0ac,0x0ad,0x0ad,0x0ae,0x0ae,0x0af,0x0af,0x0b0,
	0x0b0,0x0b1,0x0b1,0x0b2,0x0b2,0x0b3,0x0b4,0x0b4,
	0x0b5,0x0b5,0x0b6,0x0b6,0x0b7,0x0b7,0x0b8,0x0b8,
	0x0b9,0x0ba,0x0ba,0x0bb,0x0bb,0x0bc,0x0bc,0x0bd,
	0x0bd,0x0be,0x0be,0x0bf,0x0c0,0x0c0,0x0c1,0x0c1,
	0x0c2,0x0c2,0x0c3,0x0c3,0x0c4,0x0c4,0x0c5,0x0c6,
	0x0c6,0x0c7,0x0c7,0x0c8,0x0c8,0x0c9,0x0c9,0x0ca,
	0x0cb,0x0cb,0x0cc,0x0cc,0x0cd,0x0cd,0x0ce,0x0ce,
	0x0cf,0x0d0,0x0d0,0x0d1,0x0d1,0x0d2,0x0d2,0x0d3,
	0x0d3,0x0d4,0x0d5,0x0d5,0x0d6,0x0d6,0x0d7,0x0d7,
	0x0d8,0x0d8,0x0d9,0x0da,0x0da,0x0db,0x0db,0x0dc,
	0x0dc,0x0dd,0x0de,0x0de,0x0df,0x0df,0x0e0,0x0e0,
	0x0e1,0x0e1,0x0e2,0x0e3,0x0e3,0x0e4,0x0e4,0x0e5,
	0x0e5,0x0e6,0x0e7,0x0e7,0x0e8,0x0e8,0x0e9,0x0e9,
	0x0ea,0x0eb,0x0eb,0x0ec,0x0ec,0x0ed,0x0ed,0x0ee,
	0x0ef,0x0ef,0x0f0,0x0f0,0x0f1,0x0f1,0x0f2,0x0f3,
	0x0f3,0x0f4,0x0f4,0x0f5,0x0f5,0x0f6,0x0f7,0x0f7,
	0x0f8,0x0f8,0x0f9,0x0f9,0x0fa,0x0fb,0x0fb,0x0fc,
	0x0fc,0x0fd,0x0fd,0x0fe,0x0ff,0x0ff,0x100,0x100,
	0x101,0x101,0x102,0x103,0x103,0x104,0x104,0x105,
	0x106,0x106,0x107,0x107,0x108,0x108,0x109,0x10a,
	0x10a,0x10b,0x10b,0x10c,0x10c,0x10d,0x10e,0x10e,
	0x10f,0x10f,0x110,0x111,0x111,0x112,0x112,0x113,
	0x114,0x114,0x115,0x115,0x116,0x116,0x117,0x118,
	0x118,0x119,0x119,0x11a,0x11b,0x11b,0x11c,0x11c,
	0x11d,0x11e,0x11e,0x11f,0x11f,0x120,0x120,0x121,
	0x122,0x122,0x123,0x123,0x124,0x125,0x125,0x126,
	0x126,0x127,0x128,0x128,0x129,0x129,0x12a,0x12b,
	0x12b,0x12c,0x12c,0x12d,0x12e,0x12e,0x12f,0x12f,
	0x130,0x131,0x131,0x132,0x132,0x133,0x134,0x134,
	0x135,0x135,0x136,0x137,0x137,0x138,0x138,0x139,
	0x13a,0x13a,0x13b,0x13b,0x13c,0x13d,0x13d,0x13e,
	0x13e,0x13f,0x140,0x140,0x141,0x141,0x142,0x143,
	0x143,0x144,0x144,0x145,0x146,0x146,0x147,0x148,
	0x148,0x149,0x149,0x14a,0x14b,0x14b,0x14c,0x14c,
	0x14d,0x14e,0x14e,0x14f,0x14f,0x150,0x151,0x151,
	0x152,0x153,0x153,0x154,0x154,0x155,0x156,0x156,
	0x157,0x157,0x158,0x159,0x159,0x15a,0x15b,0x15b,
	0x15c,0x15c,0x15d,0x15e,0x15e,0x15f,0x160,0x160,
	0x161,0x161,0x162,0x163,0x163,0x164,0x165,0x165,
	0x166,0x166,0x167,0x168,0x168,0x169,0x16a,0x16a,
	0x16b,0x16b,0x16c,0x16d,0x16d,0x16e,0x16f,0x16f,
	0x170,0x170,0x171,0x172,0x172,0x173,0x174,0x174,
	0x175,0x175,0x176,0x177,0x177,0x178,0x179,0x179,
	0x17a,0x17a,0x17b,0x17c,0x17c,0x17d,0x17e,0x17e,
	0x17f,0x180,0x180,0x181,0x181,0x182,0x183,0x183,
	0x184,0x185,0x185,0x186,0x187,0x187,0x188,0x188,
	0x189,0x18a,0x18a,0x18b,0x18c,0x18c,0x18d,0x18e,
	0x18e,0x18f,0x190,0x190,0x191,0x191,0x192,0x193,
	0x193,0x194,0x195,0x195,0x196,0x197,0x197,0x198,
	0x199,0x199,0x19a,0x19a,0x19b,0x19c,0x19c,0x19d,
	0x19e,0x19e,0x19f,0x1a0,0x1a0,0x1a1,0x1a2,0x1a2,
	0x1a3,0x1a4,0x1a4,0x1a5,0x1a6,0x1a6,0x1a7,0x1a8,
	0x1a8,0x1a9,0x1a9,0x1aa,0x1ab,0x1ab,0x1ac,0x1ad,
	0x1ad,0x1ae,0x1af,0x1af,0x1b0,0x1b1,0x1b1,0x1b2,
	0x1b3,0x1b3,0x1b4,0x1b5,0x1b5,0x1b6,0x1b7,0x1b7,
	0x1b8,0x1b9,0x1b9,0x1ba,0x1bb,0x1bb,0x1bc,0x1bd,
	0x1bd,0x1be,0x1bf,0x1bf,0x1c0,0x1c1,0x1c1,0x1c2,
	0x1c3,0x1c3,0x1c4,0x1c5,0x1c5,0x1c6,0x1c7,0x1c7,
	0x1c8,0x1c9,0x1c9,0x1ca,0x1cb,0x1cb,0x1cc,0x1cd,
	0x1cd,0x1ce,0x1cf,0x1cf,0x1d0,0x1d1,0x1d1,0x1d2,
	0x1d3,0x1d3,0x1d4,0x1d5,0x1d5,0x1d6,0x1d7,0x1d7,
	0x1d8,0x1d9,0x1d9,0x1da,0x1db,0x1db,0x1dc,0x1dd,
	0x1dd,0x1de,0x1df,0x1df,0x1e0,0x1e1,0x1e1,0x1e2,
	0x1e3,0x1e4,0x1e4,0x1e5,0x1e6,0x1e6,0x1e7,0x1e8,
	0x1e8,0x1e9,0x1ea,0x1ea,0x1eb,0x1ec,0x1ec,0x1ed,
	0x1ee,0x1ee,0x1ef,0x1f0,0x1f0,0x1f1,0x1f2,0x1f3,
	0x1f3,0x1f4,0x1f5,0x1f5,0x1f6,0x1f7,0x1f7,0x1f8,
	0x1f9,0x1f9,0x1fa,0x1fb,0x1fb,0x1fc,0x1fd,0x1fe,
	0x1fe,0x1ff,0x200,0x200,0x201,0x202,0x202,0x203,
	0x204,0x205,0x205,0x206,0x207,0x207,0x208,0x209,
	0x209,0x20a,0x20b,0x20b,0x20c,0x20d,0x20e,0x20e,
	0x20f,0x210,0x210,0x211,0x212,0x212,0x213,0x214,
	0x215,0x215,0x216,0x217,0x217,0x218,0x219,0x21a,
	0x21a,0x21b,0x21c,0x21c,0x21d,0x21e,0x21e,0x21f,
	0x220,0x221,0x221,0x222,0x223,0x223,0x224,0x225,
	0x226,0x226,0x227,0x228,0x228,0x229,0x22a,0x22b,
	0x22b,0x22c,0x22d,0x22d,0x22e,0x22f,0x230,0x230,
	0x231,0x232,0x232,0x233,0x234,0x235,0x235,0x236,
	0x237,0x237,0x238,0x239,0x23a,0x23a,0x23b,0x23c,
	0x23c,0x23d,0x23e,0x23f,0x23f,0x240,0x241,0x241,
	0x242,0x243,0x244,0x244,0x245,0x246,0x247,0x247,
	0x248,0x249,0x249,0x24a,0x24b,0x24c,0x24c,0x24d,
	0x24e,0x24f,0x24f,0x250,0x251,0x251,0x252,0x253,
	0x254,0x254,0x255,0x256,0x257,0x257,0x258,0x259,
	0x259,0x25a,0x25b,0x25c,0x25c,0x25d,0x25e,0x25f,
	0x25f,0x260,0x261,0x262,0x262,0x263,0x264,0x265,
	0x265,0x266,0x267,0x267,0x268,0x269,0x26a,0x26a,
	0x26b,0x26c,0x26d,0x26d,0x26e,0x26f,0x270,0x270,
	0x271,0x272,0x273,0x273,0x274,0x275,0x276,0x276,
	0x277,0x278,0x279,0x279,0x27a,0x27b,0x27c,0x27c,
	0x27d,0x27e,0x27f,0x27f,0x280,0x281,0x282,0x282,
	0x283,0x284,0x285,0x285,0x286,0x287,0x288,0x288,
	0x289,0x28a,0x28b,0x28b,0x28c,0x28d,0x28e,0x28e,
	0x28f,0x290,0x291,0x291,0x292,0x293,0x294,0x294,
	0x295,0x296,0x297,0x298,0x298,0x299,0x29a,0x29b,
	0x29b,0x29c,0x29d,0x29e,0x29e,0x29f,0x2a0,0x2a1,
	0x2a1,0x2a2,0x2a3,0x2a4,0x2a5,0x2a5,0x2a6,0x2a7,
	0x2a8,0x2a8,0x2a9,0x2aa,0x2ab,0x2ab,0x2ac,0x2ad,
	0x2ae,0x2af,0x2af,0x2b0,0x2b1,0x2b2,0x2b2,0x2b3,
	0x2b4,0x2b5,0x2b5,0x2b6,0x2b7,0x2b8,0x2b9,0x2b9,
	0x2ba,0x2bb,0x2bc,0x2bc,0x2bd,0x2be,0x2bf,0x2c0,
	0x2c0,0x2c1,0x2c2,0x2c3,0x2c4,0x2c4,0x2c5,0x2c6,
	0x2c7,0x2c7,0x2c8,0x2c9,0x2ca,0x2cb,0x2cb,0x2cc,
	0x2cd,0x2ce,0x2ce,0x2cf,0x2d0,0x2d1,0x2d2,0x2d2,
	0x2d3,0x2d4,0x2d5,0x2d6,0x2d6,0x2d7,0x2d8,0x2d9,
	0x2da,0x2da,0x2db,0x2dc,0x2dd,0x2dd,0x2de,0x2df,
	0x2e0,0x2e1,0x2e1,0x2e2,0x2e3,0x2e4,0x2e5,0x2e5,
	0x2e6,0x2e7,0x2e8,0x2e9,0x2e9,0x2ea,0x2eb,0x2ec,
	0x2ed,0x2ed,0x2ee,0x2ef,0x2f0,0x2f1,0x2f1,0x2f2,
	0x2f3,0x2f4,0x2f5,0x2f5,0x2f6,0x2f7,0x2f8,0x2f9,
	0x2f9,0x2fa,0x2fb,0x2fc,0x2fd,0x2fd,0x2fe,0x2ff,
	0x300,0x301,0x302,0x302,0x303,0x304,0x305,0x306,
	0x306,0x307,0x308,0x309,0x30a,0x30a,0x30b,0x30c,
	0x30d,0x30e,0x30f,0x30f,0x310,0x311,0x312,0x313,
	0x313,0x314,0x315,0x316,0x317,0x318,0x318,0x319,
	0x31a,0x31b,0x31c,0x31c,0x31d,0x31e,0x31f,0x320,
	0x321,0x321,0x322,0x323,0x324,0x325,0x326,0x326,
	0x327,0x328,0x329,0x32a,0x32a,0x32b,0x32c,0x32d,
	0x32e,0x32f,0x32f,0x330,0x331,0x332,0x333,0x334,
	0x334,0x335,0x336,0x337,0x338,0x339,0x339,0x33a,
	0x33b,0x33c,0x33d,0x33e,0x33e,0x33f,0x340,0x341,
	0x342,0x343,0x343,0x344,0x345,0x346,0x347,0x348,
	0x349,0x349,0x34a,0x34b,0x34c,0x34d,0x34e,0x34e,
	0x34f,0x350,0x351,0x352,0x353,0x353,0x354,0x355,
	0x356,0x357,0x358,0x359,0x359,0x35a,0x35b,0x35c,
	0x35d,0x35e,0x35f,0x35f,0x360,0x361,0x362,0x363,
	0x364,0x364,0x365,0x366,0x367,0x368,0x369,0x36a,
	0x36a,0x36b,0x36c,0x36d,0x36e,0x36f,0x370,0x370,
	0x371,0x372,0x373,0x374,0x375,0x376,0x377,0x377,
	0x378,0x379,0x37a,0x37b,0x37c,0x37d,0x37d,0x37e,
	0x37f,0x380,0x381,0x382,0x383,0x383,0x384,0x385,
	0x386,0x387,0x388,0x389,0x38a,0x38a,0x38b,0x38c,
	0x38d,0x38e,0x38f,0x390,0x391,0x391,0x392,0x393,
	0x394,0x395,0x396,0x397,0x398,0x398,0x399,0x39a,
	0x39b,0x39c,0x39d,0x39e,0x39f,0x39f,0x3a0,0x3a1,
	0x3a2,0x3a3,0x3a4,0x3a5,0x3a6,0x3a7,0x3a7,0x3a8,
	0x3a9,0x3aa,0x3ab,0x3ac,0x3ad,0x3ae,0x3ae,0x3af,
	0x3b0,0x3b1,0x3b2,0x3b3,0x3b4,0x3b5,0x3b6,0x3b6,
	0x3b7,0x3b8,0x3b9,0x3ba,0x3bb,0x3bc,0x3bd,0x3be,
	0x3bf,0x3bf,0x3c0,0x3c1,0x3c2,0x3c3,0x3c4,0x3c5,
	0x3c6,0x3c7,0x3c7,0x3c8,0x3c9,0x3ca,0x3cb,0x3cc,
	0x3cd,0x3ce,0x3cf,0x3d0,0x3d1,0x3d1,0x3d2,0x3d3,
	0x3d4,0x3d5,0x3d6,0x3d7,0x3d8,0x3d9,0x3da,0x3da,
	0x3db,0x3dc,0x3dd,0x3de,0x3df,0x3e0,0x3e1,0x3e2,
	0x3e3,0x3e4,0x3e4,0x3e5,0x3e6,0x3e7,0x3e8,0x3e9,
	0x3ea,0x3eb,0x3ec,0x3ed,0x3ee,0x3ef,0x3ef,0x3f0,
	0x3f1,0x3f2,0x3f3,0x3f4,0x3f5,0x3f6,0x3f7,0x3f8,
	0x3f9,0x3fa,0x3fa,0x3fb,0x3fc,0x3fd,0x3fe,0x3ff
};

/*
 * Attenuation according to GM recommendations, in -0.375 dB units.
 * table[v] = 40 * log(v / 127) / -0.375
 */
static unsigned char snd_opl4_volume_table[128] = {
	255,224,192,173,160,150,141,134,
	128,122,117,113,109,105,102, 99,
	 96, 93, 90, 88, 85, 83, 81, 79,
	 77, 75, 73, 71, 70, 68, 67, 65,
	 64, 62, 61, 59, 58, 57, 56, 54,
	 53, 52, 51, 50, 49, 48, 47, 46,
	 45, 44, 43, 42, 41, 40, 39, 39,
	 38, 37, 36, 35, 34, 34, 33, 32,
	 31, 31, 30, 29, 29, 28, 27, 27,
	 26, 25, 25, 24, 24, 23, 22, 22,
	 21, 21, 20, 19, 19, 18, 18, 17,
	 17, 16, 16, 15, 15, 14, 14, 13,
	 13, 12, 12, 11, 11, 10, 10,  9,
	  9,  9,  8,  8,  7,  7,  6,  6,
	  6,  5,  5,  4,  4,  4,  3,  3,
	  2,  2,  2,  1,  1,  0,  0,  0
};

/*
 * Initializes all voices.
 */
void snd_opl4_synth_reset(opl4_t *opl4)
{
	unsigned long flags;
	int i;

	spin_lock_irqsave(&opl4->reg_lock, flags);
	for (i = 0; i < OPL4_MAX_VOICES; i++)
		snd_opl4_write(opl4, OPL4_REG_MISC + i, OPL4_DAMP_BIT);
	spin_unlock_irqrestore(&opl4->reg_lock, flags);

	INIT_LIST_HEAD(&opl4->off_voices);
	INIT_LIST_HEAD(&opl4->on_voices);
	memset(opl4->voices, 0, sizeof(opl4->voices));
	for (i = 0; i < OPL4_MAX_VOICES; i++) {
		opl4->voices[i].number = i;
		list_add_tail(&opl4->voices[i].list, &opl4->off_voices);
	}

	snd_midi_channel_set_clear(opl4->chset);
}

/*
 * Shuts down all voices.
 */
void snd_opl4_synth_shutdown(opl4_t *opl4)
{
	unsigned long flags;
	int i;

	spin_lock_irqsave(&opl4->reg_lock, flags);
	for (i = 0; i < OPL4_MAX_VOICES; i++)
		snd_opl4_write(opl4, OPL4_REG_MISC + i,
			       opl4->voices[i].reg_misc & ~OPL4_KEY_ON_BIT);
	spin_unlock_irqrestore(&opl4->reg_lock, flags);
}

/*
 * Executes the callback for all voices playing the specified note.
 */
static void snd_opl4_do_for_note(opl4_t *opl4, int note, snd_midi_channel_t *chan,
				 void (*func)(opl4_t *opl4, opl4_voice_t *voice))
{
	int i;
	unsigned long flags;
	opl4_voice_t *voice;

	spin_lock_irqsave(&opl4->reg_lock, flags);
	for (i = 0; i < OPL4_MAX_VOICES; i++) {
		voice = &opl4->voices[i];
		if (voice->chan == chan && voice->note == note) {
			func(opl4, voice);
		}
	}
	spin_unlock_irqrestore(&opl4->reg_lock, flags);
}

/*
 * Executes the callback for all voices of to the specified channel.
 */
static void snd_opl4_do_for_channel(opl4_t *opl4, snd_midi_channel_t *chan,
				    void (*func)(opl4_t *opl4, opl4_voice_t *voice))
{
	int i;
	unsigned long flags;
	opl4_voice_t *voice;

	spin_lock_irqsave(&opl4->reg_lock, flags);
	for (i = 0; i < OPL4_MAX_VOICES; i++) {
		voice = &opl4->voices[i];
		if (voice->chan == chan) {
			func(opl4, voice);
		}
	}
	spin_unlock_irqrestore(&opl4->reg_lock, flags);
}

/*
 * Executes the callback for all active voices.
 */
static void snd_opl4_do_for_all(opl4_t *opl4,
				void (*func)(opl4_t *opl4, opl4_voice_t *voice))
{
	int i;
	unsigned long flags;
	opl4_voice_t *voice;

	spin_lock_irqsave(&opl4->reg_lock, flags);
	for (i = 0; i < OPL4_MAX_VOICES; i++) {
		voice = &opl4->voices[i];
		if (voice->chan)
			func(opl4, voice);
	}
	spin_unlock_irqrestore(&opl4->reg_lock, flags);
}

static void snd_opl4_update_volume(opl4_t *opl4, opl4_voice_t *voice)
{
	int att;

	att = voice->sound->tone_attenuate;
	att += snd_opl4_volume_table[opl4->chset->gs_master_volume & 0x7f];
	att += snd_opl4_volume_table[voice->chan->gm_volume & 0x7f];
	att += snd_opl4_volume_table[voice->chan->gm_expression & 0x7f];
	att += snd_opl4_volume_table[voice->velocity];
	att = 0x7f - (0x7f - att) * (voice->sound->volume_factor) / 0xfe - volume_boost;
	if (att < 0)
		att = 0;
	else if (att > 0x7e)
		att = 0x7e;
	snd_opl4_write(opl4, OPL4_REG_LEVEL + voice->number,
		       (att << 1) | voice->level_direct);
	voice->level_direct = 0;
}

static void snd_opl4_update_pan(opl4_t *opl4, opl4_voice_t *voice)
{
	int pan = voice->sound->panpot;

	if (!voice->chan->drum_channel)
		pan += (voice->chan->control[MIDI_CTL_MSB_PAN] - 0x40) >> 3;
	if (pan < -7)
		pan = -7;
	else if (pan > 7)
		pan = 7;
	voice->reg_misc = (voice->reg_misc & ~OPL4_PAN_POT_MASK)
		| (pan & OPL4_PAN_POT_MASK);
	snd_opl4_write(opl4, OPL4_REG_MISC + voice->number, voice->reg_misc);
}

static void snd_opl4_update_vibrato_depth(opl4_t *opl4, opl4_voice_t *voice)
{
	int depth;

	if (voice->chan->drum_channel)
		return;
	depth = (7 - voice->sound->vibrato)
		* (voice->chan->control[MIDI_CTL_VIBRATO_DEPTH] & 0x7f);
	depth = (depth >> 7) + voice->sound->vibrato;
	voice->reg_lfo_vibrato &= ~OPL4_VIBRATO_DEPTH_MASK;
	voice->reg_lfo_vibrato |= depth & OPL4_VIBRATO_DEPTH_MASK;
	snd_opl4_write(opl4, OPL4_REG_LFO_VIBRATO + voice->number,
		       voice->reg_lfo_vibrato);
}

static void snd_opl4_update_pitch(opl4_t *opl4, opl4_voice_t *voice)
{
	snd_midi_channel_t *chan = voice->chan;
	int note, pitch, octave;

	note = chan->drum_channel ? 60 : voice->note;
	/*
	 * pitch is in 100/128 cents, so 0x80 is one semitone and
	 * 0x600 is one octave.
	 */
	pitch = ((note - 60) << 7) * voice->sound->key_scaling / 100 + (60 << 7);
	pitch += voice->sound->pitch_offset;
	if (!chan->drum_channel)
		pitch += chan->gm_rpn_coarse_tuning;
	pitch += chan->gm_rpn_fine_tuning >> 7;
	pitch += chan->midi_pitchbend * chan->gm_rpn_pitch_bend_range / 0x2000;
	if (pitch < 0)
		pitch = 0;
	else if (pitch >= 0x6000)
		pitch = 0x5fff;
	octave = pitch / 0x600 - 8;
	pitch = snd_opl4_pitch_map[pitch % 0x600];

	snd_opl4_write(opl4, OPL4_REG_OCTAVE + voice->number,
		       (octave << 4) | ((pitch >> 7) & OPL4_F_NUMBER_HIGH_MASK));
	voice->reg_f_number = (voice->reg_f_number & OPL4_TONE_NUMBER_BIT8)
		| ((pitch << 1) & OPL4_F_NUMBER_LOW_MASK);
	snd_opl4_write(opl4, OPL4_REG_F_NUMBER + voice->number, voice->reg_f_number);
}

static void snd_opl4_update_tone_parameters(opl4_t *opl4, opl4_voice_t *voice)
{
	snd_opl4_write(opl4, OPL4_REG_ATTACK_DECAY1 + voice->number,
		       voice->sound->reg_attack_decay1);
	snd_opl4_write(opl4, OPL4_REG_LEVEL_DECAY2 + voice->number,
		       voice->sound->reg_level_decay2);
	snd_opl4_write(opl4, OPL4_REG_RELEASE_CORRECTION + voice->number,
		       voice->sound->reg_release_correction);
	snd_opl4_write(opl4, OPL4_REG_TREMOLO + voice->number,
		       voice->sound->reg_tremolo);
}

/* allocate one voice */
static opl4_voice_t *snd_opl4_get_voice(opl4_t *opl4)
{
	/* first, try to get the oldest key-off voice */
	if (!list_empty(&opl4->off_voices))
		return list_entry(opl4->off_voices.next, opl4_voice_t, list);
	/* then get the oldest key-on voice */
	snd_assert(!list_empty(&opl4->on_voices), );
	return list_entry(opl4->on_voices.next, opl4_voice_t, list);
}

static void snd_opl4_wait_for_wave_headers(opl4_t *opl4)
{
	int timeout = 200;

	while ((inb(opl4->fm_port) & OPL4_STATUS_LOAD) && --timeout > 0)
		udelay(10);
}

void snd_opl4_note_on(void *private_data, int note, int vel, snd_midi_channel_t *chan)
{
	opl4_t *opl4 = private_data;
	const opl4_region_ptr_t *regions;
	opl4_voice_t *voice[2];
	const opl4_sound_t *sound[2];
	int voices = 0, i;
	unsigned long flags;

	/* determine the number of voices and voice parameters */
	i = chan->drum_channel ? 0x80 : (chan->midi_program & 0x7f);
	regions = &snd_yrw801_regions[i];
	for (i = 0; i < regions->count; i++) {
		if (note >= regions->regions[i].key_min &&
		    note <= regions->regions[i].key_max) {
			sound[voices] = &regions->regions[i].sound;
			if (++voices >= 2)
				break;
		}
	}

	/* allocate and initialize the needed voices */
	spin_lock_irqsave(&opl4->reg_lock, flags);
	for (i = 0; i < voices; i++) {
		voice[i] = snd_opl4_get_voice(opl4);
		list_del(&voice[i]->list);
		list_add_tail(&voice[i]->list, &opl4->on_voices);
		voice[i]->chan = chan;
		voice[i]->note = note;
		voice[i]->velocity = vel & 0x7f;
		voice[i]->sound = sound[i];
	}

	/* set tone number (triggers header loading) */
	for (i = 0; i < voices; i++) {
		voice[i]->reg_f_number =
			(sound[i]->tone >> 8) & OPL4_TONE_NUMBER_BIT8;
		snd_opl4_write(opl4, OPL4_REG_F_NUMBER + voice[i]->number,
			       voice[i]->reg_f_number);
		snd_opl4_write(opl4, OPL4_REG_TONE_NUMBER + voice[i]->number,
			       sound[i]->tone & 0xff);
	}

	/* set parameters which can be set while loading */
	for (i = 0; i < voices; i++) {
		voice[i]->reg_misc = OPL4_LFO_RESET_BIT;
		snd_opl4_update_pan(opl4, voice[i]);
		snd_opl4_update_pitch(opl4, voice[i]);
		voice[i]->level_direct = OPL4_LEVEL_DIRECT_BIT;
		snd_opl4_update_volume(opl4, voice[i]);
	}
	spin_unlock_irqrestore(&opl4->reg_lock, flags);

	/* wait for completion of loading */
	snd_opl4_wait_for_wave_headers(opl4);

	/* set remaining parameters */
	spin_lock_irqsave(&opl4->reg_lock, flags);
	for (i = 0; i < voices; i++) {
		snd_opl4_update_tone_parameters(opl4, voice[i]);
		voice[i]->reg_lfo_vibrato = voice[i]->sound->reg_lfo_vibrato;
		snd_opl4_update_vibrato_depth(opl4, voice[i]);
	}

	/* finally, switch on all voices */
	for (i = 0; i < voices; i++) {
		voice[i]->reg_misc =
			(voice[i]->reg_misc & 0x1f) | OPL4_KEY_ON_BIT;
		snd_opl4_write(opl4, OPL4_REG_MISC + voice[i]->number,
			       voice[i]->reg_misc);
	}
	spin_unlock_irqrestore(&opl4->reg_lock, flags);
}

static void snd_opl4_voice_off(opl4_t *opl4, opl4_voice_t *voice)
{
	list_del(&voice->list);
	list_add_tail(&voice->list, &opl4->off_voices);

	voice->reg_misc &= ~OPL4_KEY_ON_BIT;
	snd_opl4_write(opl4, OPL4_REG_MISC + voice->number, voice->reg_misc);
}

void snd_opl4_note_off(void *private_data, int note, int vel, snd_midi_channel_t *chan)
{
	opl4_t *opl4 = private_data;

	snd_opl4_do_for_note(opl4, note, chan, snd_opl4_voice_off);
}

static void snd_opl4_terminate_voice(opl4_t *opl4, opl4_voice_t *voice)
{
	list_del(&voice->list);
	list_add_tail(&voice->list, &opl4->off_voices);

	voice->reg_misc = (voice->reg_misc & ~OPL4_KEY_ON_BIT) | OPL4_DAMP_BIT;
	snd_opl4_write(opl4, OPL4_REG_MISC + voice->number, voice->reg_misc);
}

void snd_opl4_terminate_note(void *private_data, int note, snd_midi_channel_t *chan)
{
	opl4_t *opl4 = private_data;

	snd_opl4_do_for_note(opl4, note, chan, snd_opl4_terminate_voice);
}

void snd_opl4_control(void *private_data, int type, snd_midi_channel_t *chan)
{
	opl4_t *opl4 = private_data;

	switch (type) {
	case MIDI_CTL_MSB_MODWHEEL:
		chan->control[MIDI_CTL_VIBRATO_DEPTH] = chan->control[MIDI_CTL_MSB_MODWHEEL];
		snd_opl4_do_for_channel(opl4, chan, snd_opl4_update_vibrato_depth);
		break;
	case MIDI_CTL_MSB_MAIN_VOLUME:
		snd_opl4_do_for_channel(opl4, chan, snd_opl4_update_volume);
		break;
	case MIDI_CTL_MSB_PAN:
		snd_opl4_do_for_channel(opl4, chan, snd_opl4_update_pan);
		break;
	case MIDI_CTL_MSB_EXPRESSION:
		snd_opl4_do_for_channel(opl4, chan, snd_opl4_update_volume);
		break;
	case MIDI_CTL_VIBRATO_RATE:
		/* not yet supported */
		break;
	case MIDI_CTL_VIBRATO_DEPTH:
		snd_opl4_do_for_channel(opl4, chan, snd_opl4_update_vibrato_depth);
		break;
	case MIDI_CTL_VIBRATO_DELAY:
		/* not yet supported */
		break;
	case MIDI_CTL_E1_REVERB_DEPTH:
		/*
		 * Each OPL4 voice has a bit called "Pseudo-Reverb", but
		 * IMHO _not_ using it enhances the listening experience.
		 */
		break;
	case MIDI_CTL_PITCHBEND:
		snd_opl4_do_for_channel(opl4, chan, snd_opl4_update_pitch);
		break;
	}
}

void snd_opl4_sysex(void *private_data, unsigned char *buf, int len,
		    int parsed, snd_midi_channel_set_t *chset)
{
	opl4_t *opl4 = private_data;

	if (parsed == SNDRV_MIDI_SYSEX_GS_MASTER_VOLUME)
		snd_opl4_do_for_all(opl4, snd_opl4_update_volume);
}