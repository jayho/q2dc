q2dc
====

Quake2 demo converter (Jens Hoffmann, 2005)

This utility converts Quake2 demo files (.dm2) recorded with [R1CHs Enhanced Quake2
Client](http://www.r1ch.net/stuff/r1q2/) to standard Quake2 demo files. More precisely, it converts demos recorded with
the new network protocol 35 to ones compatible with the old protocol 34.

Converted demos are usable by clients that do not support protocol 35. In particular, they are editable with keygrip2.

Usage
-----
    q2dc [-f] -o output.dm2 demo.dm2

(use '-f' to fix MAX_MSGLEN error)

Caveats
-------
* 'msglen > MAX_MSGLEN': you might see this error when you open a r1q2 demo with an alternative client (nocheat,
  original quake).  Try running it with '-f' to fix this (this breaks big blocks in smaller chunks).

* Demos containing the 'new' packet type 'Compressed Packet' cannot be converted yet (according to R1ch this doesn't
  happen very often)

Why
---
It was a fun way to understand a technical spec (DM2 format) and practice a bit of C programming.

Bugs
----
Thanks to R1ch for pointing out a signed/unsigned error that lead to some strange playback behaviour of the converted
file.

Links
-----
* The original [forum entry](http://old.r1ch.net/forum/index.php?action=printpage;topic=581.0)
* Uwe Girlich's [unofficial DM2 format description](http://www.quakewiki.net/archives/demospecs/dm2/) (excellent work!)
* [R1CH](http://www.r1ch.net/)'s [protocol 35 description](http://r-1.ch/r1q2-protocol.txt)
* [R1CH](http://www.r1ch.net/)'s Quake2 client [r1q2](http://www.r1ch.net/stuff/r1q2/)
