# How the reduction works
In grating interferometry a grating is stepwise translated perpendicular to the grating lines. 
This produces a sinusoidal intensity oscillation for each pixel. The intensity variations can be described by 
![img](https://latex.codecogs.com/svg.latex?f(x,y,i)=B+A\sin\left(2\pi\frac{M}{N}i-\phi\right)+w(x,y,i)\quad{}i=0,1,\ldots,N)
where _M_ is the number of periods.
![Sines](figures/sine_curves.svg)

## The nGI equations
The reduced images are computed using estimates of _A_, _B_, and ![img](https://latex.codecogs.com/svg.latex?\phi) from sample and reference images
- Transmission images: ![img](https://latex.codecogs.com/svg.latex?TI=\frac{B}{B_0})
- Differential phase images: ![img](https://latex.codecogs.com/svg.latex?DPC=\phi-\phi_0)
- Dark field images: ![img](https://latex.codecogs.com/svg.latex?DFI=\frac{A/B}{A_0/B_0}=\frac{A\cdot{}B_0}{B\cdot{}A_0})
