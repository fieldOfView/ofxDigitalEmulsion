#include "Factory.h"

#include "ofxDigitalEmulsion/Item/RigidBody.h"
#include "ofxDigitalEmulsion/Item/View.h"
#include "ofxDigitalEmulsion/Item/Board.h"
#include "ofxDigitalEmulsion/Item/Camera.h"
#include "ofxDigitalEmulsion/Item/Projector.h"
//#include "ofxDigitalEmulsion/Item/Model.h"

#include "ofxDigitalEmulsion/Device/VideoOutput.h"

#include "ofxDigitalEmulsion/Render/NodeThroughView.h"

#include "ofxDigitalEmulsion/Procedure/Calibrate/CameraIntrinsics.h"
#include "ofxDigitalEmulsion/Procedure/Calibrate/ViewToVertices.h"
#include "ofxDigitalEmulsion/Procedure/Calibrate/HomographyFromGraycode.h"
#include "ofxDigitalEmulsion/Procedure/Scan/Graycode.h"
#include "ofxDigitalEmulsion/Procedure/Triangulate.h"

namespace ofxDigitalEmulsion {
	namespace Graph {
#pragma mark BaseFactory
		//----------
		BaseFactory::BaseFactory() {
			this->icon = nullptr;
		}

		//----------
		ofImage & BaseFactory::getIcon() {
			if (!this->icon) {
				auto node = this->makeUninitialised();
				this->icon = & node->getIcon();
			}
			return *this->icon;
		}

#pragma mark FactoryRegister
		//----------
		FactoryRegister * FactoryRegister::singleton = 0;

		//----------
		FactoryRegister & FactoryRegister::X() {
			if (!FactoryRegister::singleton) {
				auto factoryRegister = new FactoryRegister();
				FactoryRegister::singleton = factoryRegister;
			}

			return *singleton;
		}

		//----------
		FactoryRegister::FactoryRegister() {
			this->add<Item::RigidBody>();
			this->add<Item::View>();
			this->add<Item::Board>();
			this->add<Item::Camera>();
			this->add<Item::Projector>();
			//this->add<Item::Model>();
			this->add<Device::VideoOutput>();
			this->add<Render::NodeThroughView>();
			this->add<Procedure::Calibrate::CameraIntrinsics>();
			this->add<Procedure::Calibrate::ViewToVertices>();
			this->add<Procedure::Calibrate::HomographyFromGraycode>();
			this->add<Procedure::Scan::Graycode>();
			this->add<Procedure::Triangulate>();
		}

		//----------
		void FactoryRegister::add(shared_ptr<BaseFactory> factory) {
			this->insert(pair<string, shared_ptr<BaseFactory>>(factory->getNodeTypeName(), factory));
		}

		//----------
		shared_ptr<BaseFactory> FactoryRegister::get(string nodeTypeName) {
			auto findFactory = this->find(nodeTypeName);
			if (findFactory == this->end()) {
				return shared_ptr<BaseFactory>();
			}
			else {
				return findFactory->second;
			}
		}

		//----------
		shared_ptr<Editor::NodeHost> FactoryRegister::make(const Json::Value & json) {
			const auto nodeTypeName = json["NodeTypeName"].asString();

			auto factory = FactoryRegister::X().get(nodeTypeName);
			if (!factory) {
				throw(Exception("FactoryRegister::make : Missing Factory for Node type " + nodeTypeName));
			}

			auto node = factory->make();
			node->setName(json["Name"].asString());
			try {
				node->deserialize(json["Content"]);
			}
			OFXDIGITALEMULSION_CATCH_ALL_TO_ALERT // don't fail on bad deserialize, just notify user what went wrong

			auto nodeHost = make_shared<Editor::NodeHost>(node);
			
			ofRectangle bounds;
			json["Bounds"] >> bounds;
			nodeHost->setBounds(bounds);
			
			return nodeHost;
		}
	}
}