# Copyright (c) 2012 Joey Yore
#
# Permission is hereby granted, free of charge, to any person obtaining a 
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense, 
# and/or sell copies of the Software, and to permit persons to whom the 
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
# DEALINGS IN THE SOFTWARE.


CXX 		= /usr/bin/g++
CXXFLAGS 	= -Wall -g -lrt

all: testTicker testTimeout testPID testUtil testADXL testHMC testITG testList testHashTable testCircularBuffer testGPIO testVector testPubsub testIMU testUARTIMU

testTicker:	test_ticker.o
	$(CXX) $(CXXFLAGS) -o ticker test_ticker.o
	@rm -f *.o

testTimeout: test_timeout.o
	$(CXX) $(CXXFLAGS) -o timeout test_timeout.o
	@rm -f *.o

testPID: test_pid.o
	$(CXX) $(CXXFLAGS) -o pid test_pid.o
	@rm -f *.o

testUtil:	test_util.o
	$(CXX) $(CXXFLAGS) -o util test_util.o
	@rm -f *.o

testADXL:	test_adxl345.o
	$(CXX) $(CXXFLAGS) -o adxl345 test_adxl345.o
	@rm -f *.o

testHMC:	test_hmc5883l.o
	$(CXX) $(CXXFLAGS) -o hmc5883l test_hmc5883l.o
	@rm -f *.o

testITG:	test_itg3200.o
	$(CXX) $(CXXFLAGS) -o itg3200 test_itg3200.o
	@rm -f *.o

testList:	test_list.o
	$(CXX) $(CXXFLAGS) -o list test_list.o
	@rm -f *.o

testHashTable:	test_hashTable.o
	$(CXX) $(CXXFLAGS) -o hash test_hashTable.o
	@rm -f *.o

testCircularBuffer: test_CircularBuffer.o
	$(CXX) $(CXXFLAGS) -o cbuf test_CircularBuffer.o
	@rm -f *.o

testGPIO: test_gpio.o
	$(CXX) $(CXXFLAGS) -o blink test_gpio.o
	@rm -f *.o

testVector: test_vector.o
	$(CXX) $(CXXFLAGS) -o vec test_vector.o
	@rm -f *.o

testPubsub: test_pubsub.o
	$(CXX) $(CXXFLAGS) -o pubsub test_pubsub.o
	@rm -f *.o

testIMU:	test_imu.o
	$(CXX) $(CXXFLAGS) -o imu_test test_imu.o
	@rm -f *.o

testUARTIMU: test_uartIMU.o
	$(CXX) $(CXXFLAGS) -o uart_imu test_uartIMU.o
	@rm -f *.o

clean:
	@rm -f *.o ticker timeout pid util adxl345 hmc5883l itg3200 list hash cbuf blink vec pubsub imu_test uart_imu
