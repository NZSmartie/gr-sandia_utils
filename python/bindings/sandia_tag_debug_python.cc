/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(sandia_tag_debug.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(0274b64a97497ed79b5fb631eba51f9a)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <sandia_utils/sandia_tag_debug.h>
// pydoc.h is automatically generated in the build directory
#include <sandia_tag_debug_pydoc.h>

void bind_sandia_tag_debug(py::module& m)
{

    using sandia_tag_debug    = gr::sandia_utils::sandia_tag_debug;


    py::class_<sandia_tag_debug,
        gr::sync_block,
        gr::block,
        gr::basic_block,
        std::shared_ptr<sandia_tag_debug>>(m, "sandia_tag_debug", D(sandia_tag_debug))

        .def(py::init(&sandia_tag_debug::make),
           py::arg("sizeof_stream_item"),
           py::arg("name"),
           py::arg("key_filter") =  "",
           py::arg("store_all") =  true,
           D(sandia_tag_debug,make)
        )



        .def("current_tags", &sandia_tag_debug::current_tags, D(sandia_tag_debug, current_tags))

        .def("get_tag", &sandia_tag_debug::get_tag, py::arg("id"), D(sandia_tag_debug, get_tag))

        .def("clear_tags", &sandia_tag_debug::clear_tags, D(sandia_tag_debug, clear_tags))

        .def("num_tags", &sandia_tag_debug::num_tags, D(sandia_tag_debug, num_tags))

        .def("set_display", &sandia_tag_debug::set_display, py::arg("d"), D(sandia_tag_debug, set_display))

        .def("set_key_filter", &sandia_tag_debug::set_key_filter, py::arg("key_filter"), D(sandia_tag_debug, set_key_filter))

        .def("key_filter", &sandia_tag_debug::key_filter, D(sandia_tag_debug, key_filter))

        ;




}








