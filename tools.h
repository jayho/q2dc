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

#ifndef __TOOLS_H
#define __TOOLS_H

/*
#define DEBUG
*/

void die(int errorcode, char *message); 

void warn(char *message);

void debug(char *message);

#endif /* __TOOLS_H */
