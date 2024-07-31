#pragma once

#include "concavehull.hpp"
#include <eigen3/Eigen/Core>

Eigen::Vector2f flipCircular(const Eigen::Vector2f &data, const Eigen::Vector2f &center) {
  float radius = 30;

  float origin_x = center.x();
  float origin_y = center.y();
  float safe_radius = radius;

  float dx = data(0) - origin_x;
  float dy = data(1) - origin_y;
  float norm2 = std::sqrt(dx * dx + dy * dy);
  if (norm2 < safe_radius)
    safe_radius = norm2;
  if (norm2 == 0)
    return Eigen::Vector2f(center.x(), center.y());
  Eigen::Vector2f data_flipped;
  data_flipped.x() = dx + 2 * (radius - norm2) * dx / norm2 + origin_x;
  data_flipped.y() = dy + 2 * (radius - norm2) * dy / norm2 + origin_y;
  return data_flipped;
}

std::vector<Eigen::Vector2f> flipCircular(const std::vector<Eigen::Vector2f> &data, const Eigen::Vector2f &center) {
  std::vector<Eigen::Vector2f> data_flipped(data.size() + 1, Eigen::Vector2f(center[0], center[1]));
  std::vector<float> norm_vec(data.size(), 0);
  for (size_t i = 0; i < data.size(); i++) {
    data_flipped.at(i) = flipCircular(data.at(i), center);
  }
  return data_flipped;
}

std::vector<double> flipCircular(std::vector<double> coords, std::vector<double> center) {
  std::vector<double> data_flipped(coords.size(), 0);
  for (size_t i = 0; i < coords.size(); i += 2) {
    Eigen::Vector2f flip = flipCircular(Eigen::Vector2f(coords.at(i), coords.at(i+1)), Eigen::Vector2f(center[0], center[1]));
    data_flipped[i] = flip.x();
    data_flipped[i + 1] = flip.y();
  }
  return data_flipped;
}

std::vector<double> galaxy(const std::vector<double> &coords, const std::vector<double> &center, double chi_factor) {
  std::vector<double> coords_flipped = flipCircular(coords, center);
  std::vector<double> ch_flipped = concavehull(coords_flipped, chi_factor);
  std::vector<double> ch = flipCircular(ch_flipped, center);
  return ch;
}