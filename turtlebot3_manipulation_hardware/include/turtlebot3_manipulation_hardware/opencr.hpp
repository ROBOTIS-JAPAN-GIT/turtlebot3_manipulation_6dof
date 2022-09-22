// Copyright 2022 ROBOTIS CO., LTD.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Author: Darby Lim

#ifndef TURTLEBOT3_MANIPULATION_HARDWARE__OPENCR_HPP_
#define TURTLEBOT3_MANIPULATION_HARDWARE__OPENCR_HPP_

#include <array>
#include <memory>
#include <mutex>
#include <stdlib.h>
#include <string>

#include "turtlebot3_manipulation_hardware/dynamixel_sdk_wrapper.hpp"
#include "turtlebot3_manipulation_hardware/opencr_control_table.hpp"
#include "turtlebot3_manipulation_hardware/opencr_definitions.hpp"

namespace robotis
{
namespace turtlebot3_manipulation_hardware
{
class OpenCR
{
public:
  explicit OpenCR(const uint8_t & id);
  virtual ~OpenCR();

  template<typename DataByteT>
  DataByteT get_data(const uint16_t & address, const uint16_t & length)
  {
    DataByteT data = 0;
    uint8_t * p_data = reinterpret_cast<uint8_t *>(&data);

    std::lock_guard<std::mutex> lock(buffer_m_);
    switch (length) {
      case 1:
        p_data[0] = data_[address + 0];
        break;

      case 2:
        p_data[0] = data_[address + 0];
        p_data[1] = data_[address + 1];
        break;

      case 4:
        p_data[0] = data_[address + 0];
        p_data[1] = data_[address + 1];
        p_data[2] = data_[address + 2];
        p_data[3] = data_[address + 3];
        break;

      default:
        p_data[0] = data_[address + 0];
        break;
    }

    return data;
  }

  bool open_port(const std::string & usb_port);
  bool set_baud_rate(const uint32_t & baud_rate);

  uint16_t ping(std::string & log);

  bool is_connect_manipulator();
  bool is_connect_wheels();

  void play_sound(uint8_t sound) const;

  void imu_recalibration();
  opencr::IMU get_imu();

  opencr::Battery get_battery();

  void joints_torque(uint8_t onoff) const;
  void wheels_torque(uint8_t onoff) const;

  bool read_all(std::string & log);

  std::array<double, 2> get_wheel_positions();
  std::array<double, 2> get_wheel_velocities();

  std::array<double, 4> get_joint_positions();
  std::array<double, 4> get_joint_velocities();

  double get_gripper_position();
  double get_gripper_velocity();

  bool set_joint_positions(const std::array<double, 4> & radians, std::string & log);
  bool set_joint_profile_acceleration(
    const std::array<int32_t, 4> & acceleration, std::string & log);
  bool set_joint_profile_velocity(const std::array<int32_t, 4> & velocity, std::string & log);

  bool set_gripper_position(const double & meters, std::string & log);
  bool set_gripper_profile_acceleration(const int32_t & acceleration, std::string & log);
  bool set_gripper_profile_velocity(const int32_t & velocity, std::string & log);

  bool set_home_pose(std::string & log);
  bool set_init_pose(std::string & log);

  void send_heartbeat(const uint8_t & count);

  void write_byte(const uint16_t & address, uint8_t data);
  uint8_t read_byte(const uint16_t & address);

private:
  bool set_joint_variables(
    const uint16_t & address,
    const std::array<int32_t, 4> & variables,
    std::string & log);

  bool set_gripper_variables(
    const uint16_t & address, const int32_t & variables, std::string & log);

  std::unique_ptr<DynamixelSDKWrapper> dxl_sdk_wrapper_;

  uint8_t data_[CONTROL_TABLE_SIZE];
  uint8_t data_buffer_[CONTROL_TABLE_SIZE];

  std::mutex buffer_m_;
};
}  // namespace turtlebot3_manipulation_hardware
}  // namespace robotis
#endif  // TURTLEBOT3_MANIPULATION_HARDWARE__OPENCR_HPP_

