# Corgan

Corgan is a simple contacts and schedule organizer written in C
using the Gtk+3 framework.

You can also automatically export all your contacts, in the
form of a vCard version 3, to the apps directory.

## Build Requirements:
- Autoconf
- Automake
- Gettext
- Autopoint
- GTK3 developement headers

## Build Instructions:
- autoreconf -fiv
- ./configure
- make

To enable debugging run **./configure --enable-debug**.

## Install Instructions:
- sudo make install

This will install the corgan binary and its desktop file to their
respective locations. Your contacts and schedule data are saved at
$XDG\_DATA\_HOME/corgan/ which is by default ~/.local/share/corgan/.

## Uninstall Instructions:
- sudo make uninstall

While it will remove the installed binary and desktop file, your
contacts and schedule data will persist, so you must remove them
manually.
