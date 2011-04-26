/******************************************************************************
* Copyright (c) 2011, Michael P. Gerlek (mpg@flaxen.com)
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

#include <boost/test/unit_test.hpp>
#include <boost/cstdint.hpp>
#include <boost/property_tree/ptree.hpp>

#include <libpc/drivers/oci/Reader.hpp>
#include <libpc/drivers/oci/Writer.hpp>
#include <libpc/drivers/oci/Common.hpp>
#include <libpc/drivers/oci/Schema.hpp>

#include <libpc/drivers/liblas/Reader.hpp>

#include <libpc/filters/CacheFilter.hpp>
#include <libpc/filters/Chipper.hpp>

#include <libpc/drivers/faux/Reader.hpp>
#include <libpc/drivers/faux/Writer.hpp>

#include <libpc/drivers/liblas/Writer.hpp>
#include <libpc/Iterator.hpp>
#include <libpc/Utils.hpp>

#include "Support.hpp"
#include "TestConfig.hpp"

using namespace libpc;

using namespace libpc::drivers::oci;


bool ShouldRunTest()
{
    return TestConfig::g_oracle_connection.size() > 0;
}

Options GetOptions()
{
    Options options;
    boost::property_tree::ptree& tree = options.GetPTree();

    tree.put("capacity", 333);
    tree.put("overwrite", false);
    tree.put("connection", TestConfig::g_oracle_connection);
    tree.put("debug", true);
    tree.put("verbose", true);
    tree.put("scale.x", 0.0000001);
    tree.put("scale.y", 0.0000001);
    tree.put("scale.z", 0.001);
    tree.put("block_table_name", "LIBPC_TEST_BLOCKS");
    tree.put("base_table_name", "LIBPC_TEST_BASE");
    tree.put("select_sql", "SELECT CLOUD FROM LIBPC_TEST_BASE where ID=1");
    return options;
}

std::string ReadXML(std::string filename)
{

    std::istream* infile = Utils::openFile(filename, true);
    std::ifstream::pos_type size;
    // char* data;
    std::vector<char> data;
    if (infile->good()){
        infile->seekg(0, std::ios::end);
        size = infile->tellg();
        data.resize(static_cast<std::vector<char>::size_type>(size));
        // data = new char [size];
        infile->seekg (0, std::ios::beg);
        infile->read (&data.front(), size);
        // infile->close();

        // delete[] data;
        delete infile;
        return std::string(&data[0], data.size());
        // return data; 
    } 
    else 
    {   
        throw libpc_error("unable to open file!");
        // return data;
    }
    
}

void RunSQL(Connection connection, std::string sql)
{
    Statement statement = Statement(connection->CreateStatement(sql.c_str()));
    statement->Execute();    
}

BOOST_AUTO_TEST_SUITE(OCITest)




// BOOST_AUTO_TEST_CASE(initialize)
// {
//     if (!ShouldRunTest()) return;
//     
//     Options options = GetOptions();
//     
//     Connection connection = Connect(options);
//     
//     std::string base_table_name = options.GetPTree().get<std::string>("base_table_name");
//     std::string create_pc_table("CREATE TABLE " + base_table_name +" (id number, CLOUD SDO_PC, DESCRIPTION VARCHAR2(20), HEADER BLOB, BOUNDARY SDO_GEOMETRY)");
//     RunSQL(connection, create_pc_table);
//     
//     std::string block_table_name = options.GetPTree().get<std::string>("block_table_name");
//     std::string create_block_table = "CREATE TABLE " + block_table_name + " AS SELECT * FROM MDSYS.SDO_PC_BLK_TABLE";
//     RunSQL(connection, create_block_table);
// }
// 
// 
// BOOST_AUTO_TEST_CASE(test_writer)
// {
//     if (!ShouldRunTest()) return;
// 
//     Options options = GetOptions();
//     libpc::drivers::liblas::LiblasReader reader(Support::datapath("1.2-with-color.las"));
// 
//     boost::uint32_t capacity = GetOptions().GetPTree().get<boost::uint32_t>("capacity");
//     libpc::filters::CacheFilter cache(reader, 1, 1024);
//     libpc::filters::Chipper chipper(cache, capacity);
//     libpc::drivers::oci::Writer writer(chipper, options);
//     
//     BOOST_CHECK_MESSAGE(writer.getConnection().get(), "Unable to connect to Oracle" );
//     
//     writer.write(0);
//     
// }
// 
// BOOST_AUTO_TEST_CASE(test_reader)
// {
//     if (!ShouldRunTest()) return;
// 
//     Options options = GetOptions();
// 
//     libpc::drivers::oci::Reader reader(options);
//     const boost::uint64_t numPoints = reader.getNumPoints();
//     BOOST_CHECK_EQUAL(numPoints, 0);
//     
//     boost::scoped_ptr<SequentialIterator> iter(reader.createSequentialIterator());
//     
//     PointBuffer buffer(reader.getSchema(), 1065);
//     
//     const boost::uint32_t numPointsReadThisChunk = iter->read(buffer);
//     
//     BOOST_CHECK_EQUAL(numPointsReadThisChunk, 1065);
//     // std::ostream* ofs = Utils::createFile("temp.las");
//     // 
//     // libpc::drivers::liblas::LiblasWriter writer(reader, *ofs);
//     // writer.write(0);
//     // 
//     // Utils::closeFile(ofs);
// 
//     
// }

BOOST_AUTO_TEST_CASE(test_schema)
{
    if (!ShouldRunTest()) return;

    
    // std::string xml = ReadXML("/Users/hobu/hg/liblas/schemas/las.xml");
    std::string xml = ReadXML("/Users/hobu/hg/weblas/8-dimension-schema.xml");
    std::string xsd = ReadXML("/Users/hobu/hg/liblas/schemas/LAS.xsd");
    
    libpc::drivers::oci::Schema schema(xml, xsd);
    
    
}

// BOOST_AUTO_TEST_CASE(clean_up)
// {
//     if (!ShouldRunTest()) return;
//     
//     libpc::drivers::oci::Options options = GetOptions();
//     
//     Connection connection = Connect(options);
// 
//     std::string base_table_name = options.GetPTree().get<std::string>("base_table_name");
//     std::string block_table_name = options.GetPTree().get<std::string>("block_table_name");
//     
//     std::string drop_base_table = "DROP TABLE " + base_table_name;
//     std::string drop_block_table = "DROP TABLE " + block_table_name;
//     RunSQL(connection, drop_base_table);
//     RunSQL(connection, drop_block_table);    
// }

BOOST_AUTO_TEST_SUITE_END()