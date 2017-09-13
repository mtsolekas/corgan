# Corgan

Corgan is a simple contacts and schedule organizer written in C
using the Gtk+3 framework.

You can also automatically export all your contacts, in the
form of a vCard version 3, to the apps directory.

## Build

To build corgan you will need the GTK+3 developement libraries.

Run **make** which will build with debugging enabled by default.

Run **make DBG=0** to disable debugging and enable optimizations.

## Install & Run

To install run **make install**, after which you can either run it from
its desktop file entry or from the command line. Currently it only
installs locally and not in the root directory.

To uninstall run **make uninstall**, which will remove all files
created by make install.

## License

Copyright 2017 Marios Tsolekas <marios.tsolekas@gmail.com>.

Corgan is free software under the GPL version 3.
