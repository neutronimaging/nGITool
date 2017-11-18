/***************************************************************************
 *   Copyright (C) 2005 by Anders Kaestner   *
 *   anders.kaestner@env.ethz.ch   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef __ROOTFINDER_H
#define __ROOTFINDER_H

#include <base/timage.h>
#include <morphology/skeleton.h>
#include <logging/logger.h>
#include <map>
#include <vector>
#include <string>
class RootTrackConfig;

class RootFinder
{
    kipl::logging::Logger logger;
public:
    RootFinder();

    int Process(kipl::base::TImage<float,2> &img, TImage<float,2> &res, RootTrackConfig &pars);

    float EstVolume(kipl::base::TImage<unsigned char,2> img, RootTrackConfig &pars);
    int mfr(TImage<float,2> & img,TImage<float,2> &res, float sigma, float r, float w, int dirs);

    int GrowRoot(TImage<float,2> &img, TImage<float,2> &root, deque<int> & endpoints,RootTrackConfig &pars, float th_grow);

    std::map<std::string, kipl::base::TImage<float,2> > m_ImageList;
    std::vector<kipl::base::TImage<float, 2> > m_KernelList;
protected:
    float TestTree(size_t const * const dims,deque<int> & piece, deque<int> &endpoints);
    int TestFringe(TImage<int,2> &binroot,deque<int> &piece,float &fringe);
    int CleanThresholded(TImage<float,2> img, int size);
};
#endif
