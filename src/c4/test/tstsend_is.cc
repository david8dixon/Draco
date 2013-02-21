//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   c4/test/tstsend_is.cc
 * \author Kelly Thompson
 * \date   Friday, Dec 07, 2012, 14:02 pm
 * \brief  Unit tests for rtt_c4::send_is()
 * \note   Copyright (C) 2002-2012 Los Alamos National Security, LLC.
 *         All rights reserved.
 */
//---------------------------------------------------------------------------//
// $Id: tstsend_is.cc 5830 2011-05-05 19:43:43Z kellyt $
//---------------------------------------------------------------------------//

#include "../ParallelUnitTest.hh"
#include "ds++/Release.hh"
#include <sstream>
#include <vector>

#define PASSMSG(A) ut.passes(A)
#define FAILMSG(A) ut.failure(A)
#define ITFAILS    ut.failure( __LINE__ )

//---------------------------------------------------------------------------//
// TESTS
//---------------------------------------------------------------------------//

void test_simple( rtt_dsxx::UnitTest &ut )
{
    // borrowed from http://mpi.deino.net/mpi_functions/MPI_Issend.html.

    if( rtt_c4::node() == 0 )
        std::cout << "Test send_is() by sending data to proc myid+1..."
                  << std::endl;

    // C4_Req communication handles.
    std::vector<C4::C4_Req> comm_int(2);

    // for point-to-point communiction we need to know neighbor's identifiers:
    // left, right.
    int right = (rtt_c4::node()+1)%rtt_c4::nodes();
    int left  = rtt_c4::node()-1;
    if( left < 0 )
        left = rtt_c4::nodes()-1;

    // create some data to send/recv
    unsigned int const bsize(10);
    std::vector<int> buffer2(bsize);
    std::vector<int> buffer1(bsize);
    for( size_t i=0;i<bsize;++i )
        buffer1[i] = 1000*rtt_c4::node()+i;

    // post asynchronous receives.
    comm_int[0] = rtt_c4::receive_async( &buffer2[0], buffer2.size(), left );

    // send data using non-blocking synchronous send.
    rtt_c4::send_is( comm_int[1], &buffer1[0], buffer1.size(), right );
    
    // wait for all communication to finish
    rtt_c4::wait_all( comm_int.size(), &comm_int[0] );

    // exected results
    std::vector<int> expected(bsize);
    for( size_t i=0;i<bsize;++i )
        expected[i] = 1000*left+i;

    if( expected == buffer2 )
    {
        std::ostringstream msg;
        msg << "Expected data found after send_is() on node " << rtt_c4::node()
            << ".";
        PASSMSG( msg.str() );
    }
    else
    {
        std::ostringstream msg;
        msg << "Did not find expected data after send_is() on node " << rtt_c4::node()
            << ".";
        FAILMSG( msg.str() );
    }
    
    return;
}

//---------------------------------------------------------------------------//

int main(int argc, char *argv[])
{
    rtt_c4::ParallelUnitTest ut(argc, argv, rtt_dsxx::release);
    try
    {
        // Unit tests
        test_simple(ut);
    }
    catch (std::exception &err)
    {
        std::cout << "ERROR: While testing tstsend_is, " 
                  << err.what() << std::endl;
        ut.numFails++;
    }
    catch( ... )
    {
        std::cout << "ERROR: While testing tstsend_is, " 
                  << "An unknown exception was thrown." << std::endl;
        ut.numFails++;
    }
    return ut.numFails;
}   

//---------------------------------------------------------------------------//
//                        end of tstsend_is.cc
//---------------------------------------------------------------------------//