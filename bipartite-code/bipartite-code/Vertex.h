#pragma once
class Vertex {
private:
	int d;
	int w;
	bool isAssigned;
	bool isLeaf;
	bool isU;
	int idx;
public:
	Vertex() {
		d = 0;
		w = 1;
		isAssigned = false;
		idx = 0;
		isU = false;
		isLeaf = false;
	}
	void setDistance(int d0) {
		d = d0;
	}
	void setWeight(int w0) {
		w = w0;
	}
	void setAssigned() {
		isAssigned = true;
	}
	void setIdx(int i) {
		idx = i;
	}
	void setU() {
		isU = true;
	}	
	void setLeaf() {
		isLeaf= true;
	}
	int getIdx() {
		return idx;
	}
	bool getAssigned() {
		return isAssigned;
	}
	int getDistance() {
		return d;
	}
	int getWeight() {
		return w;
	}
	bool getIsU() {
		return isU;
	}
	bool getIsLeaf() {
		return isLeaf;
	}
};