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

#include "pherialize/MixedArray.hpp"
#include "pherialize/Mixed.hpp"



namespace pherialize {


MixedArray::MixedArray() {

	m_type = TYPE_NONE;
}


MixedArray::MixedArray(const std::vector <Mixed> &v) {

	m_type = TYPE_VECTOR;
	m_value.vector = new std::vector <Mixed>(v);
}


MixedArray::MixedArray(const std::map <Mixed, Mixed> &v) {

	m_type = TYPE_MAP;
	m_value.map = new std::map <Mixed, Mixed>(v);
}


MixedArray::MixedArray(const MixedArray &v) {

	m_type = v.m_type;

	switch (m_type) {
		case TYPE_VECTOR:

			m_value.vector = new std::vector <Mixed>(*v.m_value.vector);
			break;

		case TYPE_MAP:

			m_value.map = new std::map <Mixed, Mixed>(*v.m_value.map);
			break;
	}
}


MixedArray::~MixedArray() {

	switch (m_type) {
		case TYPE_VECTOR:

			delete m_value.vector;
			break;

		case TYPE_MAP:

			delete m_value.map;
			break;
	}
}


bool MixedArray::operator==(const MixedArray &v) const {

	if (m_type != v.m_type) {
		return false;
	}

	switch (m_type) {
		case TYPE_NONE:

			return true;

		case TYPE_VECTOR:

			return *m_value.vector == *v.m_value.vector;

		case TYPE_MAP:

			return *m_value.map == *v.m_value.map;
	}

	return false;
}


bool MixedArray::operator!=(const MixedArray &v) const {
	return !(*this == v);
}

MixedArray::Type MixedArray::type() const {
	return m_type;
}


const std::vector <Mixed> &MixedArray::vectorValue() const {
	if (m_type != TYPE_VECTOR) {
		throw std::runtime_error("Invalid value type for 'vector'.");
	}
	return *m_value.vector;
}

const std::map <Mixed, Mixed> &MixedArray::mapValue() const {
	// TODO: handle conversion from vector to map?
	if (m_type != TYPE_MAP) {
		throw std::runtime_error("Invalid value type for 'map'.");
	}
	return *m_value.map;
}


} // namespace pherialize
