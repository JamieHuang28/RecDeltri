#include <cstring>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <vector>

#include "concavehull.hpp"
#include "galaxy.hpp"
#include "straight_skeleton_2.hpp"
#include "simplify_polygon.hpp"
#include "simple_join.hpp"
#include "hierarchic_straight_skeleton_2.hpp"

namespace py = pybind11;

py::array_t<double> pyconcavehull(const py::array_t<double>& pts, double chi_factor) {
	py::buffer_info buf = pts.request();
	
	if (buf.ndim != 2) {
		throw std::runtime_error("Number of dimensions must be two");
	}
	
	if (buf.shape[1] != 2) {
		throw std::runtime_error("Second dimension's size must be two");
	}

	double *ptr = static_cast<double *>(buf.ptr);

	std::vector<double> ch = concavehull({ptr, ptr+buf.size}, chi_factor);

	py::buffer_info resbuf = py::buffer_info(
		ch.data(),
		buf.itemsize,
		buf.format,
		buf.ndim,
		{(size_t)(ch.size() >> 1), (size_t)2},
		buf.strides
		);
	return py::array_t<double>(resbuf);
}

py::array_t<double> pygalaxy(const py::array_t<double>& pts, const py::array_t<double>& center, double chi_factor) {
	py::buffer_info buf = pts.request();
	
	if (buf.ndim != 2) {
		throw std::runtime_error("Number of dimensions must be two");
	}
	
	if (buf.shape[1] != 2) {
		throw std::runtime_error("Second dimension's size must be two");
	}

	py::buffer_info buf_center = center.request();

	double *ptr = static_cast<double *>(buf.ptr);
	double *ptr_center = static_cast<double *>(buf_center.ptr);

	std::vector<double> ch = galaxy({ptr, ptr+buf.size}, {ptr_center, ptr_center+buf_center.size}, chi_factor);

	py::buffer_info resbuf = py::buffer_info(
		ch.data(),
		buf.itemsize,
		buf.format,
		buf.ndim,
		{(size_t)(ch.size() >> 1), (size_t)2},
		buf.strides
		);
	return py::array_t<double>(resbuf);
}

py::array_t<double> pyskeleton(const py::array_t<double>& pts) {
	py::buffer_info buf = pts.request();
	
	if (buf.ndim != 2) {
		throw std::runtime_error("Number of dimensions must be two");
	}
	
	if (buf.shape[1] != 2) {
		throw std::runtime_error("Second dimension's size must be two");
	}

	double *ptr = static_cast<double *>(buf.ptr);

	std::vector<double> ch = skeleton({ptr, ptr+buf.size});

	py::buffer_info resbuf = py::buffer_info(
		ch.data(),
		buf.itemsize,
		buf.format,
		2,
		{(size_t)(ch.size() >> 2), (size_t)4},
		{sizeof(double)*4, sizeof(double)}
		);
	return py::array_t<double>(resbuf);
}

py::array_t<double> py_hierarchic_skeleton(const py::array_t<double>& pts) {
	py::buffer_info buf = pts.request();
	
	if (buf.ndim != 2) {
		throw std::runtime_error("Number of dimensions must be two");
	}
	
	if (buf.shape[1] != 2) {
		throw std::runtime_error("Second dimension's size must be two");
	}

	double *ptr = static_cast<double *>(buf.ptr);

	std::vector<double> ch = hierarchicStraightSkeleton({ptr, ptr+buf.size});

	py::buffer_info resbuf = py::buffer_info(
		ch.data(),
		buf.itemsize,
		buf.format,
		2,
		{(size_t)(ch.size() >> 2), (size_t)4},
		{sizeof(double)*4, sizeof(double)}
		);
	return py::array_t<double>(resbuf);
}

py::array_t<double> py_simplify_polylines(const py::array_t<double>& pts, double ratio) {
	py::buffer_info buf = pts.request();
	
	if (buf.ndim != 2) {
		throw std::runtime_error("Number of dimensions must be two");
	}
	
	if (buf.shape[1] != 2) {
		throw std::runtime_error("Second dimension's size must be two");
	}

	double *ptr = static_cast<double *>(buf.ptr);

	std::vector<double> ch = simplifyPolylines({ptr, ptr+buf.size}, ratio);

	py::buffer_info resbuf = py::buffer_info(
		ch.data(),
		buf.itemsize,
		buf.format,
		buf.ndim,
		{(size_t)(ch.size() >> 1), (size_t)2},
		buf.strides
		);
	return py::array_t<double>(resbuf);
}

py::array_t<double> py_simple_join(const py::array_t<double>& pts, const py::array_t<double>& pts_other) {
	py::buffer_info buf = pts.request();
	
	if (buf.ndim != 2) {
		throw std::runtime_error("Number of dimensions must be two");
	}
	
	if (buf.shape[1] != 2) {
		throw std::runtime_error("Second dimension's size must be two");
	}

	double *ptr = static_cast<double *>(buf.ptr);

	py::buffer_info buf_other = pts_other.request();
	double *ptr_other = static_cast<double *>(buf_other.ptr);

	std::vector<double> ch = simple_join::simpleJoin({ptr, ptr+buf.size}, {ptr_other, ptr_other+buf_other.size});

	py::buffer_info resbuf = py::buffer_info(
		ch.data(),
		buf.itemsize,
		buf.format,
		buf.ndim,
		{(size_t)(ch.size() >> 1), (size_t)2},
		buf.strides
		);
	return py::array_t<double>(resbuf);
}

PYBIND11_MODULE(concavehull, m) {
	m.def("concavehull", &pyconcavehull, "Find concave hull from array of 2D points",
	      py::arg("pts"), py::arg("chi_factor") = 0.1);
	m.def("galaxy", &pygalaxy, "Find galaxy from array of 2D points",
	      py::arg("pts"), py::arg("center"), py::arg("chi_factor") = 0.1);
	m.def("skeleton", &pyskeleton, "Find straight skeleton from array of 2D points",
	      py::arg("pts"));
	m.def("simplify_polylines", &py_simplify_polylines, "Simplify polylines",
	      py::arg("pts"), py::arg("ratio") = 0.1);
	m.def("simple_join", &py_simple_join, "Join two polylines",
	      py::arg("pts"), py::arg("pts_other"));
	m.def("hierarchic_skeleton", &py_hierarchic_skeleton, "Find hierarchic straight skeleton from array of 2D points",
	      py::arg("pts"));
}
