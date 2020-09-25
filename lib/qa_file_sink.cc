/*
 * Copyright 2012 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "sandia_utils/constants.h"
#include "sandia_utils/file_sink.h"
#include "sandia_utils/file_source.h"
#include <gnuradio/block_detail.h>
#include <gnuradio/blocks/message_debug.h>
#include <gnuradio/blocks/null_source.h>
#include <gnuradio/blocks/throttle.h>
#include <gnuradio/blocks/vector_source.h>
#include <gnuradio/buffer.h>
#include <gnuradio/tags.h>
#include <gnuradio/top_block.h>
#include <pmt/pmt.h>
#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>


namespace gr {
namespace sandia_utils {
bool remove_file(std::string fname)
{
    boost::filesystem::path fs(fname);
    if (boost::filesystem::exists(fs)) {
        return boost::filesystem::remove(fs);
    }

    return false;
}

gr::tag_t gen_tag(pmt::pmt_t key, pmt::pmt_t value, uint64_t offset)
{
    gr::tag_t tag;
    tag.key = key;
    tag.value = value;
    tag.offset = offset;

    return tag;
}

// test instantiation of all file data types
BOOST_AUTO_TEST_CASE(t0)
{
    // complex float
    gr::sandia_utils::file_sink::sptr sink(
        gr::sandia_utils::file_sink::make("complex",
                                          sizeof(gr_complex),
                                          "raw",
                                          gr::sandia_utils::MANUAL,
                                          0,
                                          1000,
                                          "/tmp",
                                          "test"));

    BOOST_REQUIRE_EQUAL(std::string("file_sink"), sink->name());
    BOOST_REQUIRE_EQUAL(1, sink->input_signature()->min_streams());
    BOOST_REQUIRE_EQUAL(1, sink->input_signature()->max_streams());
    BOOST_REQUIRE_EQUAL(0, sink->output_signature()->min_streams());
    BOOST_REQUIRE_EQUAL(0, sink->output_signature()->max_streams());
    BOOST_REQUIRE_EQUAL((int)sizeof(gr_complex),
                        sink->input_signature()->sizeof_stream_item(0));

    // complex int
    sink = gr::sandia_utils::file_sink::make("complex_int",
                                             2 * sizeof(short),
                                             "raw",
                                             gr::sandia_utils::MANUAL,
                                             0,
                                             1000,
                                             "/tmp",
                                             "test");

    BOOST_REQUIRE_EQUAL(std::string("file_sink"), sink->name());
    BOOST_REQUIRE_EQUAL(1, sink->input_signature()->min_streams());
    BOOST_REQUIRE_EQUAL(1, sink->input_signature()->max_streams());
    BOOST_REQUIRE_EQUAL(0, sink->output_signature()->min_streams());
    BOOST_REQUIRE_EQUAL(0, sink->output_signature()->max_streams());
    BOOST_REQUIRE_EQUAL(2 * (int)sizeof(short),
                        sink->input_signature()->sizeof_stream_item(0));

    // float
    sink = gr::sandia_utils::file_sink::make(
        "float", sizeof(float), "raw", gr::sandia_utils::MANUAL, 0, 1000, "/tmp", "test");

    BOOST_REQUIRE_EQUAL(std::string("file_sink"), sink->name());
    BOOST_REQUIRE_EQUAL(1, sink->input_signature()->min_streams());
    BOOST_REQUIRE_EQUAL(1, sink->input_signature()->max_streams());
    BOOST_REQUIRE_EQUAL(0, sink->output_signature()->min_streams());
    BOOST_REQUIRE_EQUAL(0, sink->output_signature()->max_streams());
    BOOST_REQUIRE_EQUAL((int)sizeof(float),
                        sink->input_signature()->sizeof_stream_item(0));

    // int
    sink = gr::sandia_utils::file_sink::make(
        "int", sizeof(int32_t), "raw", gr::sandia_utils::MANUAL, 0, 1000, "/tmp", "test");

    BOOST_REQUIRE_EQUAL(std::string("file_sink"), sink->name());
    BOOST_REQUIRE_EQUAL(1, sink->input_signature()->min_streams());
    BOOST_REQUIRE_EQUAL(1, sink->input_signature()->max_streams());
    BOOST_REQUIRE_EQUAL(0, sink->output_signature()->min_streams());
    BOOST_REQUIRE_EQUAL(0, sink->output_signature()->max_streams());
    BOOST_REQUIRE_EQUAL((int)sizeof(int32_t),
                        sink->input_signature()->sizeof_stream_item(0));

    // short
    sink = gr::sandia_utils::file_sink::make(
        "short", sizeof(short), "raw", gr::sandia_utils::MANUAL, 0, 1000, "/tmp", "test");

    BOOST_REQUIRE_EQUAL(std::string("file_sink"), sink->name());
    BOOST_REQUIRE_EQUAL(1, sink->input_signature()->min_streams());
    BOOST_REQUIRE_EQUAL(1, sink->input_signature()->max_streams());
    BOOST_REQUIRE_EQUAL(0, sink->output_signature()->min_streams());
    BOOST_REQUIRE_EQUAL(0, sink->output_signature()->max_streams());
    BOOST_REQUIRE_EQUAL((int)sizeof(short),
                        sink->input_signature()->sizeof_stream_item(0));

    // byte
    sink = gr::sandia_utils::file_sink::make(
        "byte", sizeof(char), "raw", gr::sandia_utils::MANUAL, 0, 1000, "/tmp", "test");

    BOOST_REQUIRE_EQUAL(std::string("file_sink"), sink->name());
    BOOST_REQUIRE_EQUAL(1, sink->input_signature()->min_streams());
    BOOST_REQUIRE_EQUAL(1, sink->input_signature()->max_streams());
    BOOST_REQUIRE_EQUAL(0, sink->output_signature()->min_streams());
    BOOST_REQUIRE_EQUAL(0, sink->output_signature()->max_streams());
    BOOST_REQUIRE_EQUAL((int)sizeof(char),
                        sink->input_signature()->sizeof_stream_item(0));
}

// test instantiation of all file types
BOOST_AUTO_TEST_CASE(t1)
{
    // raw
    gr::sandia_utils::file_sink::sptr sink(
        gr::sandia_utils::file_sink::make("complex",
                                          sizeof(gr_complex),
                                          "raw",
                                          gr::sandia_utils::MANUAL,
                                          0,
                                          1000,
                                          "/tmp",
                                          "test"));

    BOOST_REQUIRE_EQUAL(std::string("file_sink"), sink->name());

    // raw header
    sink = gr::sandia_utils::file_sink::make("complex",
                                             sizeof(gr_complex),
                                             "raw_header",
                                             gr::sandia_utils::MANUAL,
                                             0,
                                             1000,
                                             "/tmp",
                                             "test");

    BOOST_REQUIRE_EQUAL(std::string("file_sink"), sink->name());

#ifdef HAVE_BLUEFILE_LIB
    // bluefile
    sink = gr::sandia_utils::file_sink::make(
        "complex_int", 4, "bluefile", gr::sandia_utils::MANUAL, 0, 1000, "/tmp", "test");

    BOOST_REQUIRE_EQUAL(std::string("file_sink"), sink->name());
#endif
}

// test streaming to a single file
BOOST_AUTO_TEST_CASE(t2)
{
    // generate blocks
    gr::block_sptr src(gr::blocks::null_source::make(sizeof(gr_complex)));
    gr::sandia_utils::file_sink::sptr sink(
        gr::sandia_utils::file_sink::make("complex",
                                          sizeof(gr_complex),
                                          "raw",
                                          gr::sandia_utils::MANUAL,
                                          0,
                                          1000,
                                          "/tmp",
                                          "test.fc32"));
    gr::blocks::message_debug::sptr debug(gr::blocks::message_debug::make());
    gr::block_sptr throttle(gr::blocks::throttle::make(sizeof(gr_complex), 32e3, true));

    // set sink to start recording
    sink->set_second_align(false);
    sink->set_gen_new_folder(false);
    sink->set_recording(true);

    gr::top_block_sptr tb(gr::make_top_block("t2"));
    tb->connect(src, 0, throttle, 0);
    tb->connect(throttle, 0, sink, 0);
    tb->msg_connect(sink, "pdu", debug, "store");
    tb->msg_connect(sink, "pdu", debug, "print");
    tb->start();

    // let it run for sometime
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));

    // stop recording
    sink->set_recording(false);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));

    // stop
    tb->stop();
    tb->wait();

    // collect results
    BOOST_REQUIRE_EQUAL(debug->num_messages(), 1);
    pmt::pmt_t message = debug->get_message(0);
    BOOST_REQUIRE_EQUAL(true, true);

    // clean up
    remove_file("/tmp/test.fc32");
}

// test tag name generation
BOOST_AUTO_TEST_CASE(t3)
{
    // use a vector source block to place tags in the stream
    std::vector<gr::tag_t> tags;
    tags.push_back(gen_tag(gr::sandia_utils::RATE_KEY, pmt::from_double(30.72e6), 0));
    tags.push_back(gen_tag(gr::sandia_utils::FREQ_KEY, pmt::from_double(915e6), 0));
    tags.push_back(gen_tag(gr::sandia_utils::RX_TIME_KEY,
                           pmt::make_tuple(pmt::from_uint64(0), pmt::from_double(0)),
                           0));

    // generate blocks - set throttle at a low rate so that all vector source
    // samples are not flushed before the file sink is stopped, cause the
    // flowgraph to be stopped
    std::vector<gr_complex> data(40000);
    gr::block_sptr src(gr::blocks::vector_source_c::make(data, true, 1, tags));
    gr::sandia_utils::file_sink::sptr sink(
        gr::sandia_utils::file_sink::make("complex",
                                          sizeof(gr_complex),
                                          "raw",
                                          gr::sandia_utils::MANUAL,
                                          0,
                                          1000,
                                          "/tmp",
                                          "%Y%m%d_%H_%M_%S_fc=%fcMMHz_fs=%fskkHz.dat"));
    gr::blocks::message_debug::sptr debug(gr::blocks::message_debug::make());
    gr::block_sptr throttle(gr::blocks::throttle::make(sizeof(gr_complex), 1000, true));

    // set sink to start recording
    sink->set_second_align(false);
    sink->set_gen_new_folder(false);
    sink->set_recording(true);

    gr::top_block_sptr tb(gr::make_top_block("t3"));
    tb->connect(src, 0, throttle, 0);
    tb->connect(throttle, 0, sink, 0);
    tb->msg_connect(sink, "pdu", debug, "store");
    tb->msg_connect(sink, "pdu", debug, "print");
    tb->start();

    // let it run for sometime
    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));

    // stop recording so message will be published
    sink->set_recording(false);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

    // stop
    tb->stop();
    tb->wait();

    // collect results
    BOOST_REQUIRE_EQUAL(debug->num_messages(), 1);
    pmt::pmt_t message = debug->get_message(0);
    BOOST_REQUIRE_EQUAL(pmt::is_pair(message), true);
    pmt::pmt_t dict = pmt::car(message);
    BOOST_REQUIRE_EQUAL(
        pmt::equal(pmt::dict_ref(dict, gr::sandia_utils::RATE_KEY, pmt::PMT_NIL),
                   pmt::from_double(30.72e6)),
        true);
    BOOST_REQUIRE_EQUAL(
        pmt::equal(pmt::dict_ref(dict, gr::sandia_utils::FREQ_KEY, pmt::PMT_NIL),
                   pmt::from_double(915e6)),
        true);
    BOOST_REQUIRE_EQUAL(
        pmt::equal(pmt::dict_ref(dict, gr::sandia_utils::RX_TIME_KEY, pmt::PMT_NIL),
                   pmt::make_tuple(pmt::from_uint64(0), pmt::from_double(0))),
        true);

    std::string expected_fname = "/tmp/19700101_00_00_00_fc=915MHz_fs=30720kHz.dat";
    BOOST_REQUIRE_EQUAL(
        pmt::equal(pmt::dict_ref(dict, gr::sandia_utils::FNAME_KEY, pmt::PMT_NIL),
                   pmt::intern(expected_fname)),
        true);

    // clean up
    remove_file(expected_fname);
}

// test tag name generation
BOOST_AUTO_TEST_CASE(t4)
{
    // use a vector source block to place two tags in the stream
    std::vector<gr::tag_t> tags;
    tags.push_back(gen_tag(gr::sandia_utils::RATE_KEY, pmt::from_double(30.72e6), 0));
    tags.push_back(gen_tag(gr::sandia_utils::FREQ_KEY, pmt::from_double(915e6), 0));
    tags.push_back(gen_tag(gr::sandia_utils::RX_TIME_KEY,
                           pmt::make_tuple(pmt::from_uint64(0), pmt::from_double(0)),
                           0));

    // generate blocks - set throttle at a low rate so that all vector source
    // samples are not flushed before the file sink is stopped, cause the
    // flowgraph to be stopped
    std::vector<gr_complex> data(40000);
    gr::block_sptr src(gr::blocks::vector_source_c::make(data, true, 1, tags));
    gr::sandia_utils::file_sink::sptr sink(
        gr::sandia_utils::file_sink::make("complex",
                                          sizeof(gr_complex),
                                          "raw",
                                          gr::sandia_utils::MANUAL,
                                          0,
                                          1000,
                                          "/tmp",
                                          "test.fc32"));
    gr::blocks::message_debug::sptr debug(gr::blocks::message_debug::make());
    gr::block_sptr throttle(gr::blocks::throttle::make(sizeof(gr_complex), 1000, true));

    // set sink to start recording
    sink->set_second_align(false);
    sink->set_gen_new_folder(false);
    sink->set_recording(true);

    gr::top_block_sptr tb(gr::make_top_block("t4"));
    tb->connect(src, 0, throttle, 0);
    tb->connect(throttle, 0, sink, 0);
    tb->msg_connect(sink, "pdu", debug, "store");
    tb->msg_connect(sink, "pdu", debug, "print");
    tb->start();

    // let it run for sometime
    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));

    // stop recording so message will be published
    sink->set_recording(false);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));

    // stop
    tb->stop();
    tb->wait();

    // collect results - only a single file should have been emitted
    BOOST_REQUIRE_EQUAL(debug->num_messages(), 1);

    // clean up
    remove_file("/tmp/test.fc32");
}


// test generation of multiple files based on number of samples in file
BOOST_AUTO_TEST_CASE(t5)
{
    // generate blocks
    std::vector<gr_complex> data(4000);
    gr::blocks::vector_source_c::sptr src(
        gr::blocks::vector_source_c::make(data, false, 1));
    gr::sandia_utils::file_sink::sptr sink(
        gr::sandia_utils::file_sink::make("complex",
                                          sizeof(gr_complex),
                                          "raw",
                                          gr::sandia_utils::MANUAL,
                                          2000,
                                          2000,
                                          "/tmp",
                                          "t_%02fd.fc32"));
    gr::blocks::message_debug::sptr debug(gr::blocks::message_debug::make());
    gr::block_sptr throttle(gr::blocks::throttle::make(sizeof(gr_complex), 32e3, true));

    // set sink to start recording
    sink->set_second_align(false);
    sink->set_gen_new_folder(false);
    sink->set_recording(true);

    gr::top_block_sptr tb(gr::make_top_block("t5"));
    tb->connect(src, 0, throttle, 0);
    tb->connect(throttle, 0, sink, 0);
    tb->msg_connect(sink, "pdu", debug, "store");
    tb->msg_connect(sink, "pdu", debug, "print");
    tb->start();

    // let it run for sometime
    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));

    // stop recording so message will be published
    sink->set_recording(false);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

    // stop
    tb->stop();
    tb->wait();

    // collect results - only a single file should have been emitted
    BOOST_REQUIRE_EQUAL(debug->num_messages(), 2);
    pmt::pmt_t message1 = debug->get_message(0);
    pmt::pmt_t message2 = debug->get_message(1);
    BOOST_REQUIRE_EQUAL(pmt::is_pair(message1), true);
    BOOST_REQUIRE_EQUAL(pmt::is_pair(message2), true);

    // get time of first message
    pmt::pmt_t dict1 = pmt::car(message1);
    pmt::pmt_t dict2 = pmt::car(message2);
    BOOST_REQUIRE_EQUAL(
        pmt::equal(pmt::dict_ref(dict1, gr::sandia_utils::RX_TIME_KEY, pmt::PMT_NIL),
                   pmt::PMT_NIL),
        false);
    BOOST_REQUIRE_EQUAL(
        pmt::equal(pmt::dict_ref(dict2, gr::sandia_utils::RX_TIME_KEY, pmt::PMT_NIL),
                   pmt::PMT_NIL),
        false);
    pmt::pmt_t time_tuple1 =
        pmt::dict_ref(dict1, gr::sandia_utils::RX_TIME_KEY, pmt::PMT_NIL);
    uint64_t tstart1 = pmt::to_uint64(pmt::tuple_ref(time_tuple1, 0));

    pmt::pmt_t time_tuple2 =
        pmt::dict_ref(dict2, gr::sandia_utils::RX_TIME_KEY, pmt::PMT_NIL);
    uint64_t tstart2 = pmt::to_uint64(pmt::tuple_ref(time_tuple2, 0));
    BOOST_REQUIRE_EQUAL(tstart2, tstart1 + 1);

    // clean up
    BOOST_REQUIRE_EQUAL(remove_file("/tmp/t_00.fc32"), true);
    BOOST_REQUIRE_EQUAL(remove_file("/tmp/t_01.fc32"), true);
}


BOOST_AUTO_TEST_CASE(t6)
{
    // use a vector source block to place two tags in the stream
    std::vector<gr::tag_t> tags;
    tags.push_back(gen_tag(gr::sandia_utils::RATE_KEY, pmt::from_double(30.72e6), 0));
    tags.push_back(gen_tag(gr::sandia_utils::FREQ_KEY, pmt::from_double(915e6), 0));
    tags.push_back(gen_tag(gr::sandia_utils::RX_TIME_KEY,
                           pmt::make_tuple(pmt::from_uint64(0), pmt::from_double(0)),
                           0));

    // add a second tag with an updated time to simulate an overflow
    tags.push_back(gen_tag(gr::sandia_utils::RX_TIME_KEY,
                           pmt::make_tuple(pmt::from_uint64(1), pmt::from_double(0)),
                           2000));

    // generate blocks - set throttle at a low rate so that all vector source
    // samples are not flushed before the file sink is stopped, cause the
    // flowgraph to be stopped
    std::vector<gr_complex> data(40000);
    gr::blocks::vector_source_c::sptr src(
        gr::blocks::vector_source_c::make(data, true, 1, tags));
    gr::sandia_utils::file_sink::sptr sink(
        gr::sandia_utils::file_sink::make("complex",
                                          sizeof(gr_complex),
                                          "raw",
                                          gr::sandia_utils::MANUAL,
                                          4000,
                                          1000,
                                          "/tmp",
                                          "t_%02fd.fc32"));
    gr::blocks::message_debug::sptr debug(gr::blocks::message_debug::make());
    gr::block_sptr throttle(gr::blocks::throttle::make(sizeof(gr_complex), 1000, true));

    // set sink to start recording
    sink->set_second_align(false);
    sink->set_gen_new_folder(false);
    sink->set_recording(true);

    // set the work function size call smaller than the first tag so it appears
    // in a second work function call
    sink->set_max_noutput_items(512);

    gr::top_block_sptr tb(gr::make_top_block("t6"));
    tb->connect(src, 0, throttle, 0);
    tb->connect(throttle, 0, sink, 0);
    tb->msg_connect(sink, "pdu", debug, "store");
    tb->msg_connect(sink, "pdu", debug, "print");
    tb->start();

    // let it run for sometime
    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));

    // stop recording so message will be published
    sink->set_recording(false);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

    // stop
    tb->stop();
    tb->wait();

    // collect results -two files should have been emitted
    BOOST_REQUIRE_EQUAL(debug->num_messages(), 2);
    pmt::pmt_t message1 = debug->get_message(0);
    pmt::pmt_t message2 = debug->get_message(1);
    BOOST_REQUIRE_EQUAL(pmt::is_pair(message1), true);
    BOOST_REQUIRE_EQUAL(pmt::is_pair(message2), true);

    // get time of first message
    pmt::pmt_t dict1 = pmt::car(message1);
    pmt::pmt_t dict2 = pmt::car(message2);
    BOOST_REQUIRE_EQUAL(
        pmt::equal(pmt::dict_ref(dict1, gr::sandia_utils::RX_TIME_KEY, pmt::PMT_NIL),
                   pmt::PMT_NIL),
        false);
    BOOST_REQUIRE_EQUAL(
        pmt::equal(pmt::dict_ref(dict2, gr::sandia_utils::RX_TIME_KEY, pmt::PMT_NIL),
                   pmt::PMT_NIL),
        false);
    pmt::pmt_t time_tuple1 =
        pmt::dict_ref(dict1, gr::sandia_utils::RX_TIME_KEY, pmt::PMT_NIL);
    pmt::pmt_t time_tuple2 =
        pmt::dict_ref(dict2, gr::sandia_utils::RX_TIME_KEY, pmt::PMT_NIL);
    uint64_t tstart1 = pmt::to_uint64(pmt::tuple_ref(time_tuple1, 0));
    uint64_t tstart2 = pmt::to_uint64(pmt::tuple_ref(time_tuple2, 0));
    BOOST_REQUIRE_EQUAL(tstart1, uint64_t(0));
    BOOST_REQUIRE_EQUAL(tstart2, uint64_t(1));

    // clean up
    BOOST_REQUIRE_EQUAL(remove_file("/tmp/t_00.fc32"), true);
    BOOST_REQUIRE_EQUAL(remove_file("/tmp/t_01.fc32"), true);

    // clean up
    // remove_file("/tmp/test.fc32");
}

// test streaming with tags on first sample of work function call
// https://gitlab.sandia.gov/sandia-sdr/gnuradio_common/gr-sandia_utils/issues/16
BOOST_AUTO_TEST_CASE(t7)
{
    // generate a file source without a filename so that data is never
    // generated
    gr::sandia_utils::file_source::sptr source(gr::sandia_utils::file_source::make(
        sizeof(gr_complex) * 1, "", "raw", false, false));
    source->set_begin_tag(pmt::PMT_NIL);
    source->add_file_tags(false);
    source->set_file_queue_depth(1);

    // setup sink to generate multiple files of 4000 samples each
    gr::sandia_utils::file_sink::sptr sink(
        gr::sandia_utils::file_sink::make("complex",
                                          sizeof(gr_complex),
                                          "raw",
                                          gr::sandia_utils::MANUAL,
                                          4000,
                                          1000,
                                          "/tmp",
                                          "t_%02fd.fc32"));
    sink->set_recording(true);
    sink->set_second_align(false);
    sink->set_gen_new_folder(false);

    // create top block
    // start and stop immediately so all buffers can be initialized properly
    // but the work function is not called when the input buffer is updated
    gr::top_block_sptr tb(gr::make_top_block("t7"));
    tb->connect(source, 0, sink, 0);
    tb->start();
    tb->stop();
    tb->wait();

    // get buffer pointers
    gr::buffer_sptr inbuf(source->detail()->output(0));
    gr::buffer_reader_sptr reader = sink->detail()->input(0);

    // vector pointers for input and output
    gr_vector_const_void_star input_items(1);
    gr_vector_void_star output_items(1);

    // update buffer with "data"
    inbuf->update_write_pointer(4000);

    // add tag to first item
    gr::tag_t rx_time_tag, rx_rate_tag, rx_freq_tag;
    rx_time_tag.key = gr::sandia_utils::RATE_KEY;
    rx_time_tag.value = pmt::from_double(30.72e6);
    rx_time_tag.offset = 0;
    inbuf->add_item_tag(rx_time_tag);

    // add a second tag that will be on the first sample of the second
    // work function call.  the curent file (t_01.fc32) should be closed and
    // a second file opened
    rx_time_tag.offset = 1000;
    inbuf->add_item_tag(rx_time_tag);

    // try to call work function
    input_items[0] = reader->read_pointer();
    output_items[0] = NULL; // no output for block
    int noutput_items = sink->work(1000, input_items, output_items);
    BOOST_REQUIRE_EQUAL(noutput_items, 1000);
    reader->update_read_pointer(noutput_items);

    // call work function again so second tag is consumed
    input_items[0] = reader->read_pointer();
    output_items[0] = NULL; // no output for block
    noutput_items = sink->work(1000, input_items, output_items);
    BOOST_REQUIRE_EQUAL(noutput_items, 0);
    reader->update_read_pointer(noutput_items);

    // last work function call should have returned zero
    input_items[0] = reader->read_pointer();
    output_items[0] = NULL; // no output for block
    noutput_items = sink->work(1000, input_items, output_items);
    BOOST_REQUIRE_EQUAL(noutput_items, 1000);
    reader->update_read_pointer(noutput_items);

    // check
    BOOST_REQUIRE_EQUAL(sink->stop(), true);

    // remove files
    remove_file("/tmp/t_00.fc32"); // never actually written due to first stop call
    BOOST_REQUIRE_EQUAL(remove_file("/tmp/t_01.fc32"), true);
    BOOST_REQUIRE_EQUAL(remove_file("/tmp/t_02.fc32"), true);
}

} // namespace sandia_utils
} // namespace gr
