#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../NDI_CP_Eloss.hh"

namespace py = pybind11;

int add(int i, int j) {
  return i + j;
}

PYBIND11_MODULE(cdi_ndi_py, m) {

  py::class_<rtt_cdi::CParticle>(m, "CParticle")
    .def(py::init<int, double>());

  py::class_<rtt_cdi_ndi::NDI_CP_Eloss>(m, "NDI_CP_Eloss")
    .def(py::init<const std::string &, 
	          rtt_cdi::CParticle, 
  	          rtt_cdi::CParticle >())
    .def(py::init<const std::string &, 
	          const std::string &,
	          rtt_cdi::CParticle, 
	          rtt_cdi::CParticle >())
    .def("getEloss", &rtt_cdi_ndi::NDI_CP_Eloss::getEloss)
    .def("getTemperatureGrid", &rtt_cdi_ndi::NDI_CP_Eloss::getTemperatureGrid)
    .def("getDensityGrid", &rtt_cdi_ndi::NDI_CP_Eloss::getDensityGrid)
    .def("getEnergyGrid", &rtt_cdi_ndi::NDI_CP_Eloss::getEnergyGrid)
    .def("getNumTemperatures", &rtt_cdi_ndi::NDI_CP_Eloss::getNumTemperatures)
    .def("getNumDensities", &rtt_cdi_ndi::NDI_CP_Eloss::getNumDensities)
    .def("getNumEnergies", &rtt_cdi_ndi::NDI_CP_Eloss::getNumEnergies);
    .def("getStoppingData", &rtt_cdi_ndi::NDI_CP_Eloss::get_stopping_data);
}
