#include "utils.h"

// vertices and faces of all testing objects
#include "objects.inc"

bool test_all();

int main()
{
    if (test_all()) {
        info("all tests passed, congratulations!");
    } else {
        warn("test failed");
    }

    return 0;
}

bool test_all()
{
    Halfedge_Mesh test_mesh;
    bool result = true;

    for (size_t i = 0; i != object_names.size(); ++i) {
        const char* name = object_names[i].c_str();
        // load the input mesh
        string error_msg = test_mesh.from_poly(object_faces[i], object_vertices[i]);
        if (!error_msg.empty()) {
            warn("failed to load the %s: %s", name, error_msg.c_str());
            result = false;
            continue;
        }
        // load the standard output mesh
        auto [std_vertices, std_edges] = load_std(object_names[i]);

        info("start testing subdivision for %s", name);
        test_mesh.loop_subdivide();
        // test 1: validate the mesh
        info("validate the mesh...");
        auto validation = test_mesh.validate();
        if (validation.has_value()) {
            warn("the result of subdivision is a invalid mesh: %s",
                    validation.value().second.c_str());
            result = false;
        } else {
            info("passed");
            // test 2: vertex positions
            info("compare all vertices...");
            auto [stu_vertices, stu_edges] = extract(test_mesh);
            if (stu_vertices.size() != std_vertices.size()) {
                warn("number of vertices is wrong: %zu (should be %zu)",
                    stu_vertices.size(), std_vertices.size());
                result = false;
            } else {
                auto vertices_map = test_and_map_vertices(std_vertices, stu_vertices);
                if (!vertices_map.has_value()) {
                    warn("at least on of the vertices has incorrect position");
                    result = false;
                } else {
                    info("passed");
                    // test 3: connectivity
                    info("compare all edges...");
                    unordered_map<size_t, size_t>& std_to_stu = vertices_map.value();
                    if (stu_edges.size() != std_edges.size()) {
                        warn("number of edges is wrong: %zu (should be %zu)",
                            stu_edges.size(), std_edges.size());
                        result = false;
                    } else {
                        bool edges_equal = test_edges(std_edges, stu_edges, std_to_stu);
                        if (!edges_equal) {
                            warn("at least one of the edges connects incorrect vertices");
                            result = false;
                        }
                        else {
                            info("passed");
                        }
                    }
                }
            }
        }

        info("end testing subdivision for %s\n", name);
    }

    return result;
}
