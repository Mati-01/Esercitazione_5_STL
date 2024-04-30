
// PolygonalMesh.hpp

#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <map>

namespace PolygonalLibrary {

struct PolygonalMesh {
    unsigned int NumberCell0D = 0;
    std::vector<unsigned int> Cell0DId;
    std::vector<std::array<double, 2>> Cell0DCoordinates;
    std::map<unsigned int, std::vector<unsigned int>> Cell0DMarkers;

    unsigned int NumberCell1D = 0;
    std::vector<unsigned int> Cell1DId;
    std::vector<std::array<unsigned int, 2>> Cell1DVertices;
    std::map<unsigned int, std::vector<unsigned int>> Cell1DMarkers;

    unsigned int NumberCell2D = 0;
    std::vector<unsigned int> Cell2DId;
    std::map<unsigned int, unsigned int> Cell2DMarkers;
    std::vector<std::vector<unsigned int>> Cell2DVertices;
    std::vector<std::vector<unsigned int>> Cell2DEdges;
};

} // namespace PolygonalLibrary
