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

#define BOOST_TEST_MODULE pherialize_Mixed test
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "pherialize/Mixed.hpp"


using namespace pherialize;


BOOST_AUTO_TEST_CASE(Mixed_constructors) {

	// Mixed()
	Mixed m0;
	BOOST_CHECK(m0.type() == Mixed::TYPE_NULL);
	BOOST_CHECK(m0.isNull());

	// Mixed(const char *v)
	const char *str1 = "test string";
	Mixed m1(str1);
	BOOST_CHECK(m1.type() == Mixed::TYPE_STRING);
	BOOST_CHECK(m1.stringValue() == str1);

	// Mixed(const std::string &v)
	const std::string &str2 = "test std::string";
	Mixed m2(str2);
	BOOST_CHECK(m2.type() == Mixed::TYPE_STRING);
	BOOST_CHECK(m2.stringValue() == str2);

	// Mixed(const int v)
	const int i = 42;
	Mixed m3(i);
	BOOST_CHECK(m3.type() == Mixed::TYPE_INT);
	BOOST_CHECK(m3.intValue() == i);

	// Mixed(const bool v)
	const bool b = true;
	Mixed m4(b);
	BOOST_CHECK(m4.type() == Mixed::TYPE_BOOL);
	BOOST_CHECK(m4.boolValue() == b);

	// Mixed(const MixedArray &v)
	std::vector <Mixed> arr;
	const MixedArray marr(arr);
	Mixed m5(marr);
	BOOST_CHECK(m5.type() == Mixed::TYPE_ARRAY);
	BOOST_CHECK(m5.arrayValue() == arr);

	// Mixed(const Mixed &v)
	Mixed m6(m5);
	BOOST_CHECK(m6.type() == m5.type());
	BOOST_CHECK(m6 == m5);
}


BOOST_AUTO_TEST_CASE(Mixed_operator_eq) {

	// String
	Mixed m0_1("test string");
	Mixed m0_2(std::string("test string"));
	BOOST_CHECK(m0_1 == m0_2);

	// Int
	Mixed m1_1(42);
	Mixed m1_2(42);
	BOOST_CHECK(m1_1 == m1_2);

	// Bool
	Mixed m2_1(true);
	Mixed m2_2(true);
	BOOST_CHECK(m2_1 == m2_2);

	// Array
	std::vector <Mixed> arr1;
	arr1.push_back(Mixed(42));
	const MixedArray marr1(arr1);

	std::vector <Mixed> arr2;
	arr2.push_back(Mixed(42));
	const MixedArray marr2(arr2);

	const Mixed m3_1(marr1);
	const Mixed m3_2(marr2);
	BOOST_CHECK(m3_1 == m3_2);
}


BOOST_AUTO_TEST_CASE(Mixed_operator_neq) {

	// String
	Mixed m0_1("test string");
	Mixed m0_2(std::string("test string 2"));
	BOOST_CHECK(m0_1 != m0_2);

	// Int
	Mixed m1_1(42);
	Mixed m1_2(4242);
	BOOST_CHECK(m1_1 != m1_2);

	// Bool
	Mixed m2_1(true);
	Mixed m2_2(false);
	BOOST_CHECK(m2_1 != m2_2);

	// Array
	std::vector <Mixed> arr1;
	arr1.push_back(Mixed(42));
	const MixedArray marr1(arr1);

	std::vector <Mixed> arr2;
	arr2.push_back(Mixed("42"));
	const MixedArray marr2(arr2);

	const Mixed m3_1(marr1);
	const Mixed m3_2(marr2);
	BOOST_CHECK(m3_1 != m3_2);
}
