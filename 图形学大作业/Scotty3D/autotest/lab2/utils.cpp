#include "utils.h"

tuple<vector<Vec3>, set<EdgePair>> extract(const Halfedge_Mesh& mesh)
{
    vector<Vec3> vertices;
    set<EdgePair> edges;
    unordered_map<Index, size_t> id_to_index;
    size_t current_index = 0;
    vertices.reserve(mesh.n_vertices());
    for (VertexCRef vertex = mesh.vertices_begin();
            vertex != mesh.vertices_end(); ++vertex) {
        vertices.push_back(vertex->pos);
        id_to_index.insert({vertex->id(), current_index});
        ++current_index;
    }
    for (EdgeCRef edge = mesh.edges_begin();
            edge != mesh.edges_end(); ++edge) {
        VertexCRef v1 = edge->halfedge()->vertex();
        VertexCRef v2 = edge->halfedge()->twin()->vertex();
        edges.insert({id_to_index[v1->id()], id_to_index[v2->id()]});
    }

    return {vertices, edges};
}

optional<unordered_map<size_t, size_t>>
test_and_map_vertices(const vector<Vec3>& std, const vector<Vec3>& stu)
{
    // This test assumes the size of std is equal with stu.
    // So this function only maps vertex indices of std to indices in stu.
    
    static constexpr float threshold = 1e-3;
    static constexpr float threshold_squ = threshold * threshold;

    unordered_map<size_t, size_t> std_to_stu;
    size_t n_vertices = std.size();
    for (size_t i = 0; i != n_vertices; ++i) {
        bool found = false;
        for (size_t j = 0; j != n_vertices; ++j) {
            float distance_squ = (std[i] - stu[j]).norm_squared();
            if (distance_squ < threshold_squ) {
                std_to_stu[i] = j;
                found = true;
                break;
            }
        }
        if (!found) {
            return std::nullopt;
        }
    }

    return std_to_stu;
}

bool test_edges(
    const set<EdgePair>& std,
    const set<EdgePair>& stu,
    const unordered_map<size_t, size_t>& std_to_stu
)
{
    // This test assumes the size of std is equal with stu
    for (auto it = std.begin(); it != std.end(); ++it) {
        EdgePair edge = {std_to_stu.at(it->first), std_to_stu.at(it->second)};
        EdgePair edge_reversed = {edge.second, edge.first};
        bool found = stu.find(edge) != stu.end();
        bool found_reverse = stu.find(edge_reversed) != stu.end();
        if (!found && !found_reverse) {
            return false;
        }
    }

    return true;
}

tuple<vector<Vec3>, set<EdgePair>> load_std(const string &name)
{
    using namespace std::string_literals;
    string std_path = "./std/"s + name + ".txt"s;
    FILE *std_file = std::fopen(std_path.c_str(), "r");
    size_t n_vertices, n_faces;
    vector<Vec3> vertices;
    set<EdgePair> edges;
    std::fscanf(std_file, "%zu", &n_vertices);
    vertices.reserve(n_vertices);
    for (size_t i = 0; i != n_vertices; ++i) {
        Vec3 pos;
        std::fscanf(std_file, "%f %f %f", &pos.x, &pos.y, &pos.z);
        vertices.push_back(pos);
    }
    std::fscanf(std_file, "%zu", &n_faces);
    for (size_t i = 0; i != n_faces; ++i) {
        size_t v1, v2;
        std::fscanf(std_file, "%zu %zu", &v1, &v2);
        edges.insert({v1, v2});
    }
    fclose(std_file);

    return {vertices, edges};
}
