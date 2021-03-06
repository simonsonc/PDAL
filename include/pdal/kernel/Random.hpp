/******************************************************************************
* Copyright (c) 2014, Brad Chambers (brad.chambers@gmail.com)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#pragma once

#include <pdal/FileUtils.hpp>
#include <pdal/kernel/Kernel.hpp>
#include <pdal/drivers/faux/Reader.hpp>
#include <pdal/drivers/las/Writer.hpp>

#include <pdal/Bounds.hpp>

#define SEPARATORS ",| "

#include <boost/tokenizer.hpp>
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

PDAL_C_START

PDAL_DLL void PDALRegister_kernel_random(void* factory);

PDAL_C_END

namespace pdal
{
namespace kernel
{

class PDAL_DLL Random : public Kernel
{
public:
    SET_KERNEL_NAME ("drivers.random.kernel", "Random Kernel")
    SET_KERNEL_LINK ("http://pdal.io/kernels/drivers.random.kernel.html")
    SET_KERNEL_ENABLED (true)

    Random();
    int execute();

private:
    void addSwitches();
    void validateSwitches();

    Stage* makeReader(Options readerOptions);

    std::string m_outputFile;
    bool m_bCompress;
    boost::uint64_t m_numPointsToWrite;
    BOX3D m_bounds;
    std::string m_distribution;
    std::string m_means;
    std::string m_stdevs;
};

} // kernel
} // pdal
