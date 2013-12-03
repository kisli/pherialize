//
// PHP-compatible unserializer for C++
//
// Copyright (C) 2012-2013 Kisli    http://www.kisli.com
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
//

#ifndef PHERIALIZE_EXPORT_HPP_INCLUDED
#define PHERIALIZE_EXPORT_HPP_INCLUDED


// Define PHERIALIZE_STATIC if you are linking with the static library
#ifdef PHERIALIZE_STATIC
#	include "pherialize/export-static.hpp"
#else
#	include "pherialize/export-shared.hpp"
#endif


#endif // PHERIALIZE_EXPORT_HPP_INCLUDED
