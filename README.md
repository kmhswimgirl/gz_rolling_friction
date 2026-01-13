# gz_rolling_friction
A gazebo sim plugin that solves the rolling friction problem in Gazebo Harmonic.

rollingFriction.cc is adapted from this [Robotics Stack Exchange post](https://robotics.stackexchange.com/questions/115581/sdf-example-of-rolling-friction-for-gazebo-harmonic).

## Build Instructions
1. clone this repository
2. cd into the plugin directory
3. `colcon build` 

## Using the plugin in a SDF
Here is an example of the plugin being used with a sphere model:

```xml
<?xml version="1.0" ?>
<sdf version="1.6">
  <model name="sphere-that-does-not-roll-forever">

    <static>false</static>

    <link name="link">

      <pose>0 0 0 0 0 0</pose>
    
      <visual name="visual">
          <geometry>
              <sphere>
                  <radius>0.041</radius> 
              </sphere>
          </geometry>
          <material>
              <ambient>0.8 0.0 0.0 1.0</ambient>
              <diffuse>0.8 0.0 0.0 1.0</diffuse>
              <specular>0.1 0.1 0.1 1.0</specular>
              <emissive>0 0 0 1</emissive>
          </material>
      </visual>

      <collision name="collision">
		    <geometry>
                <sphere>
                    <radius>0.041</radius> 
                </sphere>
            </geometry>
            <surface>
                <friction>
                    <ode>
                        <mu>0.2</mu>
                        <mu2>0.2</mu2>
                    </ode>
                </friction>
            </surface>
      </collision>

      <inertial>
        <mass>0.04</mass> 
        <inertia>
          <ixx>0.0002</ixx>
          <iyy>0.0002</iyy>
          <izz>0.0002</izz>
        </inertia>
      </inertial>
    </link>

    <plugin filename="librollingFriction.so" name="rolling_friction::RollingFrictionPlugin">
      <link_name>link</link_name>
      <coefficient>0.01</coefficient>
    </plugin> 

  </model>
</sdf>

```

## Appending to the `GZ_SYSTEM_SIM_PLUGIN_PATH`
The gazebo plugin path has to be updated in order to use this plugin as it needs to know where the file `librollingFriction.so` is.

After opening your bashrc file using `nano ~/.bashrc` add the following lines and remember to replace `path_to_plugin_folder` with the actual path to your plugin folder:
```bash
# ~/.bashrc

export GZ_SYSTEM_SIM_PLUGIN_PATH='/opt/ros/jazzy/lib' # my usual plugin path
export GZ_SIM_SYSTEM_PLUGIN_PATH=$GZ_SIM_SYSTEM_PLUGIN_PATH:path_to_plugin_folder/gz_rolling_friction/install/gz_rolling_friction/lib/ # previous path + rolling friction plugin
```
