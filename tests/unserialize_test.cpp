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

#define BOOST_TEST_MODULE pherialize_unserialize test
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "pherialize/unserialize.hpp"


using namespace pherialize;


BOOST_AUTO_TEST_CASE(unserializeNull) {

	shared_ptr <Mixed> m = unserialize("");

	BOOST_CHECK(m == NULL);
}


BOOST_AUTO_TEST_CASE(unserializeString) {

	shared_ptr <Mixed> m = unserialize("s:11:\"test string\";");

	BOOST_CHECK_EQUAL(Mixed::TYPE_STRING, m->type());
	BOOST_CHECK_EQUAL("test string", m->stringValue());
}


BOOST_AUTO_TEST_CASE(unserializeInteger) {

	shared_ptr <Mixed> m = unserialize("i:4242;");

	BOOST_CHECK_EQUAL(Mixed::TYPE_INT, m->type());
	BOOST_CHECK_EQUAL(4242, m->intValue());
}


BOOST_AUTO_TEST_CASE(unserializeBool) {

	shared_ptr <Mixed> m1 = unserialize("b:1;");

	BOOST_CHECK_EQUAL(Mixed::TYPE_BOOL, m1->type());
	BOOST_CHECK_EQUAL(true, m1->boolValue());

	shared_ptr <Mixed> m2 = unserialize("b:0;");

	BOOST_CHECK_EQUAL(Mixed::TYPE_BOOL, m2->type());
	BOOST_CHECK_EQUAL(false, m2->boolValue());
}


BOOST_AUTO_TEST_CASE(unserializeDouble) {

	shared_ptr <Mixed> m = unserialize("d:44.83834308566653;");

	BOOST_CHECK_EQUAL(Mixed::TYPE_DOUBLE, m->type());
	BOOST_CHECK_CLOSE(m->doubleValue(), 44.83834308566653, 0.000001);

	shared_ptr <Mixed> m2 = unserialize("d:-0.05;");

	BOOST_CHECK_EQUAL(Mixed::TYPE_DOUBLE, m2->type());
	BOOST_CHECK_CLOSE(m2->doubleValue(), -0.05, 0.000001);
}


const Mixed &readMap(const std::map <Mixed, Mixed> &map, const Mixed &key) {
	const std::map <Mixed, Mixed>::const_iterator it = map.find(key);
	if (it == map.end()) {
		throw std::out_of_range("Key not found");
	}
	return (*it).second;
}


BOOST_AUTO_TEST_CASE(unserializeMap) {

	shared_ptr <Mixed> m1 = unserialize("a:2:{s:2:\"ab\";s:2:\"cd\";s:2:\"ef\";s:2:\"gh\";}");

	BOOST_CHECK_EQUAL(Mixed::TYPE_ARRAY, m1->type());

	const MixedArray &marr1 = m1->arrayValue();

	BOOST_CHECK_EQUAL(MixedArray::TYPE_MAP, marr1.type());
	BOOST_CHECK_EQUAL(2, marr1.mapValue().size());
	BOOST_CHECK_EQUAL("cd", readMap(marr1.mapValue(), "ab").stringValue());
	BOOST_CHECK_EQUAL("gh", readMap(marr1.mapValue(), "ef").stringValue());
}


BOOST_AUTO_TEST_CASE(unserializeVector) {

	// Vector with 2 elements
	shared_ptr <Mixed> m1 = unserialize("a:2:{i:0;s:2:\"ab\";i:1;s:2:\"cd\";}");

	BOOST_CHECK_EQUAL(Mixed::TYPE_ARRAY, m1->type());

	const MixedArray &marr1 = m1->arrayValue();

	BOOST_CHECK_EQUAL(MixedArray::TYPE_VECTOR, marr1.type());
	BOOST_CHECK_EQUAL(2, marr1.vectorValue().size());
	BOOST_CHECK_EQUAL("ab", marr1.vectorValue()[0].stringValue());
	BOOST_CHECK_EQUAL("cd", marr1.vectorValue()[1].stringValue());

	// Empty vector
	shared_ptr <Mixed> m2 = unserialize("a:0:{}");

	BOOST_CHECK_EQUAL(Mixed::TYPE_ARRAY, m2->type());

	const MixedArray &marr2 = m2->arrayValue();

	BOOST_CHECK_EQUAL(MixedArray::TYPE_VECTOR, marr2.type());
	BOOST_CHECK_EQUAL(0, marr2.vectorValue().size());

	// Vector in vector
	shared_ptr <Mixed> m3 = unserialize("a:1:{i:0;a:1:{i:0;i:42;}}");

	BOOST_CHECK_EQUAL(Mixed::TYPE_ARRAY, m3->type());

	const MixedArray &marr3_1 = m3->arrayValue();

	BOOST_CHECK_EQUAL(MixedArray::TYPE_VECTOR, marr3_1.type());
	BOOST_CHECK_EQUAL(1, marr3_1.vectorValue().size());
	BOOST_CHECK_EQUAL(Mixed::TYPE_ARRAY, marr3_1.vectorValue()[0].type());

	const MixedArray &marr3_2 = marr3_1.vectorValue()[0].arrayValue();

	BOOST_CHECK_EQUAL(MixedArray::TYPE_VECTOR, marr3_2.type());
	BOOST_CHECK_EQUAL(1, marr3_2.vectorValue().size());
	BOOST_CHECK_EQUAL(Mixed::TYPE_INT, marr3_2.vectorValue()[0].type());
	BOOST_CHECK_EQUAL(42, marr3_2.vectorValue()[0].intValue());
}


BOOST_AUTO_TEST_CASE(unserializeInvalid) {

	// Incorrect string length
	BOOST_CHECK_THROW(
		unserialize("s:3:\"ab\";"),
		std::runtime_error
	);

	// Too much input data
	BOOST_CHECK_THROW(
		unserialize("i:42:1234;"),
		std::runtime_error
	);
}

