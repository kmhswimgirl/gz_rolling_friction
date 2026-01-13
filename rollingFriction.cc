#include <gz/sim/System.hh> 
#include <gz/sim/Model.hh>
#include <gz/sim/components/Link.hh> 
#include <gz/sim/components/ExternalWorldWrenchCmd.hh> 
#include <gz/plugin/Register.hh> 
#include <gz/math/Vector3.hh> 
#include <gz/sim/EntityComponentManager.hh> 
#include <gz/common/Console.hh> 
#include <gz/sim/components/Factory.hh> 
#include <gz/sim/components/Pose.hh> 

namespace rolling_friction { 
	class RollingFrictionPlugin : public gz::sim::System, public gz::sim::ISystemConfigure, public gz::sim::ISystemPreUpdate{
		 
		private: 
			gz::sim::Entity modelEntity{gz::sim::v8::kNullEntity}; 
			gz::sim::Model model{gz::sim::kNullEntity}; 
			gz::sim::Entity link{gz::sim::kNullEntity}; 
			double coefficient = 0.0005; 
			bool valid = false; gz::math::Pose3d previousPose; 
			bool firstUpdate = true; 
		
		public: 
			void Configure(const gz::sim::Entity &entity, const std::shared_ptr<const sdf::Element> &sdf, gz::sim::EntityComponentManager &ecm, gz::sim::EventManager &) override { 
				this->modelEntity = entity; this->model = gz::sim::Model(entity); 
				std::string linkName = sdf->Get<std::string>("link_name", "link").first; 
				this->coefficient = sdf->Get<double>("coefficient", 0.01).first; 
				
				// Use the proper method to get the link entity 
				auto linkEntity = this->model.LinkByName(ecm, linkName);
				 
				this->link = linkEntity; 
				
				if (this->link == gz::sim::kNullEntity){ 
					gzerr << "Link [" << linkName << "] not found in model." << std::endl; 
					return; 
				} 
					this->valid = true; 
			} 
		
		void PreUpdate(const gz::sim::UpdateInfo & _info, gz::sim::EntityComponentManager &ecm) override { 
			if (!this->valid){ 
				return; 
			}
			 // Confirm that this-link is, in fact, a Link. 
			 auto linkComp = ecm.Component<gz::sim::components::Link>(this->link); 
			if (!linkComp) { 
				 gzmsg << "Entity is not a link!\n"; 
				 return; 
			} 
			
			// Calculate current velocity. 
			double dtSec = std::chrono::duration_cast<std::chrono::duration<double>>(_info.dt).count(); 
			gz::math::Vector3d velocity = this->ComputeVelocity(this->modelEntity, ecm, dtSec); 
			// ignmsg << "Ball velocity: " << velocity << std::endl; 
			gz::math::Vector3d linearFrictionForce = -this->coefficient * velocity.Normalized(); 
			// Create a gz::msgs::Wrench object to hold force and torque data 
			gz::msgs::Wrench wrench; 
			wrench.mutable_force()->set_x(linearFrictionForce.X()); 
			wrench.mutable_force()->set_y(linearFrictionForce.Y()); 
			wrench.mutable_force()->set_z(linearFrictionForce.Z()); 
			
			wrench.mutable_torque()->set_x(0); 
			wrench.mutable_torque()->set_y(0); 
			wrench.mutable_torque()->set_z(0); 
			
			// Now directly set the wrench as the component data, applying the friction "counter-force" 
			ecm.SetComponentData<gz::sim::components::ExternalWorldWrenchCmd>(this->link, wrench); 
		} 
		
		gz::math::Vector3d ComputeVelocity(const gz::sim::v8::Entity &link, 
							const gz::sim::v8::EntityComponentManager &ecm, 
							double dt){ 
			auto poseComp = ecm.Component<gz::sim::components::Pose>(link); 
			if (!poseComp || dt <= 0.0){ 
				return gz::math::Vector3d::Zero; 
			} 
			gz::math::Pose3d currentPose = poseComp->Data();
			 
			if (this->firstUpdate){ 
				this->previousPose = currentPose; 
				this->firstUpdate = false; return gz::math::Vector3d::Zero; 
			} 
			
			gz::math::Vector3d velocity = (currentPose.Pos() - this->previousPose.Pos()) / dt; 
			
			this->previousPose = currentPose; 
			
			// ignmsg << "Computed velocity: " << velocity << std::endl; 
			
			return velocity; 
		} 
	}; 
} //namespace
	
GZ_ADD_PLUGIN(rolling_friction::RollingFrictionPlugin, 
			  gz::sim::System, 
			  gz::sim::ISystemConfigure, 
			  gz::sim::ISystemPreUpdate) 
			  
GZ_ADD_PLUGIN_ALIAS(rolling_friction::RollingFrictionPlugin, 
				    "rolling_friction::RollingFrictionPlugin")