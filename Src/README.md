# RegKey 3.2 for MS-DOS

I have added Makefiles to support building RegKey under Visual C++ 1.52 (Microsoft C++ 8 for DOS) and optionally a library usable under Visual Basic for DOS.  These files assume the availability of these tools on
your PATH, and have not been tested under other DOS compilers.

To build the C library (regkey.lib): nmake -f msc8c.lib all

To build the VB library (regkeyvb.lib): nmake -f msc8vb.lib all

This code has (with ChatGPT's assistance) been updated to use secure functions (fopen_s etc.) when building with newer MS compilers.  This has been tested under Visual Studio 2022 (v17.14.9, July 2025).

The output of this code under MS-DOS and Windows (32- and 64-bit) has
been tested and found to match.
