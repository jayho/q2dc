q2dc
====

Quake2 demo converter

Changes Quake2 demos recorded with [R1CHs Enhanced Quake2
Client](http://www.r1ch.net/stuff/r1q2/) to make them editable with keygrip2.

More precisely, it converts *.dm2 files recorded with network protocol 35 to files compatible with the old protocol 34.

Usage
-----
    q2dc [-f] -o output.dm2 input.dm2

(use '-f' to fix MAX_MSGLEN error)

Caveats
-------
* 'msglen > MAX_MSGLEN': you might see this error when you open a r1q2 demo with an alternative client (nocheat,
  original quake).  Try running it with '-f' to fix this (this breaks big blocks in smaller chunks).

* Demos containing the 'new' packet type 'Compressed Packet' cannot be converted yet (according to R1ch this doesn't
  happen very often)

Why
---
Just a fun way to practice a bit of C programming.

Links
-----
* The original [forum entry](http://old.r1ch.net/forum/index.php?action=printpage;topic=581.0)
* Uwe Girlich's [unofficial DM2 format description](http://www.quakewiki.net/archives/demospecs/dm2/)
* [R1CH](http://www.r1ch.net/)'s [protocol 35 description](http://r-1.ch/r1q2-protocol.txt)
* [R1CH](http://www.r1ch.net/)'s Quake2 client [r1q2](http://www.r1ch.net/stuff/r1q2/)
