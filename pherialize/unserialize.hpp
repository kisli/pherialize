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

#ifndef PHERIALIZE_UNSERIALIZE_HPP_INCLUDED
#define PHERIALIZE_UNSERIALIZE_HPP_INCLUDED


#include "pherialize/types.hpp"
#include "pherialize/export.hpp"

#include "pherialize/Mixed.hpp"
#include "pherialize/MixedArray.hpp"

#include <string>
#include <cstddef>


namespace pherialize {


/** Unserializes a PHP-serialize()d string to a mixed value.
  */
class PHERIALIZE_EXPORT Unserializer {

public:

	/** Constructs a new unserializer from a character string.
	  *
	  * @param data string containing serialized data
	  */
	Unserializer(const std::string &data);

	/** Unserializes the next object from this data stream.
	  *
	  * @throw std::runtime_error if a parsing error occurs
	  * @return a Mixed object, or NULL if no object can be
	  * read from the stream
	  */
	shared_ptr <Mixed> unserializeObject();

private:

	shared_ptr <Mixed> unserializeNull();
	shared_ptr <Mixed> unserializeInt();
	shared_ptr <Mixed> unserializeBool();
	shared_ptr <Mixed> unserializeDouble();
	shared_ptr <Mixed> unserializeString();
	shared_ptr <Mixed> unserializeArray();
	shared_ptr <Mixed> unserializeObjectToArray();

	std::string m_data;
	std::size_t m_pos;
	std::size_t m_length;
};


/** Unserializes an object directly from a character string.
  *
  * @throw std::runtime_error if a parsing error occurs
  * @return a Mixed object, or NULL if no object can be read
  * from the stream
  */
PHERIALIZE_EXPORT shared_ptr <Mixed> unserialize(const std::string &str);


} // namespace pherialize


#endif // PHERIALIZE_UNSERIALIZE_HPP_INCLUDED
