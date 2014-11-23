q2dc
====

Quake2 r1q2 demo converter (Jens Hoffmann, 2005)

This utility converts Quake2 demo files (.dm2) recorded with [R1CHs Enhanced Quake2
Client](http://www.r1ch.net/stuff/r1q2/) to standard Quake2 demo files. More precisely, it converts demos recorded with
the new network protocol 35 to ones compatible with the old protocol 34.

Converted demos are editable with keygrip2. See [this
link](http://old.r1ch.net/forum/index.php?action=printpage;topic=581.0) for further info.

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

Bugs
----
Thanks to R1ch for pointing out a signed/unsigned error that lead to some strange playback behaviour of the converted
file.

Thanks
------
I used Uwe Girlich's DM2 format description and R1ch's protocol 35 description to translate back the new protocol
features. See also
* Uwe Girlich's excellent [unofficial DM2 format description](http://www.quakewiki.net/archives/demospecs/dm2/)
* [R1CH's](http://www.r1ch.net/) Quake2 client [r1q2](http://www.r1ch.net/stuff/r1q2/)

