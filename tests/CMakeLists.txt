
# Mixed
ADD_EXECUTABLE(
	pherialize-Mixed-test
	Mixed_test.cpp
)

TARGET_LINK_LIBRARIES(
	pherialize-Mixed-test
	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} pherialize
)

ADD_TEST(
	pherialize-Mixed-test
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pherialize-Mixed-test
)

# MixedArray
ADD_EXECUTABLE(
	pherialize-MixedArray-test
	MixedArray_test.cpp
)

TARGET_LINK_LIBRARIES(
	pherialize-MixedArray-test
	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} pherialize
)

ADD_TEST(
	pherialize-MixedArray-test
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pherialize-MixedArray-test
)

# unserialize
ADD_EXECUTABLE(
	pherialize-unserialize-test
	unserialize_test.cpp
)

TARGET_LINK_LIBRARIES(
	pherialize-unserialize-test
	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} pherialize
)

ADD_TEST(
	pherialize-unserialize-test
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pherialize-unserialize-test
)
