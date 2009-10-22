// -*- C++ -*-

// Copyright 2006-2007 Deutsches Forschungszentrum fuer Kuenstliche Intelligenz
// or its licensors, as applicable.
//
// You may not use this file except under the terms of the accompanying license.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may
// obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Project: roughocr -- mock OCR system exercising the interfaces and useful for testing
// File: ocr-cseg-ccs.cc
// Purpose: attempt to segment a line (or page) of text into characters by labeling the connected components
// Responsible: tmb
// Reviewer:
// Primary Repository:
// Web Sites: www.iupr.org, www.dfki.de

#include "colib/colib.h"
#include "iulib/imgio.h"
#include "iulib/imglib.h"
#include "ocropus.h"

using namespace ocropus;
using namespace iulib;
using namespace colib;

struct SegmentLineByCCS : ISegmentLine {
    SegmentLineByCCS() {
        pdef("swidth",0,"smearing width");
        pdef("sheight",10,"smearing height");
    }

    ~SegmentLineByCCS() {}

    const char *description() {
        return "connected component segmenter\n";
    }

    const char *name() {
        return "segccs";
    }

    void init(const char **argv) {
        // nothing to be done
    }

    void charseg(intarray &image,bytearray &in) {
        using namespace narray_ops;
        int swidth = pgetf("swidth");
        int sheight = pgetf("sheight");
        bytearray temp;
        copy(image,in);
        for(int i=0;i<image.length();i++) image[i] = !image[i];
        copy(temp,image);
        if(swidth>0||sheight>0) binary_close_rect(temp,swidth,sheight);
        intarray labels;
        copy(labels,temp);
        label_components(labels);
        for(int i=0;i<image.length1d();i++)
            if(image.at1d(i))
                image.at1d(i) = cseg_pixel(labels.at1d(i));
        make_line_segmentation_white(image);
        check_line_segmentation(image);
    }
};

namespace ocropus {
    ISegmentLine *make_SegmentLineByCCS() {
        return new SegmentLineByCCS();
    }
}

