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
/* BINDTOOL_HEADER_FILE(tag_debug_file.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(b9353be4511d0d3b88b1732781b10825)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <sandia_utils/tag_debug_file.h>
// pydoc.h is automatically generated in the build directory
#include <tag_debug_file_pydoc.h>

void bind_tag_debug_file(py::module& m)
{

    using tag_debug_file    = gr::sandia_utils::tag_debug_file;


    py::class_<tag_debug_file,
        gr::sync_block,
        gr::block,
        gr::basic_block,
        std::shared_ptr<tag_debug_file>>(m, "tag_debug_file", D(tag_debug_file))

        .def(py::init(&tag_debug_file::make),
           py::arg("sizeof_stream_item"),
           py::arg("name"),
           py::arg("key_filter"),
           py::arg("filename"),
           D(tag_debug_file,make)
        )




        ;




}








