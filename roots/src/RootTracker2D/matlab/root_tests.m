%% Load images

a=imread('~/Dropbox/Andres/test_s5_01_Original.tif');
b=imread('~/Dropbox/Andres/test_s5_02_MFR_filtered.tif');
c=imread('~/Dropbox/Andres/test_s5_03_Thresholded_MFR.tif');

figure(1)
subplot(1,3,1),imagesc(a), axis image;
subplot(1,3,2),imagesc(b), axis image;
subplot(1,3,3),imagesc(c), axis image;


%% 
figure(2)

ax(1)=subplot(2,2,1);
sk=bwmorph(c,'thin',inf);
imagesc(sk)

K=[1 2 4 ; 128 0 8; 64 32 16];
csk=sk.*filter2(K,sk);
ax(2)=subplot(2,2,2);imagesc(csk);

skid=countbitblocks(csk,8);
s=reshape(skid,size(sk));
ax(3)=subplot(2,2,3);imagesc(s);


crosses=imdilate(2<s,ones(3));

sk2=sk; sk2(crosses)=0;

ax(4)=subplot(2,2,4); imagesc(sk2)

linkaxes(ax)

%%
figure(3)

lsk=bwlabel(sk2,8);
ax3(1)=subplot(2,2,1); imagesc(lsk);

seg.sumorig(max(lsk(:)))=0;
seg.summfr(max(lsk(:)))=0;
seg.cnt(max(lsk(:)))=0;

for i=1:prod(size(lsk))
    lbl=lsk(i);
    if (0<lbl)
        seg.sumorig(lbl)=seg.sumorig(lbl)+a(i);
        seg.summfr(lbl)=seg.summfr(lbl)+b(i);
        seg.cnt(lbl)=seg.cnt(lbl)+1;
    end
end

subplot(2,2,3),hist(seg.summfr./seg.cnt,100)
subplot(2,2,4),hist(seg.sumorig./seg.cnt,100)

%% 
figure(4)

[dx,dy]=gradient(a);

%imagesc(.*sk2);
mask=imdilate(sk2,ones(7));
ax4(1)=subplot(2,2,1),imagesc(medfilt2(atan2(dx,dy),[3 3]).*mask);
ax4(2)=subplot(2,2,2),imagesc(medfilt2(a,[3 3]).*mask);
ax4(3)=subplot(2,2,3),imagesc(medfilt2(b,[3 3]).*mask);
subplot(2,2,4), hist(b(mask==1),256)
linkaxes(ax4)
