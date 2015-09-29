/*
 * Copyright 2005 Jens Hoffmann
 * 
 * This file is part of q2dc.
 * 
 * q2dc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * 
 * Foobar is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with Foobar. If not, see
 * http://www.gnu.org/licenses/.
 */

#ifndef __DM2_H
#define __DM2_H

#define	DM2_ORIGINAL_PROTOCOL_VERSION	34
#define	DM2_ENHANCED_PROTOCOL_VERSION	35

#define DM2_MAX_MESSAGE_SIZE 1390

#define DM2_RECORD_CLIENT 0x00000001
#define DM2_RECORD_SERVER 0x00000002
#define DM2_RECORD_RELAY  0x00000080

#define EPS_GUNOFFSET		(1<<0)
#define EPS_GUNANGLES		(1<<1)
#define EPS_PMOVE_VELOCITY2	(1<<2)
#define EPS_PMOVE_ORIGIN2	(1<<3)
#define EPS_VIEWANGLE2		(1<<4)
#define EPS_STATS		(1<<5)

#define MASK_DM2_PACKET_FRAME_BYTE     0x00000010
#define MASK_DM2_PACKET_FRAME_SHORT    0x00020000
#define MASK_DM2_PACKET_FRAME          (MASK_DM2_PACKET_FRAME_BYTE|MASK_DM2_PACKET_FRAME_SHORT)
#define MASK_DM2_PACKET_SKIN_BYTE      0x00010000
#define MASK_DM2_PACKET_SKIN_SHORT     0x02000000
#define MASK_DM2_PACKET_SKIN_LONG      (MASK_DM2_PACKET_SKIN_BYTE|MASK_DM2_PACKET_SKIN_SHORT)
#define MASK_DM2_PACKET_SKIN           (MASK_DM2_PACKET_SKIN_BYTE|MASK_DM2_PACKET_SKIN_SHORT|MASK_DM2_PACKET_SKIN_LONG) 

#define DM2_MAX_STATS	32
#define DM2_MAX_ITEMS	256

#define DM2_TE_GUNSHOT 0
#define DM2_TE_BLOOD 1
#define DM2_TE_BLASTER 2
#define DM2_TE_RAILTRAIL 3
#define DM2_TE_SHOTGUN 4
#define DM2_TE_EXPLOSION1 5
#define DM2_TE_EXPLOSION2 6
#define DM2_TE_ROCKET_EXPLOSION 7
#define DM2_TE_GRENADE_EXPLOSION 8
#define DM2_TE_SPARKS 9
#define DM2_TE_SPLASH 10
#define DM2_TE_BUBBLETRAIL 11
#define DM2_TE_SCREEN_SPARKS 12
#define DM2_TE_SHIELD_SPARKS 13
#define DM2_TE_BULLET_SPARKS 14
#define DM2_TE_LASER_SPARKS 15
#define DM2_TE_PARASITE_ATTACK 16
#define DM2_TE_ROCKET_EXPLOSION_WATER 17
#define DM2_TE_GRENADE_EXPLOSION_WATER 18
#define DM2_TE_MEDIC_CABLE_ATTACK 19
#define DM2_TE_BFG_EXPLOSION 20
#define DM2_TE_BFG_BIGEXPLOSION 21
#define DM2_TE_BOSSTPORT 22
#define DM2_TE_BFG_LASER 23
#define DM2_TE_GRAPPLE_CABLE 24
#define DM2_TE_WELDING_SPARKS 25
#define DM2_TE_PLASMATRAIL 26
#define DM2_TE_GREENBLOOD_new 26
#define DM2_TE_GREENBLOOD_old 27
#define DM2_TE_BLUEHYPERBLASTER 27
#define DM2_TE_PLASMA_EXPLOSION 28
#define DM2_TE_TUNNEL_SPARKS 29
#define DM2_TE_BLASTER2 30
#define DM2_TE_RAILTRAIL2 31
#define DM2_TE_FLAME 32
#define DM2_TE_LIGHTNING 33
#define DM2_TE_DEBUGTRAIL 34
#define DM2_TE_PLAIN_EXPLOSION 35
#define DM2_TE_FLASHLIGHT 36
#define DM2_TE_FORCEWALL 37
#define DM2_TE_HEATBEAM 38
#define DM2_TE_MONSTER_HEATBEAM 39
#define DM2_TE_STEAM 40
#define DM2_TE_BUBBLETRAIL2 41
#define DM2_TE_MOREBLOOD 42
#define DM2_TE_HEATBEAM_SPARKS 43
#define DM2_TE_HEATBEAM_STEAM 44
#define DM2_TE_CHAINFIST_SMOKE 45
#define DM2_TE_ELECTRIC_SPARKS 46
#define DM2_TE_TRACKER_EXPLOSION 47
#define DM2_TE_TELEPORT_EFFECT 48
#define DM2_TE_DBALL_GOAL 49
#define DM2_TE_WIDOWBEAMOUT 50
#define DM2_TE_NUKEBLAST 51
#define DM2_TE_WIDOWSPLASH 52
#define DM2_TE_EXPLOSION1_BIG 53
#define DM2_TE_EXPLOSION1_NP 54
#define DM2_TE_FLECHETTE 55

enum {
	DM2_BAD,
	DM2_MUZZLEFLASH,
	DM2_MUZZLEFLASH2,
	DM2_TEMP_ENTITY,
	DM2_LAYOUT,
	DM2_INVENTORY,
	DM2_NOP,
	DM2_DISCONNECT,
	DM2_RECONNECT,
	DM2_SOUND,
	DM2_PRINT,
	DM2_STUFFTEXT,
	DM2_SERVERDATA,
	DM2_CONFIGSTRING,
	DM2_SPAWNBASELLINE,
	DM2_CENTERPRINT,
	DM2_DOWNLOAD,
	DM2_PLAYERINFO,
	DM2_PACKETENTITIES,
	DM2_DELTAPACKETENTITIES,
	DM2_FRAME,
	DM2_ZPACKET,
	DM2_ZDOWNLOAD
};

typedef struct {
	char name[FILENAME_MAX];
	FILE* handle;
	unsigned long size;
} DM2FILE_T;

int openDM2(DM2FILE_T *f, const char *mode);

void closeDM2(DM2FILE_T *f);

unsigned long initDM2(DM2FILE_T *f);

unsigned long checkDM2(DM2FILE_T *f);

unsigned long processDM2(DM2FILE_T *f_in, DM2FILE_T *f_out);

size_t fwriteBlock(DM2FILE_T *f, BLOCK_T *b);

size_t fwritePartialBlock(DM2FILE_T *f, BLOCK_T *b, unsigned long mark);

int checkBlock(DM2FILE_T *f, BLOCK_T *b, unsigned long mark);

#endif /* __DM2_H */
