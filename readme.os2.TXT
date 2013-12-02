The few things I've changed are:

- Auto detection of local IP address if not specified.
- IP and MACHINE parameter have been reversed to allow IP to be left empty
  for the previous change.
- Some English grammar correction <g>.
- Syslog has been disabled due to the fact that I didn't find a syslog.a,
  but you can recompile it if you have one.

To compile on your own, whoop up make.cmd.

The included EXE requires EMX DLLs to run, they are available at
ftp://hobbes.nmsu.edu/pub/os2/dev/emx/v0.9c/emxrt.zip

Thanks to the guys in #os2prog Efnet for my first C "project".. hum.

Samuel Audet <guardia@cam.org>
