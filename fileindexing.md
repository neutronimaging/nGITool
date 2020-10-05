# Advanced file indexing

In an nGI experiment it is rare that there is only a single phase stepping scan. Some scenarios are
- nGI tomography
- repeated images per phase step to improve signal to noise ratio and reject outliers
- wavelength scans
Such experiments are often acquired as a single sequence of images which requires an advanced indexing handling. nGITool supports such indexing schemes by adding the parameters __number of images__, __stride__, and __count__
In a future version it is also foreseen to implement double indexing which is useful for time-of-flight scans that for example store spectra in a folder per phase step.

## The indexing equation
![img](https://latex.codecogs.com/svg.latex?filenumber(i,j,k)=k\cdot{}i\cdot{}(count\cdot{}stride)+j)

- _i_ is the phase step index
- _j_ is the bundle index for repeated images at the same phase step (will soon be implemented)
- _k_ used for multiple phase steping scans to index the phase scans. Used for tomographies and time series.
- _count_ number of images in a step bundle.
- _stride_ the increment to reach the next phase step.

## Basic indexing
The basic indexing is used for single file scan. Here, you can use the parameters step to jump over some phase step images to speed up processing during tuning

![BasicIndexing](figures/basicindexing.svg)

## Tomography
A nGI tomography can be done in two ways:
1. Perform a phase scan before moving to the next angle.
2. Perform a tomography scan before moving the grating to the next phase step.

The _stride_ parameter helps to index the phase steps in the second case and is set to the number of projections in the tomography.
