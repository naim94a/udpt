### The UDPT Project
UDPT is an open source torrent tracker that implements [BEP 15](http://www.bittorrent.org/beps/bep_0015.html) of the BitTorrent protocol, written in C/C++. It works with Windows and on Unix-based operating systems.
Since it runs over the UDP protocol unlike most trackers, it has the benefit of using less bandwidth.

### What is a "Torrent Tracker"?
When downloading files using a torrent client, the client must find peers to download parts of the file(s) from. The client asks for peers from a server (the "tracker"). The clients tell the tracker about the torrent that it is downloading or uploading and it's progress. The tracker then responds to clients with IPs of others peers downloading the same file(s), allowing peers to download/upload files.

### Features
* UDP tracking protocol
* SQLite3 database, it support In-memory (using the `:memory:` file)
* Dynamic Tracker
* HTTP API for static trackers
* Logging
* Windows Service / Unix daemon support
* Simple ini-like configuration file (which can be empty to load defaults)
For more info about UDPT, check the [Wiki](https://github.com/naim94a/udpt/wiki).

#### License & third party libraries
UDPT is released under the GPLv3 license.
Third partly libraries include:
- SQLite3, which is public-domain; 
- Boost which is released under the [Boost License](http://www.boost.org/LICENSE_1_0.txt).

#### Issues & feature requests
Please submit bugs & feature requests at UDPT's GitHub issues page: https://github.com/naim94a/udpt/issues.

#### Authors/Contributing
On November 20th, 2012 the development started by Naim A. (@naim94a). If you would like to contribute, you can submit a pull-request on GitHub and it may get to be part of the UDPT project (After passing a good CR).