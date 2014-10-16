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

#include "pherialize/Mixed.hpp"



namespace pherialize {


Mixed::Mixed() {

	m_type = TYPE_NULL;
}


Mixed::Mixed(const std::string &v) {

	m_type = TYPE_STRING;
	m_value.stringValue = new std::string(v);
}


Mixed::Mixed(const char *v) {

	m_type = TYPE_STRING;
	m_value.stringValue = new std::string(v);
}


Mixed::Mixed(const int v) {

	m_type = TYPE_INT;
	m_value.intValue = v;
}


Mixed::Mixed(const bool v) {

	m_type = TYPE_BOOL;
	m_value.boolValue = v;
}


Mixed::Mixed(const double v) {

	m_type = TYPE_DOUBLE;
	m_value.doubleValue = v;
}


Mixed::Mixed(const MixedArray &v) {

	m_type = TYPE_ARRAY;
	m_value.arrayValue = new MixedArray(v);
}


Mixed::Mixed(const Mixed &v) {

	copyValue(v);
}


Mixed::~Mixed() {

	deleteValue(m_type, m_value);
}


void Mixed::copyValue(const Mixed &v) {

	m_type = v.m_type;

	switch (m_type) {
		case TYPE_STRING:

			m_value.stringValue = new std::string(*v.m_value.stringValue);
			break;

		case TYPE_ARRAY:

			m_value.arrayValue = new MixedArray(*v.m_value.arrayValue);
			break;

		case TYPE_INT:

			m_value.intValue = v.m_value.intValue;
			break;

		case TYPE_BOOL:

			m_value.boolValue = v.m_value.boolValue;
			break;

		case TYPE_DOUBLE:

			m_value.doubleValue = v.m_value.doubleValue;
			break;

		case TYPE_NULL:

			break;
	}
}


void Mixed::deleteValue(const Type type, ValueType &value) {

	switch (type) {
		case TYPE_STRING:

			delete value.stringValue;
			break;

		case TYPE_ARRAY:

			delete value.arrayValue;
			break;

		case TYPE_NULL:
		case TYPE_INT:
		case TYPE_BOOL:
		case TYPE_DOUBLE:

			break;
	}
}


Mixed &Mixed::operator=(const Mixed &v) {

	const Type oldType = m_type;
	ValueType oldValue = m_value;

	copyValue(v);
	deleteValue(oldType, oldValue);

	return *this;
}


bool Mixed::operator==(const Mixed &v) const {

	if (m_type != v.m_type) {
		return false;
	}

	switch (m_type) {
		case TYPE_STRING:

			return *m_value.stringValue == *v.m_value.stringValue;

		case TYPE_ARRAY:

			return *m_value.arrayValue == *v.m_value.arrayValue;

		case TYPE_NULL:

			return true;

		case TYPE_INT:

			return m_value.intValue == v.m_value.intValue;

		case TYPE_BOOL:

			return m_value.boolValue == v.m_value.boolValue;

		case TYPE_DOUBLE:

			return m_value.doubleValue == v.m_value.doubleValue;
	}

	return false;
}


bool Mixed::operator!=(const Mixed &v) const {

	return !(*this == v);
}


bool Mixed::operator<(const Mixed &v) const {

	if (m_type != v.m_type) {
		return m_type < v.m_type;
	}

	switch (m_type) {
		case TYPE_STRING:

			return *m_value.stringValue < *v.m_value.stringValue;

		case TYPE_ARRAY:

			return false;

		case TYPE_NULL:

			return false;

		case TYPE_INT:

			return m_value.intValue < v.m_value.intValue;

		case TYPE_BOOL:

			return m_value.boolValue < v.m_value.boolValue;

		case TYPE_DOUBLE:

			return m_value.doubleValue < v.m_value.doubleValue;
	}
}


Mixed::Type Mixed::type() const {
	return m_type;
}


bool Mixed::isNull() const {
	return m_type == TYPE_NULL;
}


const std::string &Mixed::stringValue() const {
	if (m_type != TYPE_STRING) {
		throw std::runtime_error("Invalid value type for 'string'.");
	}
	return *m_value.stringValue;
}


int Mixed::intValue() const {
	if (m_type != TYPE_INT) {
		throw std::runtime_error("Invalid value type for 'int'.");
	}
	return m_value.intValue;
}


bool Mixed::boolValue() const {
	if (m_type != TYPE_BOOL) {
		throw std::runtime_error("Invalid value type for 'bool'.");
	}
	return m_value.boolValue;
}


double Mixed::doubleValue() const {
	if (m_type != TYPE_DOUBLE) {
		throw std::runtime_error("Invalid value type for 'double'.");
	}
	return m_value.doubleValue;
}


const MixedArray &Mixed::arrayValue() const {
	if (m_type != TYPE_ARRAY) {
		throw std::runtime_error("Invalid value type for 'array'.");
	}
	return *m_value.arrayValue;
}


} // namespace pherialize
