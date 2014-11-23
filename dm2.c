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

/*
 * dm2.c
 *
 * See dm2.h for a short description
 */

#include <stdio.h>
#include <sys/stat.h>
#include "common.h"
#include "tools.h"
#include "block.h"
#include "dm2.h"

extern int gFixBlocks;

int openDM2(DM2FILE_T *f, const char *mode) {
	struct stat buf;	

	if ((f->handle = fopen(f->name, mode)) == NULL) return 0;
	if (stat(f->name,&buf) == -1) return 0;
	
	f->size = buf.st_size;
	rewind(f->handle);
	return 1;
}

void closeDM2(DM2FILE_T *f) {
	f->size = 0;
	fclose(f->handle);
}

unsigned long checkDM2(DM2FILE_T *f) {
	unsigned long blocksize;
	unsigned long blocks;
	BYTE buffer[4];
		
	blocks = 0;
	while (ftell(f->handle) < (long)f->size) {
		blocks++;
		if (fread(buffer, 1, 4, f->handle) != 4) return 0;
		blocksize = ((buffer[0]))+((buffer[1])<<8)+((buffer[2])<<16)+((buffer[3])<<24);
		if (blocksize == 0xFFFFFFFF) break;
		if (blocksize > MAX_BSIZE) {
			warn("Blocks of that size not supported");
			return 0;
		}
		if (fseek(f->handle, blocksize, SEEK_CUR) != 0) return 0;
	}
	if (ftell(f->handle) != (long)f->size) return 0;
	rewind(f->handle);
	
	return blocks;
}

unsigned long processDM2(DM2FILE_T *f_in, DM2FILE_T *f_out) {
	unsigned long blocks, oldMessageIndex;
	BYTE buffer[4];
	BLOCK_T b_in, b_out;  
	BYTE cmd, extrabits, surpresscount, extraflags, count, mask_snd, entitytype;
	long mask, deltaframe, entity;
	unsigned long serverframe, offset;
	short size, nextid;
	int i;
	
	/* These are for keeping playerstate */
	long statbits = 0;
	short origin2 = 0, velocity2 = 0, viewangle2 = 0;
        BYTE gunoffset[3] = {0}, gunangles[3] = {0};
	short stats[DM2_MAX_STATS] = {0};	
	
	/* Loop for reading blocks */
	blocks = 0;
  	while (ftell(f_in->handle)<(long)f_in->size) {
		debug(".");
		blocks++;
		
		initBlock(&b_in);
		initBlock(&b_out);
		
		/* Read block size */
		if (fread(buffer, 1, 4, f_in->handle) != 4) return 0;
		b_in.size = ((buffer[0]))+((buffer[1])<<8)+((buffer[2])<<16)+((buffer[3])<<24);
		if (b_in.size == 0xFFFFFFFF) {
			debug("#");
			if (fwrite(buffer, 1, 4, f_out->handle) != 4) return 0;
			break;
		}
		if (b_in.size == 0) {
			debug("X");
			if (fwrite(buffer, 1, 4, f_out->handle) != 4) return 0;	
			continue;
		}
		
		/* Read block */
		if (fread(b_in.data, 1, b_in.size, f_in->handle) != b_in.size) return 0;
		
		/* Loop for reading all messages in the current block */
		while(moreBlockData(&b_in)) {
			cmd = readByte(&b_in);
			if (cmd == 0xFF) {
				warn("cmd is 0xFF");
				break;
			}
			extrabits = cmd & 0xE0;
			cmd &= 0x1F;
			
			oldMessageIndex = b_out.size;
			writeByte(&b_out, cmd);
			
			switch(cmd) {
				case DM2_BAD:
					debug("0");
					warn("Bad command");
					return 0;
					break;
					
				case DM2_MUZZLEFLASH:
					debug("1");
					copyShort(&b_out, &b_in);
					copyByte(&b_out, &b_in);
					
					if (checkBlock(f_out, &b_out, oldMessageIndex) == 0)
						return 0;
					break;
					
				case DM2_MUZZLEFLASH2:
					debug("2");
					copyShort(&b_out, &b_in);
					copyByte(&b_out, &b_in);

					if (checkBlock(f_out, &b_out, oldMessageIndex) == 0)
						return 0;
					break;
					
				case DM2_TEMP_ENTITY: /* This is one ugly mofo */ 
					debug("3");
					entitytype = copyByte(&b_out, &b_in);
					switch (entitytype) {
  						/* version problems */
 						/* case DM2_TE_PLASMATRAIL: */
 						case DM2_TE_GREENBLOOD_new:
    							/* if (SERVERVERSION >= 32) // game version >= 3.15 */
      							goto impact_entity_1;
    							/* else
      								goto line_entity_1; */
  							break;
 						/* case DM2_TE_GREENBLOOD_old: */
  						case DM2_TE_BLUEHYPERBLASTER:
    							/* if (SERVERVERSION >= 32) // game version >= 3.15 */
      							goto line_entity_1;
    							/* else
      								//goto impact_entity_1; */
  							break;
  						/* point entity */
  						case DM2_TE_EXPLOSION1:
 						case DM2_TE_EXPLOSION2:
  						case DM2_TE_ROCKET_EXPLOSION:
  						case DM2_TE_GRENADE_EXPLOSION:
  						case DM2_TE_ROCKET_EXPLOSION_WATER:
  						case DM2_TE_GRENADE_EXPLOSION_WATER:
  						case DM2_TE_BFG_EXPLOSION:
  						case DM2_TE_BFG_BIGEXPLOSION:
  						case DM2_TE_BOSSTPORT:
  						case DM2_TE_PLASMA_EXPLOSION:
  						case DM2_TE_PLAIN_EXPLOSION:
  						case DM2_TE_CHAINFIST_SMOKE:
  						case DM2_TE_TRACKER_EXPLOSION:
  						case DM2_TE_TELEPORT_EFFECT:
  						case DM2_TE_DBALL_GOAL:
  						case DM2_TE_NUKEBLAST:
  						case DM2_TE_WIDOWSPLASH:
  						case DM2_TE_EXPLOSION1_BIG:
  						case DM2_TE_EXPLOSION1_NP:
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
 							break;
  						/* impact entity */
  						case DM2_TE_GUNSHOT:
  						case DM2_TE_BLOOD:
  						case DM2_TE_BLASTER:
  						case DM2_TE_SHOTGUN:
  						case DM2_TE_SPARKS:
  						case DM2_TE_SCREEN_SPARKS:
  						case DM2_TE_SHIELD_SPARKS:
  						case DM2_TE_BULLET_SPARKS:
  						/* case DM2_TE_GREENBLOOD_new: */
  						/* case DM2_TE_GREENBLOOD_old: */
  						case DM2_TE_BLASTER2:
  						case DM2_TE_MOREBLOOD:
  						case DM2_TE_HEATBEAM_SPARKS:
  						case DM2_TE_HEATBEAM_STEAM:
  						case DM2_TE_ELECTRIC_SPARKS:
  						case DM2_TE_FLECHETTE:
impact_entity_1:
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
    							copyByte(&b_out, &b_in);
  						break;
  						/* line entity */
  						case DM2_TE_RAILTRAIL:
  						case DM2_TE_BUBBLETRAIL:
  						case DM2_TE_BFG_LASER:
  						/* case DM2_TE_PLASMATRAIL: */
  						/* case DM2_TE_BLUEHYPERBLASTER: */
  						case DM2_TE_DEBUGTRAIL:
  						case DM2_TE_BUBBLETRAIL2:
line_entity_1:
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
  							break;
  						/* special entity */
  						case DM2_TE_SPLASH:
  						case DM2_TE_LASER_SPARKS:
  						case DM2_TE_WELDING_SPARKS:
  						case DM2_TE_TUNNEL_SPARKS:
    							copyByte(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
   							copyByte(&b_out, &b_in);
    							copyByte(&b_out, &b_in);
  							break;
  						case DM2_TE_PARASITE_ATTACK:
  						case DM2_TE_MEDIC_CABLE_ATTACK:
  						case DM2_TE_HEATBEAM:
  						case DM2_TE_MONSTER_HEATBEAM:
   							copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);;
  							break;  
  						case DM2_TE_GRAPPLE_CABLE:
   							copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
  							break;
  						case DM2_TE_FLAME: /* Quake2 can't parse this! */
    							copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
  							break;
  						case DM2_TE_LIGHTNING:
   							copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
 							break;
  						case DM2_TE_FLASHLIGHT:
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in);
  							break;
  						case DM2_TE_FORCEWALL:
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in);
  							break;
  						case DM2_TE_STEAM:
    							nextid = copyShort(&b_out, &b_in);
    							copyByte(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
    							copyByte(&b_out, &b_in);
    							copyByte(&b_out, &b_in);
    							copyShort(&b_out, &b_in);
    							if (nextid != -1)
      								copyLong(&b_out, &b_in);
  							break;
 						case DM2_TE_WIDOWBEAMOUT:
    							copyShort(&b_out, &b_in);
    							copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in);
  							break;
  						case DM2_TE_RAILTRAIL2: /* senseless, I know */
  							default:
							warn("Unknown entitytype in DM2_TEMP_ENTITY");
    							return 0;
  							break;
					}	

					if (checkBlock(f_out, &b_out, oldMessageIndex) == 0)
						return 0;
					break;
					
				case DM2_LAYOUT:
					debug("4");
					copyString(&b_out, &b_in);

					if (checkBlock(f_out, &b_out, oldMessageIndex) == 0)
						return 0;
					break;
					
				case DM2_INVENTORY:
					debug("5");
					for (i=0; i < DM2_MAX_ITEMS; i++) copyShort(&b_out, &b_in);

					if (checkBlock(f_out, &b_out, oldMessageIndex) == 0)
						return 0;
					break;
				
				case DM2_NOP:
					debug("6");
					break;
					
				case DM2_DISCONNECT:
					debug("7");
					break;
					
				case DM2_RECONNECT:
					debug("8");
					break;
					
				case DM2_SOUND:
					debug("9");
					mask_snd = copyByte(&b_out, &b_in);
					copyByte(&b_out, &b_in);
					if (mask_snd & 0x01) copyByte(&b_out, &b_in);
					if (mask_snd & 0x02) copyByte(&b_out, &b_in);
					if (mask_snd & 0x10) copyByte(&b_out, &b_in);
					if (mask_snd & 0x08) copyShort(&b_out, &b_in);
					if (mask_snd & 0x04) { copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); }

					if (checkBlock(f_out, &b_out, oldMessageIndex) == 0)
						return 0;
					break;
					
				case DM2_PRINT:
					debug("a");
					copyByte(&b_out, &b_in);
					copyString(&b_out, &b_in);

					if (checkBlock(f_out, &b_out, oldMessageIndex) == 0)
						return 0;
					break;
					
				case DM2_STUFFTEXT:
					debug("b");
					copyString(&b_out, &b_in);

					if (checkBlock(f_out, &b_out, oldMessageIndex) == 0)
						return 0;
					break;
					
				case DM2_SERVERDATA:
					debug("c");
					if (readLong(&b_in) != (long)DM2_ENHANCED_PROTOCOL_VERSION) {
						warn("Wrong protocol version (not a r1q2 demo file?)");		
						return 0;
					}
					writeLong(&b_out, (long)DM2_ORIGINAL_PROTOCOL_VERSION);
					copyLong(&b_out, &b_in);
					if (copyByte(&b_out, &b_in) != (BYTE)DM2_RECORD_CLIENT) {
						warn("Wrong record type (only client side records supported)");
						return 0;
					}
					copyString(&b_out, &b_in);
					copyShort(&b_out, &b_in);
					copyString(&b_out, &b_in);
					skipByte(&b_in);
					skipShort(&b_in);

					if (checkBlock(f_out, &b_out, oldMessageIndex) == 0)
						return 0;
					break;
					
				case DM2_CONFIGSTRING:
					debug("d");
					copyShort(&b_out, &b_in);
					copyString(&b_out, &b_in);

					if (checkBlock(f_out, &b_out, oldMessageIndex) == 0)
						return 0;
					break;
					
				case DM2_SPAWNBASELLINE:
					debug("e");
					mask = (long)copyByte(&b_out, &b_in);
					if (mask & 0x00000080) mask |= (((long)(copyByte(&b_out, &b_in))) <<  8);
					if (mask & 0x00008000) mask |= (((long)(copyByte(&b_out, &b_in))) << 16);
       					if (mask & 0x00800000) mask |= (((long)(copyByte(&b_out, &b_in))) << 24);				
					if (mask & 0x00000100) copyShort(&b_out, &b_in); else copyByte(&b_out, &b_in);
				       	if (mask & 0x00000800) copyByte(&b_out, &b_in);
        				if (mask & 0x00100000) copyByte(&b_out, &b_in);
        				if (mask & 0x00200000) copyByte(&b_out, &b_in);
        				if (mask & 0x00400000) copyByte(&b_out, &b_in);
					if (mask & MASK_DM2_PACKET_FRAME_BYTE) copyByte(&b_out, &b_in);
       					if (mask & MASK_DM2_PACKET_FRAME_SHORT) copyShort(&b_out, &b_in);
        				if (mask & MASK_DM2_PACKET_SKIN) {
          					if ((mask & MASK_DM2_PACKET_SKIN_LONG) == MASK_DM2_PACKET_SKIN_LONG)
            						copyLong(&b_out, &b_in);
          					else {
           						if (mask & MASK_DM2_PACKET_SKIN_SHORT) copyShort(&b_out, &b_in);
            						if (mask & MASK_DM2_PACKET_SKIN_BYTE) copyByte(&b_out, &b_in);
          					}
        				}
					if (mask & 0x00004000) {
          					if (mask & 0x00080000) copyLong(&b_out, &b_in);
          					else copyByte(&b_out, &b_in);
        				}
        				else {
          					if (mask & 0x00080000) copyShort(&b_out, &b_in);
        				}
        				if (mask & 0x00001000) {
          					if (mask & 0x00040000) copyLong(&b_out, &b_in);
          					else copyByte(&b_out, &b_in);
        				}
        				else {
          					if (mask & 0x00040000) copyShort(&b_out, &b_in);
        				}
					if (mask & 0x00000001) copyShort(&b_out, &b_in);
        				if (mask & 0x00000002) copyShort(&b_out, &b_in);
        				if (mask & 0x00000200) copyShort(&b_out, &b_in);
        				if (mask & 0x00000400) copyByte(&b_out, &b_in);
        				if (mask & 0x00000004) copyByte(&b_out, &b_in);
        				if (mask & 0x00000008) copyByte(&b_out, &b_in);
       					if (mask & 0x01000000) { copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); }
        				if (mask & 0x04000000) copyByte(&b_out, &b_in);
        				if (mask & 0x00000020) copyByte(&b_out, &b_in);
        				if (mask & 0x08000000) copyShort(&b_out, &b_in);

					if (checkBlock(f_out, &b_out, oldMessageIndex) == 0)
						return 0;
					break;
				
				case DM2_CENTERPRINT:
					debug("f");
					copyString(&b_out, &b_in);

					if (checkBlock(f_out, &b_out, oldMessageIndex) == 0)
						return 0;
					break;
				
				case DM2_DOWNLOAD:
					debug("g");
					size = copyShort(&b_out, &b_in);
					copyByte(&b_out, &b_in);
					if (size > 0)
						copyBytes(&b_out, &b_in, size); 

					if (checkBlock(f_out, &b_out, oldMessageIndex) == 0)
						return 0;
					break;
					
				case DM2_PLAYERINFO: /* Done in DM2_Frame */
					debug("h");
					warn("DM2_PLAYERINFO not after DM2_FRAME...oops");
					return 0;
					break;
					
				case DM2_PACKETENTITIES: /* Done in DM2_Frame */
					debug("i");
					warn("DM2_PACKETENTITIES not after DM2_FRAME...oops");
					return 0;
					break;
				/*	
				case DM2_DELTAPACKETENTITIES:
					debug("cmd=%#x\n", cmd);
					break;
				*/	
				case DM2_FRAME:
					debug("k");
					serverframe = (unsigned long)readLong(&b_in);
					offset = serverframe & 0xF8000000;
					offset >>= 27;
					serverframe &= 0x07FFFFFF;
					if (offset == 31)
						deltaframe = -1; /*0xFFFFFFFF;*/
					else
						deltaframe = serverframe - offset;
					writeLong(&b_out, serverframe);
					writeLong(&b_out, deltaframe);

					surpresscount = readByte(&b_in);
					writeByte(&b_out, surpresscount & 0x0F);
					extraflags = extrabits >> 1;
					extraflags |= (surpresscount & 0xF0) >> 4;					
					
					count = copyByte(&b_out, &b_in);
					copyBytes(&b_out, &b_in, count);
						
					/* Playerstate */
					debug("h");
					writeByte(&b_out, (BYTE)DM2_PLAYERINFO);
					mask = (long)copyShort(&b_out, &b_in);
					if (mask & 0x0001) copyByte(&b_out, &b_in);
					if (mask & 0x0002) { /* Origin */
						copyShort(&b_out, &b_in);
						copyShort(&b_out, &b_in);
					}
					if (extraflags & EPS_PMOVE_ORIGIN2) origin2 = readShort(&b_in);
					if (mask & 0x0002) writeShort(&b_out, origin2);
					
					if (mask & 0x0004) { /* Velocity */
						copyShort(&b_out, &b_in);
						copyShort(&b_out, &b_in);
					}
					if (extraflags & EPS_PMOVE_VELOCITY2) velocity2 = readShort(&b_in);
					if (mask & 0x0004) writeShort(&b_out, velocity2);
					if (mask & 0x0008) copyByte(&b_out, &b_in);
					if (mask & 0x0010) copyByte(&b_out, &b_in);
					if (mask & 0x0020) copyShort(&b_out, &b_in);
					if (mask & 0x0040) { copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); }
					if (mask & 0x0080) { copyByte(&b_out, &b_in); copyByte(&b_out, &b_in); copyByte(&b_out, &b_in); }
				       	if (mask & 0x0100) { /* ViewAngles */
						copyShort(&b_out, &b_in);
						copyShort(&b_out, &b_in);
					}
					if (extraflags & EPS_VIEWANGLE2) viewangle2 = readShort(&b_in);
					if (mask & 0x0100) writeShort(&b_out, viewangle2);
					if (mask & 0x0200) { copyByte(&b_out, &b_in); copyByte(&b_out, &b_in); copyByte(&b_out, &b_in); }
					if (mask & 0x1000) copyByte(&b_out, &b_in);
					if (mask & 0x2000) { /* Weaponframe */
						copyByte(&b_out, &b_in);
					}
					if (extraflags & EPS_GUNOFFSET)
						for (i=0; i < 3; i++) gunoffset[i] = readByte(&b_in);
					if (extraflags & EPS_GUNANGLES)
						for (i=0; i < 3; i++) gunangles[i] = readByte(&b_in);
					if (mask & 0x2000) {
						for (i=0; i < 3; i++) writeByte(&b_out, gunoffset[i]);
						for (i=0; i < 3; i++) writeByte(&b_out, gunangles[i]);
					}
					if (mask & 0x0400) { 
						copyByte(&b_out, &b_in); copyByte(&b_out, &b_in); 
						copyByte(&b_out, &b_in); copyByte(&b_out, &b_in); 
					}
					if (mask & 0x0800) copyByte(&b_out, &b_in);
					if (mask & 0x4000) copyByte(&b_out, &b_in);
					if (mask & 0x8000) skipShort(&b_in); /* from client source (not mentioned in r1q2-protocol.txt) */
					if (extraflags & EPS_STATS) {
						statbits = readLong(&b_in);
						if (statbits) {
							for (i=0; i < DM2_MAX_STATS; i++)
								if (statbits & (0x00000001<<i)) stats[i] = readShort(&b_in);
						}	
					}
					writeLong(&b_out, statbits);
					if (statbits) {
						for (i=0; i < DM2_MAX_STATS; i++)
							if (statbits & (0x00000001<<i)) writeShort(&b_out, stats[i]);
					}
					
					/* PacketEntities */
					debug("i");
					writeByte(&b_out, (BYTE)DM2_PACKETENTITIES);
					for(;;) {
						mask = (long)copyByte(&b_out, &b_in);
						if (mask & 0x00000080) mask |= (((long)(copyByte(&b_out, &b_in))) <<  8);
						if (mask & 0x00008000) mask |= (((long)(copyByte(&b_out, &b_in))) << 16);
       						if (mask & 0x00800000) mask |= (((long)(copyByte(&b_out, &b_in))) << 24);
				
						entity = (mask & 0x00000100) ? (long)(copyShort(&b_out, &b_in)) : (long)(copyByte(&b_out, &b_in));
          					if (entity == 0) break;

						if (mask & 0x00000800) copyByte(&b_out, &b_in);
          					if (mask & 0x00100000) copyByte(&b_out, &b_in);
          					if (mask & 0x00200000) copyByte(&b_out, &b_in);
          					if (mask & 0x00400000) copyByte(&b_out, &b_in);
						if (mask & MASK_DM2_PACKET_FRAME_BYTE) copyByte(&b_out, &b_in);
          					if (mask & MASK_DM2_PACKET_FRAME_SHORT) copyShort(&b_out, &b_in);
          					if (mask & MASK_DM2_PACKET_SKIN) {
            						if ((mask & MASK_DM2_PACKET_SKIN_LONG) == MASK_DM2_PACKET_SKIN_LONG)
              							copyLong(&b_out, &b_in);
            						else {
              							if (mask & MASK_DM2_PACKET_SKIN_SHORT) copyShort(&b_out, &b_in);
              							if (mask & MASK_DM2_PACKET_SKIN_BYTE) copyByte(&b_out, &b_in);
            						}
          					}
          					if (mask & 0x00004000) {
            						if (mask & 0x00080000) 	copyLong(&b_out, &b_in);
            						else copyByte(&b_out, &b_in);
          					}
          					else {
            						if (mask & 0x00080000) copyShort(&b_out, &b_in);
          					}
          					if (mask & 0x00001000) {
            						if (mask & 0x00040000) 	copyLong(&b_out, &b_in);
           						else copyByte(&b_out, &b_in);
          					}
         					else {
            						if (mask & 0x00040000)  copyShort(&b_out, &b_in);
          					}
          					if (mask & 0x00000001) copyShort(&b_out, &b_in);
          					if (mask & 0x00000002) copyShort(&b_out, &b_in);
          					if (mask & 0x00000200) copyShort(&b_out, &b_in);
          					if (mask & 0x00000400) copyByte(&b_out, &b_in);
          					if (mask & 0x00000004) copyByte(&b_out, &b_in);
          					if (mask & 0x00000008) copyByte(&b_out, &b_in);
          					if (mask & 0x01000000) { copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); copyShort(&b_out, &b_in); }
          					if (mask & 0x04000000) copyByte(&b_out, &b_in);
          					if (mask & 0x00000020) copyByte(&b_out, &b_in);
          					if (mask & 0x08000000) copyShort(&b_out, &b_in);
					}

					if (checkBlock(f_out, &b_out, oldMessageIndex) == 0)
						return 0;
					break;
				
				case DM2_ZPACKET:
					debug("j");
					warn("\nCompressed packets not yet implemented");
					return 0;
					break;

				case DM2_ZDOWNLOAD:
					debug("k");
					warn("\nCompressed downloads not yet implemented");
					return 0;
					break;
					
				default:
					warn("Unrecognized command");
					return 0;	
			} /* switch (cmd) */
		} /* while (messages) */
		if (fwriteBlock(f_out, &b_out) != b_out.size) return 0;
	} /* while (blocks) */
	
	if (ftell(f_in->handle) != (long) f_in->size) return 0;
	debug("\n");
	
	return blocks;	
}

size_t fwriteBlock(DM2FILE_T *f, BLOCK_T *b) {
	/*size_t written = 0;
	BYTE *p = (BYTE*)&b->size;

	written += fwrite(p  , 1, 1, f->handle);
	written += fwrite(p+1, 1, 1, f->handle);
	written += fwrite(p+2, 1, 1, f->handle);
	written += fwrite(p+3, 1, 1, f->handle);
	written += fwrite(b->data, 1, b->size, f->handle);

	return written-4;
	*/
	return fwritePartialBlock(f, b, b->size);
}

size_t fwritePartialBlock(DM2FILE_T *f, BLOCK_T *b, unsigned long mark) {
	size_t written = 0;
	BYTE *p = (BYTE*)&mark;
	
	written += fwrite(p  , 1, 1, f->handle);
	written += fwrite(p+1, 1, 1, f->handle);
	written += fwrite(p+2, 1, 1, f->handle);
	written += fwrite(p+3, 1, 1, f->handle);
	written += fwrite(b->data, 1, mark, f->handle);
	
	return written-4;
}

int checkBlock(DM2FILE_T *f, BLOCK_T *b, unsigned long mark) {
	if (gFixBlocks == 0)
		return 1;
	
	if (b->size <= DM2_MAX_MESSAGE_SIZE)
		return 1;

	if (fwritePartialBlock(f, b, mark) == mark) {
		shiftBlockToLeft(b, mark);
		return 1;	
	}
	return 0;
}

/* EOF */
