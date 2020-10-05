# Advanced file indexing

In an nGI experiment it is rare that there is only a single phase stepping scan. Some scenarios are
- nGI tomography
- repeated images per phase step to improve signal to noise ratio and reject outliers
- wavelength scans
Such experiments are often acquired as a single sequence of images which requires an advanced indexing handling. nGITool supports such indexing schemes by adding the parameters __number of images__, __stride__, and __count__
In a future version it is also foreseen to implement double indexing which is useful for time-of-flight scans that for example store spectra in a folder per phase step.

## The indexing equation


## Basic indexing
The basic indexing is used for single file scan. Here, you can use the parameters step to jump over some phase step images to speed up 
![BasicIndexing](figures/basicindexing.svg)
