%% Load images

a=imread('~/Dropbox/Andres/tests_01_Original.tif');
b=imread('~/Dropbox/Andres/tests_02_MFR_filtered.tif');
c=imread('~/Dropbox/Andres/tests_03_Thresholded_MFR.tif');

%% 

D=1/16*[-3 -10 -3; 0 0 0; 3 10 3];
img=medfilt2(b,[5 5]);
Dx=filter2(D,img);
Dy=filter2(D',img);

ax=subplot(2,2,1);
imagesc(log(Dx.*Dx)), axis image,

ax(2)=subplot(2,2,2);
imagesc(Dx.*Dy), axis image, caxis(5e6*[-1 1])

ax(3)=subplot(2,2,4);
imagesc(log(Dy.*Dy)), axis image

linkaxes(ax)
