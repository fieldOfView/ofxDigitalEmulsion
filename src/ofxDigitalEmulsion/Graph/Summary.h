#pragma once

#include "Node.h"

#include "ofxCvGui/Panels/World.h"

namespace ofxDigitalEmulsion {
	namespace Graph {
		class Summary : public Node {
		public:
			Summary(const Utils::Set<Graph::Node> & world);
			void init();
			string getTypeName() const override;
			ofxCvGui::PanelPtr getView() override;
		protected:
			void serialize(Json::Value &);
			void deserialize(const Json::Value &);
			void populateInspector(ofxCvGui::ElementGroupPtr);
			void callbackShowCursor(bool &);

			void drawGrid();

			shared_ptr<ofxCvGui::Panels::World> view;

			ofParameter<bool> showCursor;
			ofParameter<bool> showGrid;
			ofParameter<ofVec3f> roomMinimum;
			ofParameter<ofVec3f> roomMaximum;
			ofLight light;

			const Utils::Set<Graph::Node> & world;

			ofImage * grid;
		};
	}
}