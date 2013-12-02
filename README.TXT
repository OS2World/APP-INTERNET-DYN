DYN 1.0.4                                  Jon Klippenstein <random@ocii.com>
-----------------------------------------------------------------------------

NOTE: I haven't gotten around to updating this file much, but there have 
been one or two new things in this latest version, such as erasing the 
commandline (as shown by ps) so no one can see your MID/seccodes.

Overview
--------

DYN is a simple client program for the Monolith Coalition's DYNDNS 
service.  It creates a connection to the Monolith server and sends the 
new information for your dynamic connection so that the DNS files can be 
updated.  DYN is written on a Linux machine, but should be portable to 
all decent UNIXs.  It has been tested on Linux 2.0.27 and SunOS 5.5.

Usage
-----

The easiest way to use DYN is to run it from an ip-up script.  This 
script, usually located in /etc/ppp/ip-up is run every time PPP 
establishes a connection with your Internet Service Provider.  It runs 
the script with the following arguments: interface device speed 
local_addr remote_addr.  To run DYN every time you connect, add the 
following line to your /etc/ppp/ip-up script, filling the appropriate fields:

(sleep 5 ; /path/to/dyn MID SEC1 SEC2 SEC3 $4 MACHINE) &

Where MID is your Monolith ID, SEC{1-3} are your security codes, $4 is 
replaced by the shell with your dynamic IP address and MACHINE is your 
dyndns hostname, machine.dyn.ml.org or whatever your chose.  THe sleep 5 
part is to give the pppd daemon enough time to start up the connection 
and be ready to accept outgoing packets.  I was having trouble with it 
not being able to connect to the local nameserver, but the sleep seems to 
fix it.

I don't know how to get DYN to run automatically when you use DIP to 
establish a connection, as I've never used it. :)  One note, if you are 
using the diald program to automatically dial your link, you might have 
to put the DYN command in diald's ip-up file, not pppd's.  Check the 
diald manpage for more info.

Bugs
----

Hopefully none. :)  The only thing I can see going wrong is if you don't 
actually *have* a connection, or the connection isn't up, so it won't be 
able to lookup the server.  However, running from pppd solves both these 
problems.

Installing
----------

Simple.  Type 'make dyn' to make the DYN program, and then 'make install' 
to install the file to /usr/local/bin.  By default, DYN is installed rwx 
root only.  This is to prevent pesky users from changing your DNS entry 
:)  If you don't like this, just change it.  

Configuring
-----------

None really.  DYN automatically gets the OS and Machine type via the 
UNIX uname system call.  If you want to find out what it will send, type 
uname -m -s and it will print the values it uses.

Credits
-------

Lets see, I wrote all of the program, and I used some networking code 
from Artur Skawina <skawina@usa.net>, but it basic UNIX network code that 
I didn't know how to write correctly :)  The fetchwww.c file is included 
in this package.

To contact me, to talk about DYN, ML's dyndns, or just to say hi, send 
email to:

random@ocii.com

That's all!  Enjoy DYN, I hope it makes life easier :)

--
Jon Klippenstein             | email: random@ocii.com
Gold Network Services Canada |        root@dparrot.bohica.net
                             |        
Student  Coder  Composer     | uucp:  alberta!ve6kik!dparrot!root
