

#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"

int main() {
    PolygonalLibrary::PolygonalMesh mesh;
    std::string filepath = "PolygonalMesh";

    if (!PolygonalLibrary::ImportMesh(filepath, mesh)) {
        std::cerr << "Error importing mesh." << std::endl;
        return 1;
    }

    // Test the imported properties of the mesh
    // Implement testing logic here

    return 0;
}
