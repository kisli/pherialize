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

#include "pherialize/unserialize.hpp"

#include <stdexcept>
#include <cstdlib>
#include <istream>
#include <sstream>

#include <iostream>
#include <boost/format.hpp>


namespace pherialize {


Unserializer::Unserializer(const std::string &data) {

	m_pos = 0;
	m_length = data.length();
	m_data = data;
}


shared_ptr <Mixed> Unserializer::unserializeObject() {

	char type = m_data[m_pos];

	switch (type) {

		case 's':

			++m_pos;
			return unserializeString();

		case 'i':

			++m_pos;
			return unserializeInt();

		case 'a':

			++m_pos;
			return unserializeArray();

		case 'O':

			++m_pos;
			return unserializeObjectToArray();

		case 'N':

			++m_pos;
			return unserializeNull();

		case 'b':

			++m_pos;
			return unserializeBool();

		case 'd':

			++m_pos;
			return unserializeDouble();

		case '\0':

			return shared_ptr <Mixed>();
	}

	throw std::runtime_error(
		(boost::format("Unable to unserialize unknown type '%1%'.") % type).str()
	);
}


shared_ptr <Mixed> Unserializer::unserializeNull() {

	char *charAfterNumber;
	const std::size_t number = std::strtol(m_data.data() + m_pos, &charAfterNumber, /* base */ 10);
	m_pos = charAfterNumber - m_data.data();

	if (m_data[m_pos] != ';')
		throw std::runtime_error("Expected ';'.");
	else
		m_pos++;  // skip ';'

	return make_shared <Mixed>(false);
}


shared_ptr <Mixed> Unserializer::unserializeInt() {

	if (m_data[m_pos] != ':') {
		throw std::runtime_error("Expected ':'.");
	}

	char *charAfterNumber;
	const std::size_t number = std::strtol(m_data.data() + m_pos + 1, &charAfterNumber, /* base */ 10);

	m_pos = charAfterNumber - m_data.data();

	if (m_data[m_pos] != ';')
		throw std::runtime_error("Expected ';'.");
	else
		m_pos++;  // skip ';'

	return make_shared <Mixed>(static_cast <int>(number));
}


shared_ptr <Mixed> Unserializer::unserializeBool() {

	if (m_data[m_pos] != ':') {
		throw std::runtime_error("Expected ':'.");
	}

	char *charAfterNumber;
	const std::size_t number = std::strtol(m_data.data() + m_pos + 1, &charAfterNumber, /* base */ 10);

	m_pos = charAfterNumber - m_data.data();

	if (m_data[m_pos] != ';')
		throw std::runtime_error("Expected ';'.");
	else
		m_pos++;  // skip ';'

	return make_shared <Mixed>(number ? true : false);
}


shared_ptr <Mixed> Unserializer::unserializeDouble() {

	if (m_data[m_pos] != ':') {
		throw std::runtime_error("Expected ':'.");
	}

	const char *numberStart = m_data.data() + m_pos + 1;
	const char *numberEnd = numberStart;

	while (numberEnd < m_data.data() + m_length &&
	       (*numberEnd == '.' || *numberEnd == '-' || *numberEnd >= '0' && *numberEnd <= '9')) {
		++numberEnd;
	}

	const std::string numberStr(numberStart, numberEnd);
	std::istringstream is(numberStr);

	double number;

	if (!(is >> number)) {
		throw std::runtime_error(
			(boost::format("Invalid format for double: '%1%'.") % numberStr).str()
		);
	}

	m_pos = numberEnd - m_data.data();

	if (m_data[m_pos] != ';')
		throw std::runtime_error("Expected ';'.");
	else
		m_pos++;  // skip ';'

	return make_shared <Mixed>(number);
}


shared_ptr <Mixed> Unserializer::unserializeString() {

	if (m_data[m_pos] != ':') {
		throw std::runtime_error("Expected ':'.");
	}

	char *charAfterLen;
	const std::size_t len = std::strtol(m_data.data() + m_pos + 1, &charAfterLen, /* base */ 10);

	if (*charAfterLen != ':') {
		throw std::runtime_error("Expected ':'.");
	}

	++charAfterLen;  // skip ':'

	if (charAfterLen + 2 /* "..." */ + len > m_data.data() + m_length) {
		throw std::runtime_error("Invalid string length.");
	}

	m_pos = (charAfterLen + 2 + len) - m_data.data();

	if (m_data[m_pos] != ';')
		throw std::runtime_error("Expected ';'.");
	else
		m_pos++;  // skip ';'

	return make_shared <Mixed>(std::string(charAfterLen + 1, charAfterLen + 1 + len));
}


shared_ptr <Mixed> Unserializer::unserializeObjectToArray() {

	if (m_data[m_pos] != ':') {
		throw std::runtime_error("Expected ':'.");
	}

	char *charAfterCount;
	const std::size_t count1 = std::strtol(m_data.data() + m_pos + 1, &charAfterCount, /* base */ 10);
	const std::size_t count = std::strtol(m_data.data() + m_pos + 1 + count1 + 5, &charAfterCount, /* base */ 10);

	if (*charAfterCount != ':') {
		throw std::runtime_error("Expected ':'.");
	}

	++charAfterCount;  // skip ':'

	if (*charAfterCount != '{') {
		throw std::runtime_error("Expected '{'.");
	}

	++charAfterCount;  // skip '{'

	m_pos = charAfterCount - m_data.data();

	// Parse elements
	std::map <Mixed, Mixed> map;
	std::vector <Mixed> array;

	bool allKeysAreInteger = true;
	bool allKeysAreConsecutive = true;
	int previousKey = -1;

	while (m_data[m_pos] != '\0') {

		if (m_data[m_pos] == '}') {
			m_pos++;
			break;
		}

		shared_ptr <Mixed> key = unserializeObject();

		if (key->type() != Mixed::TYPE_INT) {
			allKeysAreInteger = false;
		} else if (key->intValue() != previousKey + 1) {
			allKeysAreConsecutive = false;
		} else {
			previousKey++;
		}

		shared_ptr <Mixed> value = unserializeObject();

		array.push_back(*value);
		map.insert(std::map <Mixed, Mixed>::value_type(*key, *value));
	}

	if (allKeysAreInteger && allKeysAreConsecutive) {
		return make_shared <Mixed>(array);
	} else {
		return make_shared <Mixed>(map);
	}
}


shared_ptr <Mixed> Unserializer::unserializeArray() {

	if (m_data[m_pos] != ':') {
		throw std::runtime_error("Expected ':'.");
	}

	char *charAfterCount;
	const std::size_t count = std::strtol(m_data.data() + m_pos + 1, &charAfterCount, /* base */ 10);

	if (*charAfterCount != ':') {
		throw std::runtime_error("Expected ':'.");
	}

	++charAfterCount;  // skip ':'

	if (*charAfterCount != '{') {
		throw std::runtime_error("Expected '{'.");
	}

	++charAfterCount;  // skip '{'

	m_pos = charAfterCount - m_data.data();

	// Parse elements
	std::map <Mixed, Mixed> map;
	std::vector <Mixed> array;

	bool allKeysAreInteger = true;
	bool allKeysAreConsecutive = true;
	int previousKey = -1;

	while (m_data[m_pos] != '\0') {

		if (m_data[m_pos] == '}') {
			m_pos++;
			break;
		}

		shared_ptr <Mixed> key = unserializeObject();

		if (key->type() != Mixed::TYPE_INT) {
			allKeysAreInteger = false;
		} else if (key->intValue() != previousKey + 1) {
			allKeysAreConsecutive = false;
		} else {
			previousKey++;
		}

		shared_ptr <Mixed> value = unserializeObject();

		array.push_back(*value);
		map.insert(std::map <Mixed, Mixed>::value_type(*key, *value));
	}

	if (allKeysAreInteger && allKeysAreConsecutive) {
		return make_shared <Mixed>(array);
	} else {
		return make_shared <Mixed>(map);
	}
}


shared_ptr <Mixed> unserialize(const std::string &str) {

	Unserializer un(str);
	shared_ptr <Mixed> val = un.unserializeObject();

	if (un.unserializeObject()) {
		throw std::runtime_error("Expected end of data.");
	}

	return val;
}


} // namespace pherialize
