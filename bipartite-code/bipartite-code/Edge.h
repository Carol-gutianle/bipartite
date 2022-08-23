#pragma once
#include"global.h"
class Edge {
private:
	int from;
	int to;
	double weight;
public:
	Edge() {}
	Edge(int from0, int to0, double weight0) {
		from = from0;
		to = to0;
		weight = weight0;
	}
	void setFrom(int from0) {
		from = from0;
	}
	int getFrom() {
		return from;
	}
	void setTo(int to0) {
		to = to0;
	}
	int getTo() {
		return to;
	}
	void setWeight(double weight0) {
		weight = weight0;
	}
	double getWeight() {
		return weight;
	}
};
