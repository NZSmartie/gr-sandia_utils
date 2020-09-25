/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "tag_debug_file_impl.h"
#include <gnuradio/io_signature.h>
#include <fstream>
#include <iomanip>
#include <iostream>

namespace gr {
namespace sandia_utils {

tag_debug_file::sptr tag_debug_file::make(size_t sizeof_stream_item,
                                          const std::string& name,
                                          const std::string& key_filter,
                                          const std::string& filename)
{
    return gnuradio::get_initial_sptr(
        new tag_debug_file_impl(sizeof_stream_item, name, key_filter, filename));
}

/*
 * The private constructor
 */
tag_debug_file_impl::tag_debug_file_impl(size_t sizeof_stream_item,
                                         const std::string& name,
                                         const std::string& key_filter,
                                         const std::string& filename)
    : gr::sync_block("tag_debug_file",
                     io_signature::make(1, -1, sizeof_stream_item),
                     io_signature::make(0, 0, 0)),
      d_name(name),
      d_filename(filename),
      d_display(true)
{
    set_key_filter(key_filter);
}

/*
 * Our virtual destructor.
 */
tag_debug_file_impl::~tag_debug_file_impl() {}

std::vector<tag_t> tag_debug_file_impl::current_tags()
{
    gr::thread::scoped_lock l(d_mutex);
    return d_tags;
}

int tag_debug_file_impl::num_tags()
{
    std::vector<tag_t> t;
    get_tags_in_range(t, 0, 0, nitems_read(0));
    return static_cast<int>(t.size());
}

void tag_debug_file_impl::set_display(bool d) { d_display = d; }

void tag_debug_file_impl::set_filewrite(bool w, bool clear = false)
{
    d_write_file = w;

    if (clear) {
        const char* fname(d_filename.c_str());
        std::ofstream out(fname, std::ios::binary | std::ios::out);
        out << "";
        out.flush();
        out.close();
    }
}

void tag_debug_file_impl::set_key_filter(const std::string& key_filter)
{
    if (key_filter == "")
        d_filter = pmt::PMT_NIL;
    else
        d_filter = pmt::intern(key_filter);
}

std::string tag_debug_file_impl::key_filter() const
{
    return pmt::symbol_to_string(d_filter);
}

int tag_debug_file_impl::work(int noutput_items,
                              gr_vector_const_void_star& input_items,
                              gr_vector_void_star& output_items)
{
    gr::thread::scoped_lock l(d_mutex);
    bool toprint = false;

    std::stringstream sout;
    if (d_display) {
        sout << std::endl
             << "----------------------------------------------------------------------";
        sout << std::endl << "Tag Debug: " << d_name << std::endl;
    }

    uint64_t abs_N, end_N;
    for (size_t i = 0; i < input_items.size(); i++) {
        abs_N = nitems_read(i);
        end_N = abs_N + (uint64_t)(noutput_items);

        d_tags.clear();
        if (pmt::is_null(d_filter))
            get_tags_in_range(d_tags, i, abs_N, end_N);
        else
            get_tags_in_range(d_tags, i, abs_N, end_N, d_filter);

        if (d_tags.size() > 0) {
            toprint = true;
        }

        if (d_display || d_write_file) {
            sout << "Input Stream: " << std::setw(2) << std::setfill('0') << i
                 << std::setfill(' ') << std::endl;
            for (d_tags_itr = d_tags.begin(); d_tags_itr != d_tags.end(); d_tags_itr++) {
                sout << std::setw(10) << "Offset: " << d_tags_itr->offset << std::setw(10)
                     << "Source: "
                     << (pmt::is_symbol(d_tags_itr->srcid)
                             ? pmt::symbol_to_string(d_tags_itr->srcid)
                             : "n/a")
                     << std::setw(10) << "Key: " << pmt::symbol_to_string(d_tags_itr->key)
                     << std::setw(10) << "Value: ";
                sout << d_tags_itr->value << std::endl;
            }
        }
    }

    if (toprint) {
        sout << "----------------------------------------------------------------------";
        sout << std::endl;

        if (d_display) {
            std::cout << sout.str();
        }

        if (d_write_file) {
            const char* fname(d_filename.c_str());
            std::string outstr(sout.str());
            std::ofstream out(fname, std::ios::binary | std::ios::app | std::ios::out);
            out << outstr;
            out.flush();
            out.close();
        }
    }

    return noutput_items;
}

void tag_debug_file_impl::setup_rpc()
{
#ifdef GR_CTRLPORT
    add_rpc_variable(rpcbasic_sptr(
        new rpcbasic_register_get<tag_debug_file, int>(alias(),
                                                       "num. tags",
                                                       &tag_debug_file::num_tags,
                                                       pmt::from_long(0),
                                                       pmt::from_long(10000),
                                                       pmt::from_long(0),
                                                       "",
                                                       "Number of Tags",
                                                       RPC_PRIVLVL_MIN,
                                                       DISPTIME | DISPOPTSTRIP)));
#endif /* GR_CTRLPORT */
}

} /* namespace sandia_utils */
} /* namespace gr */
