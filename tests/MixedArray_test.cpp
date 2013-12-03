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

#define BOOST_TEST_MODULE pherialize_MixedArray test
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "pherialize/Mixed.hpp"
#include "pherialize/MixedArray.hpp"


using namespace pherialize;


BOOST_AUTO_TEST_CASE(MixedArray_constructors) {

	// MixedArray()
	MixedArray m0;
	BOOST_CHECK(m0.type() == MixedArray::TYPE_NONE);

	// MixedArray(const std::vector <Mixed> &v)
	std::vector <Mixed> v;
	v.push_back(Mixed(42));
	v.push_back(Mixed(123));

	MixedArray m1(v);
	BOOST_CHECK(m1.type() == MixedArray::TYPE_VECTOR);
	BOOST_CHECK(m1.vectorValue() == v);

	// MixedArray(const std::map <Mixed, Mixed> &v)
	std::map <Mixed, Mixed> m;
	m[Mixed(42)] = Mixed("forty-two");
	m[Mixed("abc")] = Mixed("def");

	MixedArray m2(m);
	BOOST_CHECK(m2.type() == MixedArray::TYPE_MAP);
	BOOST_CHECK(m2.mapValue() == m);
}
