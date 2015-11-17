/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2015,
 *  TU Dortmund - Institute of Control Theory and Systems Engineering.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the institute nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Maximilian Krämer, Christoph Rösmann
 *********************************************************************/

#ifndef PXPINCHER_H
#define PXPINCHER_H

#include <string>
#include <math.h>

#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/robot_hw.h>
#include <controller_manager/controller_manager.h>

#include "sensor_msgs/JointState.h"
#include "std_msgs/String.h"
#include "pxpincher_msgs/pxpincher_diagnostic.h"

#include "ros/ros.h"

#include "pxpincher_comm/pxprotocol.h"
#include "pxpincher_comm/serialcomm.h"
#include "pxpincher_comm/servostatus.h"
#include "pxparameter.h"

#include "simulation.h"


class PxPincher : public hardware_interface::RobotHW
{
public:
    PxPincher();
    ~PxPincher();

    void start();

private:

    void update();
    void fillControlRegister(const std::vector<ServoStatus>& stati);
    void calculateControlStep();
    void performAction();
    void initRobot();

    static double tick2rad(int position);
    static std::vector<double> tick2rad(const std::vector<int>& positions);
    
    static double tick2rads(int speed);
    static std::vector<double> tick2rads(const std::vector<int>& speeds);
    
    static double convVoltage(int volt);
    static std::vector<double> convVoltage(const std::vector<int>& volt);

    sensor_msgs::JointState getJointState();
    pxpincher_msgs::pxpincher_diagnostic getDiagnostics(const std::vector<ServoStatus>& stati);

    void simulationCallback(const sensor_msgs::JointStateConstPtr &state);

    ros::Publisher state_publisher_, diagnostic_publisher_;
    ros::Subscriber sim_subscriber_;
    ros::NodeHandle nhandle_;

    hardware_interface::JointStateInterface jnt_state_interface_;
    hardware_interface::PositionJointInterface jnt_position_interface_;

    
    controller_manager::ControllerManager controller_manager_;

    Simulation sim_object_;
    PXParameter params_;

    SerialComm comm_;
    PXProtocol protocol_;

    
    struct JointData
    {
        double cmd = 0;
        double pos = 0;
        double vel = 0;
        double eff = 0;
    };
    
    std::vector<JointData> joint_data_;


    constexpr static double PI = 3.141592653589793;
    constexpr static double conversionFactor1 = 5*PI/3066;
    constexpr static double conversionFactor2 = 19*PI/5115;

    ros::Time last_;

    bool sim_;
    int rate_;

};

#endif // PXPINCHER_H