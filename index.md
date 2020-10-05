# nGITool

The nGITool is a reduction tool for grating interferometry imaging. It was developed for neutron imaging to reduce the data obtained using the Talbot-Lau interferometer that was implemented at Paul Scherrer Institut [A. Kaestner et al. (2011)](http://dx.doi.org/10.1016/j.nima.2011.08.022). The tool was long time cloased source and also had a node locking protection. Now it is released as open source under the GNU 3.x licence.

The tool is implemeted in C++ and uses [Qt 5.x](https://www.qt.io/) for the GUI

## Table of contents
* [Application overview](overview.md)
* [Method description](method.md)
* [Processing modules]
    * [Estimators](estimators.md)
    * [Pre-processing](preprocessing.md)
* [File indexing schemes](fileindexing.md)

## Download
There is currently not official release of nGITool. 

We do however use a build server that provides builds of the latest development version for download. 
- [Windows](https://jenkins.esss.dk/imaging/job/ImagingSuite_Win10/)
- [MacOS](https://jenkins.esss.dk/imaging/job/ImagingSuite_OSX_10.14/)

Disclaimer: Even though we a making our best efforts to provide as stable code as possible, there is no guarantee of the versions on the build server will work flawlessly.
