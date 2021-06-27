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
/* BINDTOOL_HEADER_FILE(complex_to_interleaved_short.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(c3152d1f278090b55b671fdae62e5f0a)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <sandia_utils/complex_to_interleaved_short.h>
// pydoc.h is automatically generated in the build directory
#include <complex_to_interleaved_short_pydoc.h>

void bind_complex_to_interleaved_short(py::module& m)
{

    using complex_to_interleaved_short    = gr::sandia_utils::complex_to_interleaved_short;


    py::class_<complex_to_interleaved_short,
        gr::sync_interpolator,
        gr::sync_block,
        gr::block,
        gr::basic_block,
        std::shared_ptr<complex_to_interleaved_short>>(m, "complex_to_interleaved_short", D(complex_to_interleaved_short))

        .def(py::init(&complex_to_interleaved_short::make),
           py::arg("vector") =  false,
           py::arg("scale") =  1.0,
           D(complex_to_interleaved_short,make)
        )




        ;




}







