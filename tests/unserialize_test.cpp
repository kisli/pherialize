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

	BOOST_CHECK(m->type() == Mixed::TYPE_STRING);
	BOOST_CHECK(m->stringValue() == "test string");
}


BOOST_AUTO_TEST_CASE(unserializeInteger) {

	shared_ptr <Mixed> m = unserialize("i:4242;");

	BOOST_CHECK(m->type() == Mixed::TYPE_INT);
	BOOST_CHECK(m->intValue() == 4242);
}


BOOST_AUTO_TEST_CASE(unserializeBool) {

	shared_ptr <Mixed> m1 = unserialize("b:1;");

	BOOST_CHECK(m1->type() == Mixed::TYPE_BOOL);
	BOOST_CHECK(m1->boolValue() == true);

	shared_ptr <Mixed> m2 = unserialize("b:0;");

	BOOST_CHECK(m2->type() == Mixed::TYPE_BOOL);
	BOOST_CHECK(m2->boolValue() == false);
}


BOOST_AUTO_TEST_CASE(unserializeDouble) {

	shared_ptr <Mixed> m = unserialize("d:44.83834308566653;");

	BOOST_CHECK(m->type() == Mixed::TYPE_DOUBLE);
	BOOST_CHECK_CLOSE(m->doubleValue(), 44.83834308566653, 0.000001);
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

	BOOST_CHECK(m1->type() == Mixed::TYPE_ARRAY);

	const MixedArray &marr1 = m1->arrayValue();

	BOOST_CHECK(marr1.type() == MixedArray::TYPE_MAP);
	BOOST_CHECK(marr1.mapValue().size() == 2);
	BOOST_CHECK(readMap(marr1.mapValue(), "ab") == "cd");
	BOOST_CHECK(readMap(marr1.mapValue(), "ef") == "gh");
}


BOOST_AUTO_TEST_CASE(unserializeVector) {

	// Vector with 2 elements
	shared_ptr <Mixed> m1 = unserialize("a:2:{i:0;s:2:\"ab\";i:1;s:2:\"cd\";}");

	BOOST_CHECK(m1->type() == Mixed::TYPE_ARRAY);

	const MixedArray &marr1 = m1->arrayValue();

	BOOST_CHECK(marr1.type() == MixedArray::TYPE_VECTOR);
	BOOST_CHECK(marr1.vectorValue().size() == 2);
	BOOST_CHECK(marr1.vectorValue()[0] == "ab");
	BOOST_CHECK(marr1.vectorValue()[1] == "cd");

	// Empty vector
	shared_ptr <Mixed> m2 = unserialize("a:0:{}");

	BOOST_CHECK(m2->type() == Mixed::TYPE_ARRAY);

	const MixedArray &marr2 = m2->arrayValue();

	BOOST_CHECK(marr2.type() == MixedArray::TYPE_VECTOR);
	BOOST_CHECK(marr2.vectorValue().size() == 0);

	// Vector in vector
	shared_ptr <Mixed> m3 = unserialize("a:1:{i:0;a:1:{i:0;i:42;}}");

	BOOST_CHECK(m3->type() == Mixed::TYPE_ARRAY);

	const MixedArray &marr3_1 = m3->arrayValue();

	BOOST_CHECK(marr3_1.type() == MixedArray::TYPE_VECTOR);
	BOOST_CHECK(marr3_1.vectorValue().size() == 1);
	BOOST_CHECK(marr3_1.vectorValue()[0].type() == Mixed::TYPE_ARRAY);

	const MixedArray &marr3_2 = marr3_1.vectorValue()[0].arrayValue();

	BOOST_CHECK(marr3_2.type() == MixedArray::TYPE_VECTOR);
	BOOST_CHECK(marr3_2.vectorValue().size() == 1);
	BOOST_CHECK(marr3_2.vectorValue()[0].type() == Mixed::TYPE_INT);
	BOOST_CHECK(marr3_2.vectorValue()[0].intValue() == 42);
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
