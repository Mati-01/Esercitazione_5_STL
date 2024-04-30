// Utils.hpp

#pragma once

#include <iostream>
#include <fstream>
#include "PolygonalMesh.hpp"

namespace PolygonalLibrary {

bool ImportMesh(const std::string& filepath, PolygonalMesh& mesh);
bool ImportCell0Ds(const std::string& filename, PolygonalMesh& mesh);
bool ImportCell1Ds(const std::string& filename, PolygonalMesh& mesh);
bool ImportCell2Ds(const std::string& filename, PolygonalMesh& mesh);

} // namespace PolygonalLibrary
