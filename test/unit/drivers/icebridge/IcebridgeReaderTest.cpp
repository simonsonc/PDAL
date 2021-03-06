/******************************************************************************
* Copyright (c) 2014, Connor Manning, connor@hobu.co
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

#include "UnitTest.hpp"

#include <pdal/Options.hpp>
#include <pdal/PointBuffer.hpp>
#include <pdal/PipelineReader.hpp>
#include <pdal/PipelineManager.hpp>

#include <pdal/drivers/icebridge/Reader.hpp>

#include "StageTester.hpp"
#include "Support.hpp"

using namespace pdal;

template <typename T>
void checkDimension(const PointBuffer& data, std::size_t index,
    Dimension::Id::Enum dim, T expected)
{
    float actual = data.getFieldAs<T>(dim, index);
    BOOST_CHECK_CLOSE(expected, actual, 0.000001);
}

void checkPoint(
        const PointBuffer& data,
        std::size_t index,
        float time,
        float latitude,
        float longitude,
        float elevation,
        int xmtSig,
        int rcvSig,
        float azimuth,
        float pitch,
        float roll,
        float gpsPdop,
        float pulseWidth,
        float relTime)
{
    using namespace Dimension;
    checkDimension(data, index, Id::OffsetTime, time);
    checkDimension(data, index, Id::Y, latitude);
    checkDimension(data, index, Id::X, longitude);
    checkDimension(data, index, Id::Z, elevation);
    checkDimension(data, index, Id::StartPulse, xmtSig);
    checkDimension(data, index, Id::ReflectedPulse, rcvSig);
    checkDimension(data, index, Id::ScanAngleRank, azimuth);
    checkDimension(data, index, Id::Pitch, pitch);
    checkDimension(data, index, Id::Roll, roll);
    checkDimension(data, index, Id::Pdop, gpsPdop);
    checkDimension(data, index, Id::PulseWidth, pulseWidth);
    checkDimension(data, index, Id::GpsTime, relTime);
}

std::string getFilePath()
{
    return Support::datapath("icebridge/twoPoints.h5");
}

BOOST_AUTO_TEST_SUITE(IcebridgeReaderTest)

BOOST_AUTO_TEST_CASE(testRead)
{
    Option filename("filename", getFilePath(), "");
    Options options(filename);
    drivers::icebridge::Reader reader;
    reader.setOptions(options);

    PointContext ctx;
    reader.prepare(ctx);
    PointBufferSet pbSet = reader.execute(ctx);
    BOOST_CHECK_EQUAL(pbSet.size(), 1);
    PointBufferPtr buf = *pbSet.begin();
    BOOST_CHECK_EQUAL(buf->size(), 2);

    checkPoint(
            *buf,
            0,
            141437548,     // time
            82.605319,      // latitude
            301.406196,     // longitude
            18.678,         // elevation
            2408,           // xmtSig
            181,            // rcvSig
            49.91,          // azimuth
            -4.376,         // pitch
            0.608,          // roll
            2.9,            // gpsPdop
            20.0,           // pulseWidth
            0.0);           // relTime

    checkPoint(
            *buf,
            1,
            141437548,     // time
            82.605287,      // latitude
            301.404862,     // longitude
            18.688,         // elevation
            2642,           // xmtSig
            173,            // rcvSig
            52.006,         // azimuth
            -4.376,         // pitch
            0.609,          // roll
            2.9,            // gpsPdop
            17.0,           // pulseWidth
            0.0);           // relTime
}

BOOST_AUTO_TEST_CASE(testPipeline)
{
    PipelineManager manager;
    PipelineReader reader(manager);

    bool isWriter =
        reader.readPipeline(Support::datapath("icebridge/pipeline.xml"));
    BOOST_CHECK(isWriter);

    point_count_t numPoints = manager.execute();
    BOOST_CHECK_EQUAL(numPoints, 2);
    FileUtils::deleteFile(Support::datapath("icebridge/outfile.txt"));
}


BOOST_AUTO_TEST_SUITE_END()
