// ***************************************************************************
//    Copyright (C) 2010 by Anders Kaestner   
//    anders.kaestner@psi.ch   
// ***************************************************************************

#include <vector>
#include <deque>
#include <cstdlib>
#include <sstream>

#include <base/timage.h>
#include <filters/filter.h>
#include <filters/medianfilter.h>
#include <morphology/label.h>
#include <io/io_matlab.h>
#include <io/io_tiff.h>
#include <morphology/morphdist.h>
#include <morphology/label.h>
#include <math/mathconstants.h>

#include <morphology/skeleton.h>
#include <base/thistogram.h>
#include "RootTrackConfig.h"
#include "rootfinder.h"

using namespace std;
using namespace kipl::base;
using namespace kipl::filters;
using namespace kipl::io;
using namespace kipl::morphology;
//using namespace ImageThreshold;
using namespace kipl::visualization;


RootFinder::RootFinder() :
    logger("RootFinder")
{}

int RootFinder::Process(TImage<float,2> &img, TImage<float,2> &res, RootTrackConfig &pars)
{
    m_ImageList.clear();
    std::ostringstream msg;
	size_t const * const dims=img.Dims();
    if (pars.mProcessingSettings.grow_thresmin<pars.mProcessingSettings.thres_low) {
        msg<<"grow threshold< min threshold";
        logger(kipl::logging::Logger::LogError,msg.str());
        throw kipl::base::KiplException(msg.str(),__FILE__,__LINE__);
	}
    if (pars.mProcessingSettings.thres_quantile<pars.mProcessingSettings.grow_thresmin) {
        msg.str("");
        msg<<"mfr threshold < grow threshold";
        logger(kipl::logging::Logger::LogError,msg.str());
        throw kipl::base::KiplException(msg.str(),__FILE__,__LINE__);
	}

    m_ImageList["01 Original"]=img;
    TImage<float,2> m_ImgMFR;
    mfr(img,m_ImgMFR,pars.mProcessingSettings.mfr_sigma,
        pars.mProcessingSettings.mfr_ratio,
        pars.mProcessingSettings.mfr_omega,
        pars.mProcessingSettings.mfr_dirs);
    m_ImageList["02 MFR filtered"]=m_ImgMFR;
    m_ImageList["02 MFR filtered"].Clone();
	string filename;
	string varname;
    if (pars.mProcessingSettings.save_steps) { // Save mfr image
        varname=pars.mImageInformation.basename+"_mfr";
        filename=pars.mImageInformation.path+varname+".tif";
        WriteTIFF32(m_ImgMFR,filename.c_str());
	}

    logger(kipl::logging::Logger::LogMessage,"Identifying thresholds");
    vector<pair<float,float> > hist;

    float fMin=*min_element(m_ImgMFR.GetDataPtr(),m_ImgMFR.GetDataPtr()+m_ImgMFR.Size());
    float fMax=*max_element(m_ImgMFR.GetDataPtr(),m_ImgMFR.GetDataPtr()+m_ImgMFR.Size());
    kipl::base::THistogram<float> histogram(pars.mProcessingSettings.m_nHistogramSize,fMin,fMax);
    hist=histogram.cumHist(m_ImgMFR,true);

    vector<pair<float,float> >::iterator hit;
    float th=0.0f;
    float th_low=0.0f;
    float th_grow=0.0f;

    ofstream f("/Users/kaestner/hist.csv");

    for (hit=hist.begin(); hit!=hist.end(); hit++) {
        f<<hit->first<<", "<<hit->second<<endl;
    }

    for (hit=hist.begin(); hit!=hist.end(); hit++) {
        float value=hit->second;
        cout<<setw(8)<<(hit->first)<<" "<<setw(8)<<value<<endl;

        if (pars.mProcessingSettings.thres_low<value)
            th_low=hit->first;

        if (pars.mProcessingSettings.grow_thresmin<value)
            th_grow=hit->first;

        if (pars.mProcessingSettings.thres_quantile<value) {
            th=hit->first;
            break;
        }
    }


    msg<<"Thresholds the MFR image with: "<<th<<" ("<<pars.mProcessingSettings.thres_quantile*100<<"\%), "
      <<th_low<<" ("<<pars.mProcessingSettings.thres_low*100<<"\%)"<<endl;

    logger(kipl::logging::Logger::LogMessage,msg.str());

    TImage<float,2> m_ImgThresMFR(dims);
	for (size_t i=0; i<img.Size(); i++) {
        m_ImgThresMFR[i]=m_ImgMFR[i]>th;
        if (m_ImgMFR[i]<th_low) m_ImgMFR[i]=th_low;
	}
	
    if (0<pars.mProcessingSettings.thres_clean)
        CleanThresholded(m_ImgThresMFR,pars.mProcessingSettings.thres_clean);

    if (pars.mProcessingSettings.save_steps) { // Save thresholded
        varname=pars.mImageInformation.basename+"_th";
        filename=pars.mImageInformation.path+varname+pars.mImageInformation.extensions.back();
        WriteTIFF(m_ImgThresMFR,filename.c_str());
    }

    m_ImageList["03 Thresholded MFR"]=m_ImgThresMFR;
    kipl::base::TImage<float,2> m_ImgRoot;

    if (pars.mProcessingSettings.m_bTracking) {
        logger(kipl::logging::Logger::LogMessage,"Computing skeleton");
		deque<int> skel;
        kipl::base::TImage<float,2> m_ImgSkel;
        kipl::morphology::thinning(m_ImgThresMFR, m_ImgSkel,skel);
		
        if (pars.mProcessingSettings.save_steps) { // Save skeleton
            varname=pars.mImageInformation.basename+"_sk";
            filename=pars.mImageInformation.path+varname+".tif";
            WriteTIFF(m_ImgSkel,filename.c_str());
		}

        m_ImageList["04 Skeleton"]=m_ImgSkel;
		deque<int> lines, crossings,endpoints;
        logger(kipl::logging::Logger::LogMessage,"Removing crossings: Skeleton analysis");
        kipl::morphology::SkeletonAnalysis(res,skel, lines, crossings, endpoints);
        logger(kipl::logging::Logger::LogMessage,"Eroding crossings from skeleton");
		CNeighborhood ng(dims,2,conn4);
		int pos,p;
		while (!crossings.empty()) {
			pos=crossings.front();
			crossings.pop_front();
			for (size_t i=0; i<ng.N(); i++)
				if ((p=ng.neighbor(pos,i))!=-1)
					res[p]=0;
		}

        logger(kipl::logging::Logger::LogMessage,"Removing small segments: Labeling");
		TImage<int,2> labels;
        int cnt=kipl::morphology::LabelImage(res, labels,conn8);

        vector<pair<size_t,size_t> > areas;
        LabelArea(labels,cnt,areas);

        deque<int> labels2remove;
        for (size_t i=0; i<areas.size(); i++) {
            if (areas[i].second<pars.mProcessingSettings.m_nSmallItem)
				labels2remove.push_back(i);
        }

        msg.str("");
        msg<<"Found "<<labels2remove.size()<<" items to remove"<<endl;

        if (pars.mProcessingSettings.save_steps) { // Save labelled skeleton
            varname=pars.mImageInformation.basename+"_lsk";
            filename=pars.mImageInformation.path+varname+pars.mImageInformation.extensions.back();
			WriteTIFF(labels,filename.c_str());
		}

		deque<int>::iterator it,it2;
		deque<int> skel_tmp;

		skel_tmp.clear();
		for (it2=skel.begin(); it2!=skel.end(); it2++) {
			int pos=*it2;
			int lbl=-1;
			for (it=labels2remove.begin(); it!=labels2remove.end(); it++) {
				lbl=*it;
				if (labels[pos]==lbl)
					break;
			}
			if (labels[pos]==lbl) 
				res[pos]=0;
			else
				skel_tmp.push_back(pos);
			
		}

		skel.clear();
		copy(skel_tmp.begin(),skel_tmp.end(),back_inserter(skel));

        if (pars.mProcessingSettings.save_steps) { // Save cleaned skeleton
            varname=pars.mImageInformation.basename+"_clsk";
            filename=pars.mImageInformation.path+varname+pars.mImageInformation.extensions.back();
			WriteTIFF(res,filename.c_str());
		}
		
        kipl::morphology::SkeletonAnalysis(res,skel, lines, crossings, endpoints);

        msg.str("");
        msg<<"Skeleton info:"<<endl;
        msg<<skel.size()<<" skeleton size"<<endl;
        msg<<lines.size()<<" lines"<<endl;
        msg<<endpoints.size()<<" endpoints"<<endl<<endl;
        logger(kipl::logging::Logger::LogMessage,msg.str());

        GrowRoot(m_ImgMFR, m_ImgRoot,endpoints,pars,th_grow);
	}
	
	size_t mdims[2]={3,3};
    kipl::filters::TMedianFilter<float,2> mf(mdims);

    if (m_ImgRoot.Size()==0)
        res=mf(m_ImgThresMFR);
    else
        res=mf(m_ImgRoot);
	
	return 1;

}



int RootFinder::GrowRoot(TImage<float,2> &img, TImage<float,2> &root, deque<int> & endpoints,RootTrackConfig &pars, float th_grow)
{
    ostringstream msg;
    logger(kipl::logging::Logger::LogMessage,"Growing roots from end points");
	const int inQueue=-2;
	const int isPiece=-1;

	size_t const * const dims=img.Dims();

	TImage<int,2> binroot(dims);
	deque<int> piece,pieceQ, pieceends;
	string varname,filename;

	CNeighborhood ng(dims,2,conn4);
	int pos,p;
	size_t i;
	int piecesize=0;
	
	binroot=0;
	for (i=0; i<6; i++)
		binroot[i]=i;

	float th,tree, fringe;
	int N=0;
	int segcnt=0;
	int bridge=0;
	char t;

	while (!endpoints.empty()) {
		t='L';
        msg.str("");
        msg<<"Iteration "<<N++<<": Found "<<endpoints.size()<<" end points";

		pieceQ.clear();
		pieceends.clear();
		pos=endpoints.front();
		pieceQ.push_back(pos);
		binroot[pos]=inQueue;
		piece.clear();
		endpoints.pop_front();
		th=img[pos];
		tree=0; fringe=0;
		while (th>=th_grow) { // Test II
			while (!pieceQ.empty()) {
				pos=pieceQ.front();
				pieceQ.pop_front();
				if (binroot[pos]==inQueue) {
					piece.push_back(pos);
					binroot[pos]=isPiece;
				}
				for (i=0; i<ng.N(); i++) {
					if ((p=ng.neighbor(pos,i))!=-1) {
						if ((binroot[p]==0) && (img[p]>=th) ) {
							pieceQ.push_back(p);
							binroot[p]=inQueue;
						}
					}	
				}
			}
			piecesize=piece.size();
            if (piecesize>pars.mProcessingSettings.piece_testsize) {
				tree=TestTree(dims,piece,pieceends);
				bridge=TestFringe(binroot,piece,fringe);

                if (tree>pars.mProcessingSettings.piece_tree) {// Test IV
					t='t';	
					break;
				}

                if (piecesize>pars.mProcessingSettings.piece_maxsize) {// Test I
					t='s';
					break;
				}
	
				if (bridge) { // Test III
					t='b';
					break;
				}
				
                if (fringe>pars.mProcessingSettings.piece_fringe) {
					t='f';
					break;
				}
			}
            th-=pars.mProcessingSettings.grow_thresdec;
			pieceQ=piece;
		}

        msg<<t<<", piece size="<<piecesize<<", tree="<<tree<<", fringe="<<fringe;
        logger(kipl::logging::Logger::LogVerbose,msg.str());
		int segval=0;
        if ((piecesize<=pars.mProcessingSettings.piece_maxsize) && (piecesize>pars.mProcessingSettings.piece_minsize)) {
            switch (pars.mProcessingSettings.vis_seginfo) {
				case 0: segval=1; break;
				case 1: segval=++segcnt; break;
				case 2:
					switch (t) {
						case 'b': segval=4 ; break;
						case 'f': segval=5 ; break;
						case 's': segval=2 ; break;
						case 't': segval=3 ; break;
						case 'L': segval=1 ; break;
						default: segval=0; break;
					}
					break;
				default : segval=1; break;
			}
			copy(pieceends.begin(),pieceends.end(),back_inserter(endpoints));
		}

		while (!piece.empty()) {
			binroot[piece.front()]=segval;
			piece.pop_front();
		}

        if (pars.mProcessingSettings.save_steps==2) {
			char valstr[10];
			sprintf(valstr,"%d",segcnt);
            varname=pars.mImageInformation.basename+valstr;
            filename=pars.mImageInformation.path+"_lbl"+varname+".tif";
			WriteTIFF(binroot,filename.c_str());
		}
	}
	
    if (pars.mProcessingSettings.save_steps) {
        varname=pars.mImageInformation.basename+"_piece";
        filename=pars.mImageInformation.path+varname+".tif";
		WriteTIFF(binroot,filename.c_str());
	}
	root.Resize(dims);
	for (i=0; i<binroot.Size(); i++)
		root[i]=binroot[i]!=0;

	return 1;
}

int RootFinder::TestFringe(TImage<int,2> &binroot,deque<int> &piece, float &fringe)
{
	size_t const * const dims=binroot.Dims();

	CNeighborhood ng(dims,2,conn4);
	int cnt=0;
	deque<int>::iterator it;
	int pos,p,seg=0;
	size_t i;
	fringe=0;
	for (it=piece.begin(); it!=piece.end(); it++) {
		pos=*it;
		for (i=0; i<ng.N(); i++) {
			if ((p=ng.neighbor(pos,i))!=-1) {
				if ((binroot[p]>0)) {
					cnt++;
					if (seg && (seg!=binroot[p]))
						return 1;
					
					seg=binroot[p];
				}
			}	
		}
	}

	fringe=float(cnt)/piece.size();
	return 0;
}

float RootFinder::TestTree(size_t const * const dims,deque<int> & piece, deque<int> &endpoints)
{
	TImage<unsigned char, 2> tmp(dims),res;
	deque<int> skel;
	skel=piece;

	for (size_t i=0; i<piece.size(); i++)
		tmp[piece[i]]=1;

    kipl::morphology::thinning(tmp, res,skel);
	deque<int> lines,crossings;
    kipl::morphology::SkeletonAnalysis(res,skel, lines, crossings, endpoints);
	
	return float(crossings.size())/(lines.size()+endpoints.size());
}

int RootFinder::mfr(TImage<float,2> & img,TImage<float,2> &res, float sigma, float r, float w, int dirs)
{
	cout<<"Computing mfr image using "<<dirs<<" filters"<<endl;
	size_t const * const dims=img.Dims();

    m_KernelList.clear();

	res.Resize(dims);
	res=-1e7;
	TImage<float,2> tmp(dims);
	int x,y,k=0;
	size_t i,j, pos;
	float *K;
	float sin_phi,cos_phi,sum;
	float xval,yval;
	k=int(2.5*sigma+0.5);
    size_t fdims[2]={(2*k+1),(2*k+1)};

    float s1=2*sigma*sigma;
    float s2=r*r*s1;
	float phi=0, delta_phi=fPi/(dirs+1);
	for (i=0; i<dirs; i++) {
        kipl::base::TImage<float,2> K(fdims);
        m_KernelList.push_back(K);
        float *pK=K.GetDataPtr();

		cout<<"."<<flush;
		// Create filter kernel 
		sin_phi=sin(i*delta_phi);
		cos_phi=cos(i*delta_phi);
		sum=0.0f;
		for (pos=0,x=-k; x<=k; x++)
			for (y=-k; y<=k; y++, pos++) {
				xval=x*cos_phi+y*sin_phi;
				yval=y*cos_phi-x*sin_phi;
                //pK[pos]=-exp(-(xval*xval/s1+yval*yval/s2));

                pK[pos]=-cos(w*yval)*exp(-(xval*xval/s1+yval*yval/s2));
                sum+=pK[pos];
			}

		sum=sum/pos;
		for (x=0; x<pos; x++)
			K[x]-=sum;
				
		size_t fdims[2]={2*k+1, 2*k+1};
        TFilter<float,2> G(pK,fdims);
		tmp=G(img,kipl::filters::FilterBase::EdgeMirror);

		for (j=0; j<img.Size(); j++)
			res[j]=res[j]>tmp[j] ? res[j] : tmp[j];
	}

	cout<<endl;
	delete [] K;

	return 1;
}

float RootFinder::EstVolume(TImage<unsigned char,2> img, RootTrackConfig &pars)
{
    logger(kipl::logging::Logger::LogMessage,"Computing the volume of the root system");
	TImage<int,2> mask;
	TImage<unsigned char,2> skel;
	TImage<float,2> dist;
	deque<int> skellist;
    kipl::morphology::thinning(img,skel,skellist);
	WriteTIFF(skel,"vol_skel.tif");
	EuclideanDistance(img,dist,conn8);
	WriteTIFF(dist,"vol_dist.tif");
	deque<int>::iterator pos;
	map<int,float>::iterator volit;
	int p;
    float voxvol=pars.mProcessingSettings.pixel_size*pars.mProcessingSettings.pixel_size*pars.mProcessingSettings.pixel_size;
	
	float volume=0;
	float container_volume;
    if (pars.mProcessingSettings.sample_volume>0)
        container_volume=pars.mProcessingSettings.sample_volume;
	else
        container_volume=img.Size(0)*img.Size(1)*pars.mProcessingSettings.sample_thick*pars.mProcessingSettings.pixel_size*pars.mProcessingSettings.pixel_size;

	
    if (pars.mImageInformation.maskname.empty()) {
		for (pos=skellist.begin(); pos!=skellist.end(); pos++) {
			p=*pos;
			volume+=dist[p]*dist[p]*fPi*voxvol;
		}
		
	}
	else {
		map<int,float> regionvolume;
        ReadMAT(mask,pars.mImageInformation.maskname.c_str());
		for (pos=skellist.begin(); pos!=skellist.end(); pos++) {
			p=*pos;
			if (regionvolume.find(mask[p])==regionvolume.end())
				regionvolume[mask[p]]=dist[p]*dist[p]*fPi*voxvol;
			else
				regionvolume[mask[p]]+=dist[p]*dist[p]*fPi*voxvol;
		}
		cout<<"The volume of the segments are"<<endl;
		for (volit=regionvolume.begin(); volit!=regionvolume.end(); volit++) {
            cout<<volit->first<<" "<<volit->second<<" "<<pars.mProcessingSettings.pixel_unit<<"3 "<<100*volit->second/container_volume<<"%"<<endl;
			volume+=volit->second;
		}
			
	}
    cout<<endl<<"The total segmented root volume is : "<<volume<<" "<<pars.mProcessingSettings.pixel_unit<<"3"<<endl;
	cout<<"     corresponding to "<<100*volume/container_volume<<"% of the total volume"<<endl<<endl;
	return 1;
}

int RootFinder::CleanThresholded(TImage<float,2> img, int size)
{
	TImage<int,2> labels;
	long cnt=LabelImage(img,labels,kipl::morphology::conn8);
    vector<pair<size_t,size_t> > areas;
	LabelArea(labels,cnt,areas);

	vector<pair<size_t,size_t> >::iterator it;
	vector<size_t> remove_list;
	
	for (it=areas.begin(); it!=areas.end(); it++) {
		if (it->first < size)
			remove_list.push_back(it->second);
	}
	kipl::morphology::RemoveConnectedRegion(img,remove_list,kipl::morphology::conn8);


	return 0;
}
