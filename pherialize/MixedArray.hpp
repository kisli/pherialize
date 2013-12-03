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

#ifndef PHERIALIZE_MIXEDARRAY_HPP_INCLUDED
#define PHERIALIZE_MIXEDARRAY_HPP_INCLUDED


#include "pherialize/types.hpp"
#include "pherialize/export.hpp"

#include <vector>
#include <map>
#include <stdexcept>


namespace pherialize {


class Mixed;


/** An array or map containing mixed values.
  */
class PHERIALIZE_EXPORT MixedArray {

public:

	/** Possible types of array.
	  */
	enum Type {
		TYPE_NONE,
		TYPE_VECTOR,
		TYPE_MAP,
	};


	MixedArray();
	MixedArray(const std::vector <Mixed> &v);
	MixedArray(const std::map <Mixed, Mixed> &v);

	MixedArray(const MixedArray &v);

	~MixedArray();


	/** Returns the actual type of array.
	  *
	  * @return array type
	  */
	Type type() const;

	/** Returns the value as a vector.
	  *
	  * @throw std::runtime_error if the stored value is not a vector
	  * @return vector value
	  */
	const std::vector <Mixed> &vectorValue() const;

	/** Returns the value as a map.
	  *
	  * @throw std::runtime_error if the stored value is not a map
	  * @return map value
	  */
	const std::map <Mixed, Mixed> &mapValue() const;


	bool operator==(const MixedArray &v) const;
	bool operator!=(const MixedArray &v) const;

private:

	MixedArray &operator=(const MixedArray &v);


	union ValueType {
		std::vector <Mixed> *vector;
		std::map <Mixed, Mixed> *map;
	};

	Type m_type;
	ValueType m_value;
};


} // namespace pherialize


#endif // PHERIALIZE_MIXEDARRAY_HPP_INCLUDED
