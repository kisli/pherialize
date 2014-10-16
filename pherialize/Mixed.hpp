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

#ifndef PHERIALIZE_MIXED_HPP_INCLUDED
#define PHERIALIZE_MIXED_HPP_INCLUDED


#include "pherialize/types.hpp"
#include "pherialize/export.hpp"

#include "pherialize/MixedArray.hpp"

#include <string>
#include <stdexcept>


namespace pherialize {


/** A mixed value (variant), as in PHP.
  */
class PHERIALIZE_EXPORT Mixed {

public:

	/** Possible types for value.
	  */
	enum Type {
		TYPE_NULL,
		TYPE_STRING,
		TYPE_INT,
		TYPE_BOOL,
		TYPE_DOUBLE,
		TYPE_ARRAY,
	};


	Mixed();
	Mixed(const std::string &v);
	Mixed(const char *v);
	Mixed(const int v);
	Mixed(const bool v);
	Mixed(const double v);
	Mixed(const MixedArray &v);

	Mixed(const Mixed &v);

	~Mixed();


	/** Returns the current type of the value.
	  */
	Type type() const;

	/** Returns whether this is the null value.
	  *
	  * @return true if the value is null, or false otherwise
	  */
	bool isNull() const;

	/** Returns the value as a string.
	  *
	  * @throw std::runtime_error if the stored value is not a string
	  * @return string value
	  */
	const std::string &stringValue() const;

	/** Returns the value as an int.
	  *
	  * @throw std::runtime_error if the stored value is not an int
	  * @return int value
	  */
	int intValue() const;

	/** Returns the value as a bool.
	  *
	  * @throw std::runtime_error if the stored value is not a bool
	  * @return bool value
	  */
	bool boolValue() const;

	/** Returns the value as a double.
	  *
	  * @throw std::runtime_error if the stored value is not a double
	  * @return double value
	  */
	double doubleValue() const;

	/** Returns the value as a mixed array.
	  *
	  * @throw std::runtime_error if the stored value is not an array
	  * @return array value
	  */
	const MixedArray &arrayValue() const;

	Mixed &operator=(const Mixed &v);
	bool operator==(const Mixed &v) const;
	bool operator!=(const Mixed &v) const;
	bool operator<(const Mixed &v) const;

private:

	union ValueType {
		std::string *stringValue;
		int intValue;
		bool boolValue;
		double doubleValue;
		MixedArray *arrayValue;
	};


	void copyValue(const Mixed &v);
	void deleteValue(const Type type, ValueType &value);


	Type m_type;
	ValueType m_value;
};


} // namespace pherialize


#endif // PHERIALIZE_MIXED_HPP_INCLUDED
