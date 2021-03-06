/******************************************************************************
* Copyright (c) 2012, Howard Butler, hobu.inc@gmail.com
* Copyright (c) 2013, Paul Ramsey, pramsey@cleverelephant.ca
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

#include <pdal/Writer.hpp>
#include <pdal/drivers/pgpointcloud/common.hpp>

namespace pdal
{
namespace drivers
{
namespace pgpointcloud
{


class PDAL_DLL Writer : public pdal::Writer
{
public:
    SET_STAGE_NAME("drivers.pgpointcloud.writer", "PostgresSQL Pointcloud Database Writer")
    SET_STAGE_LINK("http://pdal.io/stages/drivers.pgpointcloud.writer.html")
#ifdef PDAL_HAVE_POSTGRESQL
    SET_STAGE_ENABLED(true)
#else
    SET_STAGE_ENABLED(false)
#endif

    Writer();
    ~Writer();

    static Options getDefaultOptions();


private:
    Writer& operator=(const Writer&); // not implemented
    Writer(const Writer&); // not implemented

    virtual void processOptions(const Options& options);
    virtual void ready(PointContextRef ctx);
    virtual void write(const PointBuffer& pointBuffer);
    virtual void done(PointContextRef ctx);
    virtual void initialize();

    void writeInit();
    void writeTile(const PointBuffer& buffer);

    bool CheckTableExists(std::string const& name);
    bool CheckPointCloudExists();
    bool CheckPostGISExists();
    uint32_t SetupSchema(uint32_t srid);

    void CreateTable(std::string const& schema_name,
                     std::string const& table_name,
                     std::string const& column_name,
                     boost::uint32_t pcid);

    void DeleteTable(std::string const& schema_name,
                     std::string const& table_name);

    void CreateIndex(std::string const& schema_name,
                     std::string const& table_name,
                     std::string const& column_name);

    bool WriteBlock(PointBuffer const& buffer);

    PGconn* m_session;
    std::string m_schema_name;
    std::string m_table_name;
    std::string m_column_name;
    std::string m_connection;
    CompressionType::Enum m_patch_compression_type;
    uint32_t m_patch_capacity;
    uint32_t m_srid;
    uint32_t m_pcid;
    bool m_have_postgis;
    bool m_create_index;
    bool m_overwrite;
    std::string m_insert;
    std::string m_hex;
    size_t m_pointSize;
    Orientation::Enum m_orientation;
    bool m_pack;
    std::string m_pre_sql;
    Dimension::IdList m_dims;
    std::vector<Dimension::Type::Enum> m_types;

    // lose this
    bool m_schema_is_initialized;
};

} // namespace pgpointcloud
} // namespace drivers
} // namespace pdal

