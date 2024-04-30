// Utils.cpp

#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "PolygonalMesh.hpp"
#include <vector>

namespace PolygonalLibrary {

bool ImportMesh(const std::string& filepath, PolygonalMesh& mesh) {
    if (!ImportCell0Ds(filepath + "/Cell0Ds.csv", mesh)) {
        std::cerr << "Error importing Cell0Ds." << std::endl;
        return false;
    }
    else {
        std::cout << "Cell0D marker:" << std::endl;
        for (auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++) {
            std::cout << "key:\t" << it->first << "\t values:";
            for (const unsigned int id : it->second)
                std::cout << "\t" << id;

            std::cout << std::endl;
        }
    }

    if (!ImportCell1Ds(filepath + "/Cell1Ds.csv", mesh)) {
        std::cerr << "Error importing Cell1Ds." << std::endl;
        return false;
    }
    else {
        std::cout << "Cell1D marker:" << std::endl;
        for (auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++) {
            std::cout << "key:\t" << it->first << "\t values:";
            for (const unsigned int id : it->second)
                std::cout << "\t" << id;

            std::cout << std::endl;
        }
    }

    if (!ImportCell2Ds(filepath + "/Cell2Ds.csv", mesh)) {
        std::cerr << "Error importing Cell2Ds." << std::endl;
        return false;
    }
    else {
        // Test:
        for (unsigned int c = 0; c < mesh.NumberCell2D; c++) {
            std::vector<unsigned int> edges = mesh.Cell2DEdges[c];

            for (unsigned int e = 0; e < edges.size(); e++) {
                const unsigned int origin = mesh.Cell1DVertices[edges[e]][0];
                const unsigned int end = mesh.Cell1DVertices[edges[e]][1];

                auto findOrigin = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), origin);
                if (findOrigin == mesh.Cell2DVertices[c].end()) {
                    std::cerr << "Wrong mesh" << std::endl;
                    return false;
                }

                auto findEnd = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), end);
                if (findEnd == mesh.Cell2DVertices[c].end()) {
                    std::cerr << "Wrong mesh" << std::endl;
                    return false;
                }
            }
        }
    }

    return true;
}

// Funzione ausiliaria per verificare se una riga contiene dati numerici
bool IsNumericLine(const std::string& line) {
    std::istringstream converter(line);
    std::string token;

    // Prova a estrarre il primo token dalla riga
    if (std::getline(converter, token, ';')) {
        try {
            // Prova a convertire il token in un numero
            std::stod(token);
            // Se la conversione ha successo, la riga contiene dati numerici
            return true;
        } catch (const std::invalid_argument& e) {
            // Se la conversione fallisce, la riga non contiene dati numerici
            return false;
        }
    }

    // Se non c'è nessun token, la riga è vuota e non contiene dati numerici
    return false;
}

// Funzione per l'importazione dei vertici 0D
bool ImportCell0Ds(const std::string& filename, PolygonalMesh& mesh) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    std::string line;
    // Ignora la prima riga
    std::getline(file, line);

    while (std::getline(file, line)) {
        // Se la riga non contiene dati numerici, passa alla prossima riga
        if (!IsNumericLine(line)) {
            continue;
        }

        std::istringstream converter(line);
        std::string token;
        std::array<double, 2> coordinates;

        // Leggi i dati dalla riga
        std::getline(converter, token, ';');
        unsigned int id = std::stoi(token);

        std::getline(converter, token, ';');
        unsigned int marker = std::stoi(token);

        std::getline(converter, token, ';');
        coordinates[0] = std::stod(token);

        std::getline(converter, token, ';');
        coordinates[1] = std::stod(token);

        // Aggiungi i dati alla mesh
        mesh.Cell0DId.push_back(id);
        mesh.Cell0DMarkers[marker].push_back(id);
        mesh.Cell0DCoordinates.push_back(coordinates);
    }

    file.close();
    return true;
}

// Funzione per l'importazione dei vertici 1D
bool ImportCell1Ds(const std::string& filename, PolygonalMesh& mesh) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    std::string line;
    // Ignora la prima riga
    std::getline(file, line);

    while (std::getline(file, line)) {
        // Se la riga non contiene dati numerici, passa alla prossima riga
        if (!IsNumericLine(line)) {
            continue;
        }

        std::istringstream converter(line);
        std::string token;
        std::array<unsigned int, 2> vertices;

        // Leggi i dati dalla riga
        std::getline(converter, token, ';');
        unsigned int id = std::stoi(token);

        std::getline(converter, token, ';');
        unsigned int marker = std::stoi(token);

        std::getline(converter, token, ';');
        vertices[0] = std::stoi(token);

        std::getline(converter, token, ';');
        vertices[1] = std::stoi(token);

        // Aggiungi i dati alla mesh
        mesh.Cell1DId.push_back(id);
        mesh.Cell1DMarkers[marker].push_back(id);
        mesh.Cell1DVertices.push_back(vertices);
    }

    file.close();
    return true;
}

bool ImportCell2Ds(const std::string &filename, PolygonalMesh& mesh) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    unsigned int lineCount = 0; // Contatore per il numero di righe

    // Ignora la prima riga
    std::getline(file, line);

    while (std::getline(file, line)) {
        lineCount++; // Incrementa il contatore di righe

        // Converti la riga in un stream di input
        std::istringstream converter(line);
        std::string token;
        std::array<unsigned int, 3> vertices;
        std::vector<unsigned int> edges; // Modifica in un vettore

        // Split della riga tramite il delimitatore ';'
        std::getline(converter, token, ';');
        try {
            mesh.Cell2DId.push_back(std::stoi(token));
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid ID on line " << lineCount << ": " << token << std::endl;
            continue; // Passa alla prossima riga
        }

        std::getline(converter, token, ';');
        try {
            unsigned int marker = std::stoi(token);
            mesh.Cell2DMarkers[mesh.Cell2DId.back()] = marker;
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid marker on line " << lineCount << ": " << token << std::endl;
            continue; // Passa alla prossima riga
        }

        std::getline(converter, token, ';'); // Leggi il numero di vertici
        unsigned int numVertices;
        std::getline(converter, token, ';');
        try {
            numVertices = std::stoi(token);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid numVertices on line " << lineCount << ": " << token << std::endl;
            continue; // Passa alla prossima riga
        }
        std::vector<unsigned int> vertexList(numVertices);

        // // Controllo sul numero di vertici
        // if (numVertices != 3) {
        //     std::cerr << "Invalid number of vertices on line " << lineCount << ": " << numVertices << std::endl;
        //     continue; // Passa alla prossima riga
        // }

        // Leggi i vertici
        for (unsigned int i = 0; i < numVertices; ++i) {
            std::getline(converter, token, ';');
            try {
                vertexList[i] = std::stoi(token);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid vertex on line " << lineCount << ": " << token << std::endl;
                continue; // Passa alla prossima riga
            }
        }

        // Leggi il numero di bordi
        std::getline(converter, token, ';');
        unsigned int numEdges;
        try {
            numEdges = std::stoi(token);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid numEdges on line " << lineCount << ": " << token << std::endl;
            continue; // Passa alla prossima riga
        }

        // Leggi i bordi
        edges.resize(numEdges); // Ridimensiona il vettore degli edge
        for (unsigned int i = 0; i < numEdges; ++i) {
            std::getline(converter, token, ';');
            try {
                edges[i] = std::stoi(token);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid edge on line " << lineCount << ": " << token << std::endl;
                continue; // Passa alla prossima riga
            }
        }

        // Aggiungi i vertici e i bordi alla mesh
        mesh.Cell2DVertices.push_back({vertices[0], vertices[1], vertices[2]});

        mesh.Cell2DEdges.push_back(edges);

        std::cout << "Current line: " << line << std::endl;
    }

    file.close();
    return true;
}
}




