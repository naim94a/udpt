.. UDPT documentation master file, created by
   sphinx-quickstart on Mon Oct  2 02:02:26 2017.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

UDPT Documentation
==================

.. toctree::
    :maxdepth: 2
    :caption: Contents:
    :numbered:
    :hidden:

    building
    udpt.conf
    restapi.rst

UDPT is a lightweight UDP torrent tracker, it implements BEP15_ of the BitTorrent protocol.
This project was developed with simplicity and security in mind.

This project is being developed by `@naim94a`_. UDPT's development started November 20th, 2012.

Source code can be found here: https://github.com/naim94a/udpt

Features
--------
* UDP protocol based tracker
* :doc:`Simple INI configuration file <udpt.conf>`
* :doc:`HTTP REST API <restapi>`
* In Memory volatile database
* Logging
* Linux daemon / Windows Service
* Choice of *static* or *dynamic* tracker modes

.. _BEP15: http://www.bittorrent.org/beps/bep_0015.html
.. _@naim94a: https://github.com/naim94a/
