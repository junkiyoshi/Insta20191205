#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(1.5);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->face.clearIndices();
	this->frame.clearIndices();

	for (int i = 0; i < 2; i++) {
	
		auto location = glm::vec3(this->make_point(ofRandom(50, 300), ofRandom(100)), -150);
		this->face.addVertex(location);
		this->frame.addVertex(location);
	}

	for (int i = this->face.getNumVertices() - 1; i >= 0; i--) {

		auto vertex = this->face.getVertex(i);
		vertex.z += 1;

		if (vertex.z < 150) {

			this->face.setVertex(i, vertex);
			this->frame.setVertex(i, vertex);
		}
		else {

			this->face.removeVertex(i);
			this->frame.removeVertex(i);
		}
	}

	for (int i = 0; i < this->face.getNumVertices(); i++) {

		vector<int> near_index;
		for (int k = 0; k < this->face.getNumVertices(); k++) {

			if (i == k) continue;

			if (glm::distance(this->frame.getVertex(i), this->frame.getVertex(k)) < 30) {

				near_index.push_back(k);
			}
		}

		for (int k = 0; k < near_index.size(); k++) {

			this->frame.addIndex(i);
			this->frame.addIndex(near_index[k]);

			for (int m = 0; m < near_index.size(); m++) {

				if (k == m) continue;

				if (glm::distance(this->frame.getVertex(near_index[k]), this->frame.getVertex(near_index[m])) < 30) {

					this->face.addIndex(i);
					this->face.addIndex(near_index[k]);
					this->face.addIndex(near_index[m]);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(270);
	ofRotateZ(ofGetFrameNum() * 0.5);

	ofSetColor(39);
	ofNoFill();

	ofDrawBox(glm::vec3(), 300);

	for (auto& vertex : this->frame.getVertices()) {

		ofDrawSphere(vertex, 1);
	}

	ofSetColor(39, 39, 239);
	this->face.drawFaces();

	ofSetColor(39);
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec2 ofApp::make_point(int len, int param) {

	param = param % 100;
	if (param < 25) {

		return glm::vec2(ofMap(param, 0, 25, -len * 0.5, len * 0.5), -len * 0.5);
	}
	else if (param < 50) {

		return glm::vec2(len * 0.5, ofMap(param, 25, 50, -len * 0.5, len * 0.5));
	}
	else if (param < 75) {

		return glm::vec2(ofMap(param, 50, 75, len * 0.5, -len * 0.5), len * 0.5);
	}
	else {

		return glm::vec2(-len * 0.5, ofMap(param, 75, 100, len * 0.5, -len * 0.5));
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}