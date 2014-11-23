/*
 q2dc -- Version 0.1
 Quake2 r1q2 demo converter 
 Copyright (C) 2005 Jens Hoffmann


 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

 Jens Hoffmann (hanf, pubic gnats)
 Germany
 jens@spackfisch.de
*/


A little update in advance
--------------------------
Kudos to R1ch for pointing out a signed/unsigned error that 
lead to some strange playback behaviour of the converted
file.


What it does
------------
This ultility converts Quake2 demo files recorded with
R1CHs Enhanced Quake2 Client (http://www.r1ch.net/stuff/r1q2/)
to standard Quake2 demo files.

It converts demos recorded with the new network protocol 35 to
ones compatible with the old protocol (34). With the additional
option '-f' it breaks big blocks in smaller chunks.


What that means
---------------
Converted demos (even without '-f') are now editable with
keygrip2.

The additional '-f' option should fix the 'msglen > MAX_MSGLEN'
error you get when you open a r1q2 demo with an alternative client
(nocheat, original quake).


Missing features
----------------
Demos containing the 'new' packet type 'Compressed Packet' cannot 
be converted yet. According to R1ch this doesn't happen very often,
but if there are problems with this it can be implemented without
too much hassle.


How
----
I used Uwe Girlich's DM2 format description and R1ch's protocol 35 
description to translate back the new protocol features.


Credits
-------
- Uwe Girlich for the excellent unofficial DM2 format description, 
  his 'Little Movie Processing Centre' (LMPC) inclusive source code
- R1CH for his excellent client (http://www.r1ch.net/) inclusive 
  source code

