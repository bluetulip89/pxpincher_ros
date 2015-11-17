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

#include "pxpincher_cpp/simulation.h"

Simulation::Simulation()
{
    offsets_ = {0.0,0.0,0.0,0.0,0.0};
    qDots_ = {0.0,0.0,0.0,0.0,0.0};

    currentState_.header.stamp = ros::Time::now();
    currentState_.velocity = qDots_;
    currentState_.position = offsets_;
}

Simulation::Simulation(std::vector<double> offsets):
    offsets_(offsets)
{
    qDots_ = {0.0,0.0,0.0,0.0,0.0};

    currentState_.header.stamp = ros::Time::now();
    currentState_.velocity = qDots_;
    currentState_.position = offsets_;
}

void Simulation::setQDot(std::vector<double> qDot)
{
    qDots_ = qDot;
}

sensor_msgs::JointState Simulation::performSimulationStep(double duration)
{
    std::vector<std::string> names = {"J1","J2","J3","J4","J5"};

    for(int i = 0; i < qDots_.size(); ++i){
        offsets_[i] += qDots_[i]*duration;
    }


    currentState_.header.stamp = ros::Time::now();
    currentState_.name = names;
    currentState_.position = offsets_;
    currentState_.velocity = qDots_;

    return currentState_;
}

sensor_msgs::JointState Simulation::getCurrentState()
{
    return currentState_;
}