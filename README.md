# Corgan

Corgan is a simple contacts and schedule organizer written in C
using the Gtk+3 framework.

You can also automatically export all your contacts, in the
form of a vCard version 3, to the apps directory.

## Build Requirements:
- Autoconf
- Automake
- GTK3 developement headers

## Build Instructions:
- autoreconf -i
- ./configure
- make

To enable debugging run **./configure --enable-debug=yes**.

## Install/Uninstall Instructions:
- sudo make install
- sudo make uninstall

After installing your contacts and schedule files will be saved
to ~/.local/share/corgan/ and will have to removed manually after
uninstalling.

## License

Copyright 2017 Marios Tsolekas <marios.tsolekas@gmail.com>.

Corgan is free software under the GPL version 3.
