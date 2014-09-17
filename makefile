all: apf_mappingv2.cpp
	g++ apf_mappingv2.cpp -o apf `pkg-config --libs opencv`